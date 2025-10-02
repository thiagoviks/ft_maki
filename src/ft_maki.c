#include "../include/ft_maki.h"

//used to manage RAM by doubled linked list
static t_block *head = FT_NULL; 
//is used on ft_strtok 
static char *saveptr = FT_NULL;

static size_t align(size_t size)
{
    return ((size + (FT_ALIGNMENT - 1)) & ~(FT_ALIGNMENT - 1));
}

ssize_t ft_write(int fd, const void *buf, size_t len)
{
    ssize_t ret;
    __asm__ volatile (
		"movq $1, %%rax\n\t"        // syscall number for write
		"movq %1, %%rdi\n\t"        // file descriptor
		"movq %2, %%rsi\n\t"        // buffer
		"movq %3, %%rdx\n\t"        // count
		"syscall\n\t"
		"movq %%rax, %0\n\t"
		: "=r" (ret)
		: "r" ((long)fd), "r" (buf), "r" ((long)len)
		: "%rax", "%rdi", "%rsi", "%rdx"
	);
    return (ret);
}

ssize_t ft_read(int fd, void *buf, size_t count)
{
    ssize_t ret;
    asm volatile (
        "mov $0, %%rax\n"
        "syscall"
        : "=a"(ret)
        : "D"(fd), "S"(buf), "d"(count)
        : "rcx", "r11", "memory"
    );
    return (ret);
}

int ft_open(const char *pathname, int flags, int mode)
{
    int ret;
    asm volatile (
        "mov $2, %%rax\n"
        "syscall"
        : "=a"(ret)
        : "D"(pathname), "S"(flags), "d"(mode)
        : "rcx", "r11", "memory"
    );
    return (ret);
}

int ft_close(int fd)
{
    int ret;
    asm volatile (
        "mov $3, %%rax\n"
        "syscall"
        : "=a"(ret)
        : "D"(fd)
        : "rcx", "r11", "memory"
    );
    return (ret);
}

void ft_exit(long code)
{
    asm volatile (
        "mov $60, %%rax\n"
        "mov %0, %%rdi\n"
        "syscall"
        : : "r"(code) : "rax", "rdi"
    );
    __builtin_unreachable();
}

char	*ft_strcat(char *s1, const char *s2)
{
	int i;
	int j;

	i = 0;
	while (s1[i] != '\0')
		i++;
	j = 0;
	while (s2[j] != '\0')
	{
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

size_t ft_strlen(const char *s)
{
    size_t len = 0;
    while (s[len])
        len++;
    return (len);
}

void	ft_putchar(char c)
{
	ft_write(1, &c, 1);
}

void    ft_putstr(const char *str)
{
    while (*str)
    {
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
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
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
            return (ft_atoi_base(str + 1, 8));  // octal
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
            if (e & 1) result /= base;
            base *= base;
            e >>= 1;
        }
    } else {
        while (e) {
            if (e & 1) result *= base;
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
            if (e & 1) result /= base;
            base *= base;
            e >>= 1;
        }
    } else {
        while (e) {
            if (e & 1) result *= base;
            base *= base;
            e >>= 1;
        }
    }
    return (result);
}

static int starts_with_ci(const char *s, const char *prefix) {
    while (*prefix) {
        if (ft_tolower((unsigned char)*s) != ft_tolower((unsigned char)*prefix)) return 0;
        s++; prefix++;
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
        if (*p >= '0' && *p <= '9') digit = *p - '0';
        else if (*p >= 'a' && *p <= 'f') digit = 10 + (*p - 'a');
        else digit = 10 + (*p - 'A');
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
            if (*p >= '0' && *p <= '9') digit = *p - '0';
            else if (*p >= 'a' && *p <= 'f') digit = 10 + (*p - 'a');
            else digit = 10 + (*p - 'A');
            frac += digit * base;
            base /= 16.0;
            p++;
            any_digits = 1;
        }
    }

    value += frac;
    if (!any_digits) return 0.0;

    int exp_sign = 1, exp_val = 0;
    if (*p == 'p' || *p == 'P') {
        p++;
        if (*p == '+') { p++; }
        else if (*p == '-') { exp_sign = -1; p++; }
        while (ft_isdigit((unsigned char)*p)) {
            int d = *p - '0';
            if (exp_val < 1000000) exp_val = exp_val * 10 + d;
            p++;
        }
    }

    int final_exp = exp_sign * exp_val;
    if (final_exp != 0) value = value * pow2_int(final_exp);
    return (sign * value);
}

