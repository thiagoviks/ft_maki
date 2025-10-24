#include "../include/ft_maki.h"

// used to manage RAM by doubled linked list
static t_block *head = FT_NULL;
// is used on ft_strtok
static char *saveptr = FT_NULL;

/* xorshift64* PRNG
   state is 64-bit nonzero */
// used for randomness functions
static ft_uint64_t g_prng_state = 0; /* 0 means "not seeded" */

static ft_size_t align(ft_size_t size) {
  return ((size + (FT_ALIGNMENT - 1)) & ~(FT_ALIGNMENT - 1));
}

/*
 * x86_64 syscall convention:
 * rax = syscall number
 * rdi, rsi, rdx, r10, r8, r9 = args 1..6
 * syscall clobbers rcx, r11 and may clobber memory
 *
 * This function returns the raw rax value (signed long).
 */

long ft_syscall(long number, ...) {
  va_list ap;
  long a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0;

  va_start(ap, number);
  a1 = va_arg(ap, long);
  a2 = va_arg(ap, long);
  a3 = va_arg(ap, long);
  a4 = va_arg(ap, long);
  a5 = va_arg(ap, long);
  a6 = va_arg(ap, long);
  va_end(ap);

  register long r10 asm("r10") = a4;
  register long r8 asm("r8") = a5;
  register long r9 asm("r9") = a6;

  register long rax asm("rax") = number;
  register long rdi asm("rdi") = a1;
  register long rsi asm("rsi") = a2;
  register long rdx asm("rdx") = a3;

  asm volatile("syscall"
               : "+r"(rax)
               : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8), "r"(r9)
               : "rcx", "r11", "memory");

  return rax;
}

/* this function call syscall 96 to return current time in seconds and
 * microseconds*/
int ft_gettimeofday(struct s_timeval *tv, void *tz) {
  long ret;

  if (!tv) {
    ft_errno = FT_EINVAL;
    return (-1);
  }

  ret = ft_syscall(FT_SYS_GETTIMEOFDAY, tv, tz);
  if (ret < 0) {
    ft_errno = -ret;
    return (-1);
  }

  return (0);
}

int ft_isatty(int fd) {
  /*The third argument is a pointer to a termios structure — here we simply
pass any buffer; if the ioctl fails with -ENOTTY, the descriptor is not
tty. */
  char buf[8]; /*enough space; kernel only needs non-null pointer*/
  long ret;

  ret = ft_syscall(FT_SYS_IOCTL, (long)fd, (long)FT_TCGETS,
                   (long)(ft_uintptr_t)buf, 0, 0, 0);

  if (ret < 0) {
    /* kernel returns -ERRNO on rax; if negative, we convert */
    long err = -ret;
    /* common limit of negative errno returned from kernel is -4095..-1 */
    if (err > 0 && err <= 4095) {
      ft_errno = (int)err;
    } else {
      ft_errno = -1; /* unknown error */
    }
    /* behavior: isatty returns 0 if not a tty (or error) */
    return 0;
  }
  return 1;
}

ft_ssize_t ft_write(int fd, const void *buf, ft_size_t len) {
  ft_ssize_t ret;
  __asm__ volatile("movq $1, %%rax\n\t" // syscall number for write
                   "movq %1, %%rdi\n\t" // file descriptor
                   "movq %2, %%rsi\n\t" // buffer
                   "movq %3, %%rdx\n\t" // count
                   "syscall\n\t"
                   "movq %%rax, %0\n\t"
                   : "=r"(ret)
                   : "r"((long)fd), "r"(buf), "r"((long)len)
                   : "%rax", "%rdi", "%rsi", "%rdx");
  return (ret);
}

ft_ssize_t ft_read(int fd, void *buf, ft_size_t count) {
  ft_ssize_t ret;
  asm volatile("mov $0, %%rax\n"
               "syscall"
               : "=a"(ret)
               : "D"(fd), "S"(buf), "d"(count)
               : "rcx", "r11", "memory");
  return (ret);
}

int ft_open(const char *pathname, int flags, int mode) {
  int ret;
  asm volatile("mov $2, %%rax\n"
               "syscall"
               : "=a"(ret)
               : "D"(pathname), "S"(flags), "d"(mode)
               : "rcx", "r11", "memory");
  return (ret);
}

int ft_close(int fd) {
  int ret;
  asm volatile("mov $3, %%rax\n"
               "syscall"
               : "=a"(ret)
               : "D"(fd)
               : "rcx", "r11", "memory");
  return (ret);
}

void ft_exit(long code) {
  asm volatile("mov $60, %%rax\n"
               "mov %0, %%rdi\n"
               "syscall"
               :
               : "r"(code)
               : "rax", "rdi");
  __builtin_unreachable();
}

char *ft_strcat(char *s1, const char *s2) {
  int i;
  int j;

  i = 0;
  while (s1[i] != '\0')
    i++;
  j = 0;
  while (s2[j] != '\0') {
    s1[i + j] = (char)s2[j];
    j++;
  }
  s1[i + j] = '\0';
  return (s1);
}

char *ft_strcpy(char *dst, const char *src) {
  char *ret = dst;
  while (*src)
    *dst++ = *src++;
  *dst = '\0';
  return (ret);
}

char *ft_strncpy(char *dest, const char *src, unsigned int n) {
  unsigned int i = 0;

  // copy up to n or until '\0' is found
  while (i < n && src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }

  // fill with '\0' if there is space left
  while (i < n) {
    dest[i] = '\0';
    i++;
  }

  return (dest);
}

ft_size_t ft_strlen(const char *s) {
  ft_size_t len = 0;
  while (s[len])
    len++;
  return (len);
}

void ft_putchar(char c) { ft_write(1, &c, 1); }

void ft_putstr(const char *str) {
  while (*str) {
    ft_write(1, str++, 1);
  }
}

int ft_atoi(const char *str) {
  int result = 0;
  int sign = 1;

  // Skip whitespace
  while (*str == ' ' || (*str >= 9 && *str <= 13))
    str++;

  // Handle sign
  if (*str == '-' || *str == '+') {
    if (*str == '-')
      sign = -1;
    str++;
  }

  // Parse digits
  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }

  return (result * sign);
}

static int char_to_digit(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return (-1);
}

int ft_atoi_base(const char *str, int base) {
  if (base < 2 || base > 16)
    return (0);

  int result = 0;
  int sign = 1;

  // Skip whitespace
  while (*str == ' ' || (*str >= 9 && *str <= 13))
    str++;

  // Handle sign
  if (*str == '-' || *str == '+') {
    if (*str == '-')
      sign = -1;
    str++;
  }

  // Convert digits
  int digit;
  while ((digit = char_to_digit(*str)) >= 0 && digit < base) {
    result = result * base + digit;
    str++;
  }

  return (result * sign);
}

int ft_atoi_auto(const char *str) {
  // Skip whitespace
  while (*str == ' ' || (*str >= 9 && *str <= 13))
    str++;

  // Detect base prefix
  if (*str == '0') {
    if (*(str + 1) == 'x' || *(str + 1) == 'X')
      return (ft_atoi_base(str + 2, 16)); // hex
    else if (*(str + 1) >= '0' && *(str + 1) <= '7')
      return (ft_atoi_base(str + 1, 8)); // octal
  }

  return (ft_atoi_base(str, 10)); // default decimal
}

/* -----------------------------
   atof implementation (string -> double)
   Supports:
   - whitespace, optional sign
   - decimal floats with exponent (e/E)
   - hexadecimal floats with binary exponent (p/P)
   - inf, infinity, nan
   ----------------------------- */