double  ft_atof(const char *s)
{
    const char *p = s;
    while (ft_isspace((unsigned char)*p)) p++;

    int sign = 1;
    if (*p == '+') p++;
    else if (*p == '-') { sign = -1; p++; }

    if (starts_with_ci(p, "nan")) {
        uint64_t nan_bits = 0x7FF8000000000001ULL;
        double nanval;
        ft_memcpy(&nanval, &nan_bits, sizeof(nanval));
        return (nanval * sign);
    }
    if (starts_with_ci(p, "inf")) {
        uint64_t inf_bits = 0x7FF0000000000000ULL;
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
        if (place != 1.0) value /= place;
    }
    if (!any_digits) return 0.0 * sign;

    int exp_sign = 1, exp_val = 0;
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+') p++;
        else if (*p == '-') { exp_sign = -1; p++; }
        if (!ft_isdigit((unsigned char)*p)) return sign * value;
        while (ft_isdigit((unsigned char)*p)) {
            if (exp_val < 1000000) exp_val = exp_val * 10 + (*p - '0');
            p++;
        }
    }
    int final_exp = exp_sign * exp_val;
    if (final_exp != 0) value *= pow10_int(final_exp);

    return (sign * value);
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i;

    i = 0;
    while(s1[i] == s2[i] && s1[i])
        i++;
    return (s1[i] - s2[i]);
}