static double pow10_int(int exp) {
  double result = 1.0, base = 10.0;
  int e = exp;
  if (e < 0) {
    e = -e;
    while (e) {
      if (e & 1)
        result /= base;
      base *= base;
      e >>= 1;
    }
  } else {
    while (e) {
      if (e & 1)
        result *= base;
      base *= base;
      e >>= 1;
    }
  }
  return (result);
}

static double pow2_int(int exp) {
  double result = 1.0, base = 2.0;
  int e = exp;
  if (e < 0) {
    e = -e;
    while (e) {
      if (e & 1)
        result /= base;
      base *= base;
      e >>= 1;
    }
  } else {
    while (e) {
      if (e & 1)
        result *= base;
      base *= base;
      e >>= 1;
    }
  }
  return (result);
}

static int starts_with_ci(const char *s, const char *prefix) {
  while (*prefix) {
    if (ft_tolower((unsigned char)*s) != ft_tolower((unsigned char)*prefix))
      return 0;
    s++;
    prefix++;
  }
  return (1);
}

/* parse hexadecimal float */
static double parse_hex_float(const char *p, int sign) {
  double value = 0.0;
  int any_digits = 0;

  // parse 0x prefix already consumed
  while (ft_isxdigit((unsigned char)*p)) {
    int digit;
    if (*p >= '0' && *p <= '9')
      digit = *p - '0';
    else if (*p >= 'a' && *p <= 'f')
      digit = 10 + (*p - 'a');
    else
      digit = 10 + (*p - 'A');
    value = value * 16.0 + digit;
    p++;
    any_digits = 1;
  }

  double frac = 0.0;
  double base = 1.0 / 16.0;
  if (*p == '.') {
    p++;
    while (ft_isxdigit((unsigned char)*p)) {
      int digit;
      if (*p >= '0' && *p <= '9')
        digit = *p - '0';
      else if (*p >= 'a' && *p <= 'f')
        digit = 10 + (*p - 'a');
      else
        digit = 10 + (*p - 'A');
      frac += digit * base;
      base /= 16.0;
      p++;
      any_digits = 1;
    }
  }

  value += frac;
  if (!any_digits)
    return 0.0;

  int exp_sign = 1, exp_val = 0;
  if (*p == 'p' || *p == 'P') {
    p++;
    if (*p == '+') {
      p++;
    } else if (*p == '-') {
      exp_sign = -1;
      p++;
    }
    while (ft_isdigit((unsigned char)*p)) {
      int d = *p - '0';
      if (exp_val < 1000000)
        exp_val = exp_val * 10 + d;
      p++;
    }
  }

  int final_exp = exp_sign * exp_val;
  if (final_exp != 0)
    value = value * pow2_int(final_exp);
  return (sign * value);
}

double ft_atof(const char *s) {
  const char *p = s;
  while (ft_isspace((unsigned char)*p))
    p++;

  int sign = 1;
  if (*p == '+')
    p++;
  else if (*p == '-') {
    sign = -1;
    p++;
  }

  if (starts_with_ci(p, "nan")) {
    ft_uint64_t nan_bits = 0x7FF8000000000001ULL;
    double nanval;
    ft_memcpy(&nanval, &nan_bits, sizeof(nanval));
    return (nanval * sign);
  }
  if (starts_with_ci(p, "inf")) {
    ft_uint64_t inf_bits = 0x7FF0000000000000ULL;
    double infval;
    ft_memcpy(&infval, &inf_bits, sizeof(infval));
    return (infval * sign);
  }

  // detect hex float
  if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
    return (parse_hex_float(p + 2, sign));
  }

  double value = 0.0;
  int any_digits = 0;
  while (ft_isdigit((unsigned char)*p)) {
    any_digits = 1;
    value = value * 10.0 + (*p - '0');
    p++;
  }
  if (*p == '.') {
    p++;
    double place = 1.0;
    while (ft_isdigit((unsigned char)*p)) {
      any_digits = 1;
      place *= 10.0;
      value = value * 10.0 + (*p - '0');
      p++;
    }
    if (place != 1.0)
      value /= place;
  }
  if (!any_digits)
    return 0.0 * sign;

  int exp_sign = 1, exp_val = 0;
  if (*p == 'e' || *p == 'E') {
    p++;
    if (*p == '+')
      p++;
    else if (*p == '-') {
      exp_sign = -1;
      p++;
    }
    if (!ft_isdigit((unsigned char)*p))
      return sign * value;
    while (ft_isdigit((unsigned char)*p)) {
      if (exp_val < 1000000)
        exp_val = exp_val * 10 + (*p - '0');
      p++;
    }
  }
  int final_exp = exp_sign * exp_val;
  if (final_exp != 0)
    value *= pow10_int(final_exp);

  return (sign * value);
}

int ft_strcmp(const char *s1, const char *s2) {
  int i;

  i = 0;
  while (s1[i] == s2[i] && s1[i])
    i++;
  return (s1[i] - s2[i]);
}

char *ft_strdup(const char *src) {
  char *str;
  int i = 0;

  while (src[i] != '\0')
    i++;

  str = ft_malloc(sizeof(char) * (ft_strlen((char *)src) + 1));
  if (!str)
    return (FT_NULL);
  i = 0;
  while (src[i]) {
    str[i] = src[i];
    i++;
  }
  str[i] = '\0';
  return (str);
}

static int is_delim(char c, const char *delim) {
  while (*delim) {
    if (c == *delim)
      return (1);
    delim++;
  }
  return (0);
}

char *ft_strtok(char *str, const char *delim) {
  if (str)
    saveptr = str;
  else if (!saveptr)
    return (FT_NULL);

  // Pula delimitadores iniciais
  while (*saveptr && is_delim(*saveptr, delim))
    saveptr++;

  if (*saveptr == '\0')
    return (FT_NULL);

  char *token_start = saveptr;

  while (*saveptr && !is_delim(*saveptr, delim))
    saveptr++;

  if (*saveptr) {
    *saveptr = '\0';
    saveptr++;
  }

  return (token_start);
}

char *ft_strpbrk(const char *s, const char *accept) {
  while (*s) {
    const char *a = accept;
    while (*a) {
      if (*s == *a)
        return (char *)s;
      a++;
    }
    s++;
  }
  return (FT_NULL);
}

static t_block *find_free_block(ft_size_t size) {
  t_block *current = head;
  while (current) {
    if (current->free && current->size >= size) {
      return (current);
    }
    current = current->next;
  }
  return (FT_NULL);
}