char    *ft_strdup(const char *src)
{
    char *str;
    int i = 0;

    while (src[i] != '\0')
        i++;

    str = ft_malloc(sizeof(char) * (ft_strlen((char *)src) + 1));
    if (!str)
        return (FT_NULL);
    i = 0;
    while (src[i])
    {
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

static t_block *find_free_block(size_t size)
{
    t_block *current = head;
    while (current)
    {
        if (current->free && current->size >= size)
        {
            return (current);
        }
        current = current->next;
    }
    return (FT_NULL);
}

static t_block *request_space(size_t size)
{
    void *addr;
    t_block *block;

    if (size >= FT_MMAP_THRESHOLD) {
        size_t total = sizeof(t_block) + size;

        asm volatile(
            "mov $9, %%rax\n"     // syscall number for mmap
            "mov $0, %%rdi\n"     // addr = NULL
            "mov %1, %%rsi\n"     // length
            "mov $0x22, %%rdx\n"  // PROT_READ | PROT_WRITE
            "mov $0x21, %%r10\n"  // MAP_PRIVATE | MAP_ANONYMOUS
            "mov $-1, %%r8\n"     // fd = -1
            "mov $0, %%r9\n"      // offset = 0
            "syscall\n"
            "mov %%rax, %0\n"
            : "=r"(addr)
            : "r"(total)
            : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9"
        );

        if (addr == (void *)-1)
        {
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
    asm volatile(
        "mov $12, %%rax\n"
        "syscall\n"
        "mov %%rax, %0\n"
        : "=r"(old_brk)
        :
        : "rax"
    );

    void *new_brk = (char *)old_brk + sizeof(t_block) + size;

    void *result;
    asm volatile(
        "mov $12, %%rax\n"
        "mov %1, %%rdi\n"
        "syscall\n"
        "mov %%rax, %0\n"
        : "=r"(result)
        : "r"(new_brk)
        : "rax", "rdi"
    );
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

void *ft_malloc(size_t size) {
    if (size == 0) return (FT_NULL);

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

void ft_free(void *ptr)
{
    if (!ptr) return;
    t_block *block = ((t_block *)ptr) - 1;
    if (block->is_mmap)
    {
        // syscall: munmap
        asm volatile (
            "mov $11, %%rax\n"       // munmap
            "mov %0, %%rdi\n"
            "mov %1, %%rsi\n"
            "syscall\n"
            :
            : "r"(block), "r"(block->size + sizeof(t_block))
            : "rax", "rdi", "rsi"
        );
        return;
    }
    block->free = 1;
    coalesce(block);
}

// Custom memset implementation
void *ft_memset(void *ptr, int value, size_t num) {
    unsigned char *p = ptr;  // Treat the pointer as a byte pointer
    while (num--) {
        *p = (unsigned char)value;  // Set each byte to the given value
        p++;  // Move to the next byte
    }
    return (ptr);  // Return the original pointer
}

// Custom memcpy implementation
void *ft_memcpy(void *dest, const void *src, size_t num) {
    unsigned char *d = dest;  // Destination pointer
    const unsigned char *s = src;  // Source pointer
    while (num--) {
        *d = *s;  // Copy each byte from source to destination
        d++;  // Move to the next byte in the destination
        s++;  // Move to the next byte in the source
    }
    return (dest);  // Return the destination pointer
}


// Custom calloc implementation
void *ft_calloc(size_t nmemb, size_t size) {
    size_t total_size = nmemb * size; // Calculate total size required
    void *ptr = ft_malloc(total_size); // Allocate memory using malloc
    if (ptr) ft_memset(ptr, 0, total_size); // Initialize the allocated memory to 0
    return (ptr);
}

// Custom realloc implementation
void *ft_realloc(void *ptr, size_t size) {
    if (!ptr) return ft_malloc(size); // If the pointer is NULL, behave like malloc()
    if (size == 0) 
    { 
        ft_free(ptr);
        return (FT_NULL);
    } // If size is 0, free the memory and return NULL

    t_block *block = (t_block*)ptr - 1;
    if (block->size >= size) return (ptr); // If the current block is already large enough, return the same pointer

    // If the block cannot be expanded, allocate a new block with the requested size
    void *new_ptr = ft_malloc(size);
    if (!new_ptr) return (FT_NULL); // If malloc fails, return NULL

    // Copy the data from the old block to the new block
    ft_memcpy(new_ptr, ptr, block->size);
    ft_free(ptr); // Free the old block
    return (new_ptr); // Return the new block
}

void	ft_reverse(char *str, int length)
{
	int		start;
	int		end;
	char	temp;

	start = 0;
	end = length - 1;
	while (start < end)
	{
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

int	ft_itoa_str(int num, char *str)
{
	int	i;
	int	is_negative;

	i = 0;
	is_negative = 0;
	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}
	if (num == 0)
		str[i++] = '0';
	while (num > 0)
	{
		str[i++] = (num % 10) + '0';
		num /= 10;
	}
	if (is_negative)
		str[i++] = '-';
	str[i] = '\0';
	ft_reverse(str, i);
	return (i);
}

void	ft_map(int *arr, size_t size, int (*func)(int))
{
	size_t	i;
	int		result;
	char	output[20];
	int		length;

	i = 0;
	while (i < size)
	{
		result = func(arr[i]);
		length = ft_itoa_str(result, output);
		ft_write(1, output, length);
		ft_write(1, "\n", 1);
		i++;
	}
}

ssize_t ft_getline(char **lineptr, size_t *n, int fd)
{
	if (!lineptr || !n)
		return (-1);

	if (*lineptr == FT_NULL || *n == 0)
	{
		*n = 128;
		*lineptr = (char *)ft_malloc(*n);
		if (!*lineptr)
			return (-1);
	}

	size_t i = 0;
	char c;
	ssize_t bytes_read;

	while (1)
	{
		bytes_read = ft_read(fd, &c, 1);
		if (bytes_read <= 0)
			break;

		// Reallocate if buffer is not sufficient
		if (i >= *n - 1)
		{
			size_t new_size = *n * 2;
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

//is used on ft_perror start
int saved_errno; // or extern if defined elsewhere
static const char *current_lang = "en";

void	ft_set_language(const char *lang) {
	current_lang = lang;
}

//Function for each language
static const char	*msg_success(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Sucesso");
	return ("Success");
}

static const char	*msg_operation_not_permitted(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Operação nao permitida");
	return ("Operation not permitted");
}

static const char	*msg_no_such_file_or_directory(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Arquivo ou diretorio inexistente");
	return ("No such file or directory");
}

static const char	*msg_no_such_process(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Nao existe esse processo");
	return ("No such process");
}

static const char	*msg_interrupted_system_call(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Chamada de sistema interrompida");
	return ("Interrupted system call");
}

static const char	*msg_input_output_error(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Erro de entrada e saida");
	return ("Input/output error");
}

static const char	*msg_no_such_device_or_address(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Nao existe tal dispositivo ou endereco");
	return ("No such device or address");
}

static const char	*msg_argument_list_too_long(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Lista de argumentos muito longa");
	return ("Argument list too long");
}

static const char	*msg_exec_format_error(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Erro de Formato do exec");
	return ("Exec format error");
}

static const char	*msg_bad_file_descriptor(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Erro de descricao do arquivo");
	return ("Bad file descriptor");
}

static const char	*msg_no_child_processes(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Nenhum processo filho");
	return ("No child processes");
}

static const char	*msg_out_of_memory(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Sem memoria");
	return ("Out of memory");
}

static const char	*msg_permission_denied(void) {
	if (ft_strcmp(current_lang, "pt") == 0)
		return ("Permissao negada");
	return ("Permission denied");
}

static const t_error g_errors[] = {
    { 0,  msg_success },
    { 1,  msg_operation_not_permitted },
	{ 2,  msg_no_such_file_or_directory },
	{ 3,  msg_no_such_process },
	{ 4,  msg_interrupted_system_call },
	{ 5,  msg_input_output_error },
	{ 6,  msg_no_such_device_or_address },
    { 7,  msg_argument_list_too_long },
    { 8,  msg_exec_format_error },
    { 9,  msg_bad_file_descriptor },
    { 10, msg_no_child_processes },
	{ 12, msg_out_of_memory },
	{ 13, msg_permission_denied }
};

//is used on ft_perror end
//binary search

static const char	*ft_get_error_message(int err) {
	size_t left = 0;
	size_t right = sizeof(g_errors) / sizeof(t_error) - 1;

	while (left <= right) {
		size_t mid = left + (right - left) / 2;

		if (g_errors[mid].code == err)
			return g_errors[mid].get_message();
		else if (g_errors[mid].code < err)
			left = mid + 1;
		else
			right = mid - 1;
	}
	return ((ft_strcmp(current_lang, "pt") == 0)
		? "Erro desconhecido"
		: "Unknown error");
}

void ft_perror(const char *str) {
    if (str && *str)
        ft_printf("%s: ", str);
    ft_printf("%s\n", ft_get_error_message(saved_errno));
}

const char *ft_strerror(int err)
{
    return (ft_get_error_message(err));
}

void ft_list_all_errors(void)
{
    size_t len = sizeof(g_errors) / sizeof(t_error);
    for (size_t code = 0; code <= len; code++)
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
                while (*str == ' ') str++; // Skip leading spaces
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

char	*ft_utoa_base(unsigned int value, char *buf, int base) {
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

char	*ft_itoa_base(int value, char *buf, int base) {
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
                while (*s) *p++ = *s++;
            } else if (*fmt == 'd') {
                int val = va_arg(args, int);
                char tmp[20];
                ft_itoa_base(val, tmp, 10);  // implement this if not done
                const char *s = tmp;
                while (*s) *p++ = *s++;
            } else if (*fmt == 'x') {
                unsigned int val = va_arg(args, unsigned int);
                char tmp[20];
                ft_utoa_base(val, tmp, 16);  // implement this too
                const char *s = tmp;
                while (*s) *p++ = *s++;
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