static t_block *request_space(ft_size_t size) {
  void *addr;
  t_block *block;

  if (size >= FT_MMAP_THRESHOLD) {
    ft_size_t total = sizeof(t_block) + size;

    asm volatile("mov $9, %%rax\n"    // syscall number for mmap
                 "mov $0, %%rdi\n"    // addr = NULL
                 "mov %1, %%rsi\n"    // length
                 "mov $0x22, %%rdx\n" // PROT_READ | PROT_WRITE
                 "mov $0x21, %%r10\n" // MAP_PRIVATE | MAP_ANONYMOUS
                 "mov $-1, %%r8\n"    // fd = -1
                 "mov $0, %%r9\n"     // offset = 0
                 "syscall\n"
                 "mov %%rax, %0\n"
                 : "=r"(addr)
                 : "r"(total)
                 : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9");

    if (addr == (void *)-1) {
      return (FT_NULL);
    }

    block = (t_block *)addr;
    block->size = size;
    block->free = 0;
    block->is_mmap = 1;
    block->next = FT_NULL;
    block->prev = FT_NULL;

    return (block);
  }
  // syscall brk
  void *old_brk;
  asm volatile("mov $12, %%rax\n"
               "syscall\n"
               "mov %%rax, %0\n"
               : "=r"(old_brk)
               :
               : "rax");

  void *new_brk = (char *)old_brk + sizeof(t_block) + size;

  void *result;
  asm volatile("mov $12, %%rax\n"
               "mov %1, %%rdi\n"
               "syscall\n"
               "mov %%rax, %0\n"
               : "=r"(result)
               : "r"(new_brk)
               : "rax", "rdi");
  if (result != new_brk)
    return (FT_NULL);

  block = (t_block *)old_brk;
  block->size = size;
  block->free = 0;
  block->is_mmap = 0;
  block->next = FT_NULL;
  block->prev = FT_NULL;

  if (!head)
    head = block;
  else {
    t_block *current = head;
    while (current->next)
      current = current->next;
    current->next = block;
    block->prev = current;
  }

  return block;
}

void *ft_malloc(ft_size_t size) {
  if (size == 0)
    return (FT_NULL);

  size = align(size);
  t_block *block = find_free_block(size);

  if (block) {
    block->free = 0;
    return ((void *)(block + 1));
  }

  block = request_space(size);
  if (!block)
    return (FT_NULL);

  return ((void *)(block + 1));
}

static void coalesce(t_block *block) {
  if (block->next && block->next->free && !block->next->is_mmap) {
    block->size += sizeof(t_block) + block->next->size;
    block->next = block->next->next;
    if (block->next)
      block->next->prev = block;
  }
  if (block->prev && block->prev->free && !block->prev->is_mmap) {
    block->prev->size += sizeof(t_block) + block->size;
    block->prev->next = block->next;
    if (block->next)
      block->next->prev = block->prev;
  }
}

void ft_free(void *ptr) {
  if (!ptr)
    return;
  t_block *block = ((t_block *)ptr) - 1;
  if (block->is_mmap) {
    // syscall: munmap
    asm volatile("mov $11, %%rax\n" // munmap
                 "mov %0, %%rdi\n"
                 "mov %1, %%rsi\n"
                 "syscall\n"
                 :
                 : "r"(block), "r"(block->size + sizeof(t_block))
                 : "rax", "rdi", "rsi");
    return;
  }
  block->free = 1;
  coalesce(block);
}

// Custom memset implementation
void *ft_memset(void *ptr, int value, ft_size_t num) {
  unsigned char *p = ptr; // Treat the pointer as a byte pointer
  while (num--) {
    *p = (unsigned char)value; // Set each byte to the given value
    p++;                       // Move to the next byte
  }
  return (ptr); // Return the original pointer
}

// Custom memcpy implementation
void *ft_memcpy(void *dest, const void *src, ft_size_t num) {
  unsigned char *d = dest;      // Destination pointer
  const unsigned char *s = src; // Source pointer
  while (num--) {
    *d = *s; // Copy each byte from source to destination
    d++;     // Move to the next byte in the destination
    s++;     // Move to the next byte in the source
  }
  return (dest); // Return the destination pointer
}

// Custom calloc implementation
void *ft_calloc(ft_size_t nmemb, ft_size_t size) {
  ft_size_t total_size = nmemb * size; // Calculate total size required
  void *ptr = ft_malloc(total_size);   // Allocate memory using malloc
  if (ptr)
    ft_memset(ptr, 0, total_size); // Initialize the allocated memory to 0
  return (ptr);
}

// Custom realloc implementation
void *ft_realloc(void *ptr, ft_size_t size) {
  if (!ptr)
    return ft_malloc(size); // If the pointer is NULL, behave like malloc()
  if (size == 0) {
    ft_free(ptr);
    return (FT_NULL);
  } // If size is 0, free the memory and return NULL

  t_block *block = (t_block *)ptr - 1;
  if (block->size >= size)
    return (ptr); // If the current block is already large enough, return the
                  // same pointer

  // If the block cannot be expanded, allocate a new block with the requested
  // size
  void *new_ptr = ft_malloc(size);
  if (!new_ptr)
    return (FT_NULL); // If malloc fails, return NULL

  // Copy the data from the old block to the new block
  ft_memcpy(new_ptr, ptr, block->size);
  ft_free(ptr);     // Free the old block
  return (new_ptr); // Return the new block
}

void ft_reverse(char *str, int length) {
  int start;
  int end;
  char temp;

  start = 0;
  end = length - 1;
  while (start < end) {
    temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}

int ft_itoa_str(int num, char *str) {
  int i;
  int is_negative;

  i = 0;
  is_negative = 0;
  if (num < 0) {
    is_negative = 1;
    num = -num;
  }
  if (num == 0)
    str[i++] = '0';
  while (num > 0) {
    str[i++] = (num % 10) + '0';
    num /= 10;
  }
  if (is_negative)
    str[i++] = '-';
  str[i] = '\0';
  ft_reverse(str, i);
  return (i);
}

void ft_map(int *arr, ft_size_t size, int (*func)(int)) {
  ft_size_t i;
  int result;
  char output[20];
  int length;

  i = 0;
  while (i < size) {
    result = func(arr[i]);
    length = ft_itoa_str(result, output);
    ft_write(1, output, length);
    ft_write(1, "\n", 1);
    i++;
  }
}

ft_ssize_t ft_getline(char **lineptr, ft_size_t *n, int fd) {
  if (!lineptr || !n)
    return (-1);

  if (*lineptr == FT_NULL || *n == 0) {
    *n = 128;
    *lineptr = (char *)ft_malloc(*n);
    if (!*lineptr)
      return (-1);
  }

  ft_size_t i = 0;
  char c;
  ft_ssize_t bytes_read;

  while (1) {
    bytes_read = ft_read(fd, &c, 1);
    if (bytes_read <= 0)
      break;

    // Reallocate if buffer is not sufficient
    if (i >= *n - 1) {
      ft_size_t new_size = *n * 2;
      char *new_ptr = (char *)ft_realloc(*lineptr, new_size);
      if (!new_ptr)
        return (-1);
      *lineptr = new_ptr;
      *n = new_size;
    }

    (*lineptr)[i++] = c;
    if (c == '\n')
      break;
  }

  if (i == 0)
    return (-1);

  (*lineptr)[i] = '\0';
  return (i);
}

// is used on ft_perror start
int ft_errno; // or extern if defined elsewhere
static const char *current_lang = "en";

void ft_set_language(const char *lang) { current_lang = lang; }

// Function for each language
static const char *msg_success(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Sucesso");
  return ("Success");
}

static const char *msg_operation_not_permitted(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Operação nao permitida");
  return ("Operation not permitted");
}

static const char *msg_no_such_file_or_directory(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Arquivo ou diretorio inexistente");
  return ("No such file or directory");
}

static const char *msg_no_such_process(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Nao existe esse processo");
  return ("No such process");
}

static const char *msg_interrupted_system_call(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Chamada de sistema interrompida");
  return ("Interrupted system call");
}

static const char *msg_input_output_error(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Erro de entrada e saida");
  return ("Input/output error");
}

static const char *msg_no_such_device_or_address(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Nao existe tal dispositivo ou endereco");
  return ("No such device or address");
}

static const char *msg_argument_list_too_long(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Lista de argumentos muito longa");
  return ("Argument list too long");
}

static const char *msg_exec_format_error(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Erro de Formato do exec");
  return ("Exec format error");
}

static const char *msg_bad_file_descriptor(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Erro de descricao do arquivo");
  return ("Bad file descriptor");
}

static const char *msg_no_child_processes(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Nenhum processo filho");
  return ("No child processes");
}

static const char *msg_out_of_memory(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Sem memoria");
  return ("Out of memory");
}

static const char *msg_permission_denied(void) {
  if (ft_strcmp(current_lang, "pt") == 0)
    return ("Permissao negada");
  return ("Permission denied");
}

static const t_error g_errors[] = {{0, msg_success},
                                   {1, msg_operation_not_permitted},
                                   {2, msg_no_such_file_or_directory},
                                   {3, msg_no_such_process},
                                   {4, msg_interrupted_system_call},
                                   {5, msg_input_output_error},
                                   {6, msg_no_such_device_or_address},
                                   {7, msg_argument_list_too_long},
                                   {8, msg_exec_format_error},
                                   {9, msg_bad_file_descriptor},
                                   {10, msg_no_child_processes},
                                   {12, msg_out_of_memory},
                                   {13, msg_permission_denied}};

// is used on ft_perror end
// binary search

static const char *ft_get_error_message(int err) {
  ft_size_t left = 0;
  ft_size_t right = sizeof(g_errors) / sizeof(t_error) - 1;

  while (left <= right) {
    ft_size_t mid = left + (right - left) / 2;

    if (g_errors[mid].code == err)
      return g_errors[mid].get_message();
    else if (g_errors[mid].code < err)
      left = mid + 1;
    else
      right = mid - 1;
  }
  return ((ft_strcmp(current_lang, "pt") == 0) ? "Erro desconhecido"
                                               : "Unknown error");
}

void ft_perror(const char *str) {
  if (str && *str)
    ft_printf("%s: ", str);
  ft_printf("%s\n", ft_get_error_message(ft_errno));
}

const char *ft_strerror(int err) { return (ft_get_error_message(err)); }

void ft_list_all_errors(void) {
  ft_size_t len = sizeof(g_errors) / sizeof(t_error);
  for (ft_size_t code = 0; code <= len; code++)
    ft_printf("Error %d: %s\n", code, ft_get_error_message(code));
}

int ft_sscanf(const char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);

  int assigned = 0;
  while (*format && *str) {
    if (*format == '%') {
      format++;
      if (*format == 'd') {
        int *iptr = va_arg(args, int *);
        *iptr = ft_atoi(str);
        while (*str == ' ' || (*str >= '0' && *str <= '9') || *str == '-')
          str++;
        assigned++;
      } else if (*format == 's') {
        char *sptr = va_arg(args, char *);
        while (*str == ' ')
          str++; // Skip leading spaces
        while (*str && *str != ' ') {
          *sptr++ = *str++;
        }
        *sptr = '\0';
        assigned++;
      }
    } else if (*format == *str) {
      str++;
    }
    format++;
  }
  va_end(args);
  return (assigned);
}

char *ft_utoa_base(unsigned int value, char *buf, int base) {
  static const char digits[] = "0123456789abcdef";
  char tmp[32];
  int i = 0;

  if (base < 2 || base > 16) {
    buf[0] = '\0';
    return (buf);
  }

  // Special case for 0
  if (value == 0) {
    buf[0] = '0';
    buf[1] = '\0';
    return (buf);
  }

  while (value > 0) {
    tmp[i++] = digits[value % base];
    value /= base;
  }

  // Reverse the result into the output buffer
  for (int j = 0; j < i; ++j)
    buf[j] = tmp[i - j - 1];
  buf[i] = '\0';

  return (buf);
}

char *ft_itoa_base(int value, char *buf, int base) {
  if (value < 0 && base == 10) {
    *buf++ = '-';
    ft_utoa_base((unsigned int)(-value), buf, base);
    return (buf - 1);
  } else {
    return (ft_utoa_base((unsigned int)value, buf, base));
  }
}

int ft_sprintf(char *buf, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char *p = buf;

  while (*fmt) {
    if (*fmt == '%' && *(fmt + 1)) {
      fmt++;
      if (*fmt == 's') {
        const char *s = va_arg(args, const char *);
        while (*s)
          *p++ = *s++;
      } else if (*fmt == 'd') {
        int val = va_arg(args, int);
        char tmp[20];
        ft_itoa_base(val, tmp, 10); // implement this if not done
        const char *s = tmp;
        while (*s)
          *p++ = *s++;
      } else if (*fmt == 'x') {
        unsigned int val = va_arg(args, unsigned int);
        char tmp[20];
        ft_utoa_base(val, tmp, 16); // implement this too
        const char *s = tmp;
        while (*s)
          *p++ = *s++;
      } else if (*fmt == 'c') {
        char c = (char)va_arg(args, int);
        *p++ = c;
      } else {
        *p++ = '%';
        *p++ = *fmt;
      }
    } else {
      *p++ = *fmt;
    }
    fmt++;
  }

  *p = '\0';
  va_end(args);
  return ((int)(p - buf));
}

/* splitmix64: transform seed (32/64 bits) into 64-bit "good" seed state */
static inline ft_uint64_t splitmix64_next(ft_uint64_t *state) {
  ft_uint64_t z = (*state += 0x9E3779B97F4A7C15ULL);
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return (z ^ (z >> 31));
}

/* ft_srandom: initializes the generator with a 32-bit seed*/
void ft_srandom(unsigned int seed) {
  ft_uint64_t st = (ft_uint64_t)seed;
  /* build a good 64-bit state from the seed (splitmix64) */
  ft_uint64_t sm = st;
  /* Populate the state with at least one non-null value */
  g_prng_state = splitmix64_next(&sm);
  if (g_prng_state == 0)
    g_prng_state = 0xF39A4B1C2D3E5F67ULL; /* fallback */
}

/* generates 64-bit pseudo-random using xorshift64 */
static ft_uint64_t ft_xorshift64star(void) {
  ft_uint64_t x = g_prng_state;
  if (x == 0) {
    /* if not seeded, initialize with value derived from a constant */
    ft_uint64_t init = 0x243F6A8885A308D3ULL; /* any non-zero value */
    g_prng_state = init;
    x = init;
  }
  /* xorshift64* variant */
  x ^= x >> 12; // a
  x ^= x << 25; // b
  x ^= x >> 27; // c
  g_prng_state = x;
  return (x * 2685821657736338717ULL);
}

/* ft_random_from_my: returns a non-negative long (similar to random()) */
long ft_random(void) {
  ft_uint64_t r = ft_xorshift64star();
  /* map to 31 non-negative bits (compatible with RAND_MAX 0x7fffffff) */
  return ((long)(r >> 33)); /* 64-31 = 33 -> uses the highest 31 bits */
}

/* optional: rand_r-like function that uses a seed by reference (32-bit) */
int ft_rand_r(unsigned int *seedp) {
  /* simple local LCG (not related to xorshift) for rand_r compat */
  if (!seedp)
    return (-1);
  ft_uint32_t s = *seedp;
  s = s * 1103515245u + 12345u;
  *seedp = s;
  return ((int)((s >> 16) & 0x7FFF));
}

ft_time_t ft_time(ft_time_t *tloc) {
  /* ft_syscall(FT_SYS_time, tloc) retorna seconds since epoch or -1 on error */
  long ret = ft_syscall(FT_SYS_time, tloc);
  if (ret == -1) {
    /* errno already adjusted by the syscall wrapper if necessary*/
    return ((ft_time_t)-1);
  }
  return ((ft_time_t)ret);
}

char *ft_fgets(char *str, int size, T_FT_FILE *stream) {
  int i = 0;
  while (i < size - 1) {
    if (stream->buf_pos >= stream->buf_len) {
      ft_ssize_t n = ft_read(stream->fd, stream->buffer, FT_BUFSIZ);
      if (n <= 0) {
        if (n == 0 || stream->eof)
          break;
        stream->error = 1;
        return (FT_NULL);
      }
      stream->file_pos += n;
      stream->buf_pos = 0;
      stream->buf_len = n;
    }

    char c = stream->buffer[stream->buf_pos++];
    str[i++] = c;

    if (c == '\n')
      break; // Stop at newline

    if (stream->buf_pos == stream->buf_len && i < size - 1) {
      str[i] = '\0'; // Handle partial line
      return (str);
    }
  }
  str[i] = '\0'; // Null-terminate string
  return ((i > 0) ? str : FT_NULL);
}