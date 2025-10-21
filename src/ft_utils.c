#include "../include/ft_maki.h"

void ft_bzero(void *s, ft_size_t n) { ft_memset(s, 0, n); }
void *ft_memccpy(void *dest, const void *src, int c, ft_size_t n) {
  ft_size_t i;

  i = 0;
  while (i < n) {
    *((unsigned char *)dest + i) = *((unsigned char *)src + i);
    if (*((unsigned char *)src + i) == (unsigned char)c) {
      return (dest + i + 1);
      break;
    }
    i++;
  }
  return (FT_NULL);
}

void *ft_memmove(void *dest, const void *src, ft_size_t n) {
  ft_size_t i;

  i = 0;
  if (dest == src || !n)
    return (dest);
  if (dest < src) {
    while (i < n) {
      *((char *)dest + i) = *((char *)src + i);
      i++;
    }
  } else {
    while (n > 0) {
      *((char *)dest + n - 1) = *((char *)src + n - 1);
      n--;
    }
  }
  return (dest);
}

void *ft_memchr(const void *s, int c, ft_size_t n) {
  ft_size_t i;

  i = 0;
  while (i < n) {
    if (*((unsigned char *)s + i) == (unsigned char)c)
      return ((void *)s + i);
    i++;
  }
  return (FT_NULL);
}

int ft_memcmp(const void *s1, const void *s2, ft_size_t n) {
  ft_size_t i;

  i = 0;
  while (i < n) {
    if (*((unsigned char *)s1 + i) == *((unsigned char *)s2 + i))
      i++;
    else
      return ((*((unsigned char *)s1 + i)) - *((unsigned char *)s2 + i));
  }
  return (0);
}

ft_size_t ft_strlcpy(char *dst, const char *src, ft_size_t size) {
  ft_size_t count;

  if (!dst || !src)
    return (0);
  count = 0;
  if (size == 0)
    return (ft_strlen(src));
  while (count < size - 1 && src[count]) {
    dst[count] = src[count];
    count++;
  }
  dst[count] = '\0';
  return (ft_strlen(src));
}

ft_size_t ft_strlcat(char *dest, const char *src, ft_size_t size) {
  ft_size_t i;
  ft_size_t j;
  ft_size_t size_dest;
  ft_size_t size_src;

  size_dest = ft_strlen(dest);
  size_src = ft_strlen(src);
  if (size == 0)
    return (size_src);
  i = 0;
  if (size >= size_dest) {
    while (dest[i] != '\0')
      i++;
    j = 0;
    while (src[j] != '\0' && i + 1 < size)
      dest[i++] = src[j++];
    dest[i] = '\0';
  }
  if (size_dest > size)
    return (size_src + size);
  else
    return (size_dest + size_src);
}

char *ft_strchr(const char *s, int c) {
  int i;

  i = 0;
  while (s[i] != '\0') {
    if (s[i] == c)
      return ((char *)s + i);
    i++;
  }
  if (c == '\0')
    return ((char *)s + i);
  return (0);
}

char *ft_strrchr(const char *s, int c) {
  int i;

  i = 0;
  while (s[i] != '\0')
    i++;
  if (c == 0)
    return ((char *)s + i);
  i--;
  while (i >= 0 && s[i] != '\0') {
    if (s[i] == c)
      return ((char *)s + i);
    i--;
  }
  return (0);
}

char *ft_strnstr(const char *big, const char *little, ft_size_t len) {
  ft_size_t little_len;

  if (*little == '\0')
    return ((char *)big);
  little_len = ft_strlen((char *)little);
  while (*big != '\0' && len-- >= little_len) {
    if (*big == *little && ft_memcmp(big, little, little_len) == 0)
      return ((char *)big);
    big++;
  }
  return (FT_NULL);
}

int ft_strncmp(const char *s1, const char *s2, ft_size_t n) {
  ft_size_t i;

  i = 0;
  while (i < n && s1[i] != '\0' && s2[i] != '\0') {
    if ((unsigned char)s1[i] != (unsigned char)s2[i])
      return ((unsigned char)s1[i] - (unsigned char)s2[i]);
    i++;
  }
  if (i != n)
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
  return (0);
}

unsigned long ft_strcspn(const char *__s, const char *__reject) {
    unsigned char seen[256] = {0};
    const unsigned char *s = (const unsigned char *)__s;
    const unsigned char *r = (const unsigned char *)__reject;

    /* mark reject bytes */
    while (*r) {
        seen[*r++] = 1;
    }

    /* scan s for first byte that is in reject */
    unsigned long idx = 0;
    while (s[idx]) {
        if (seen[s[idx]]) {
            return (idx);
        }
        idx++;
    }
    return (idx); /* no bytes from reject found in s */
}

int ft_isalpha(int c) {
  if (c >= 65 && c <= 90)
    return (c);
  else if (c >= 97 && c <= 122)
    return (c);
  return (0);
}

int ft_isdigit(int c) {
  if (c >= 48 && c <= 57)
    return (c);
  return (0);
}

int ft_isalnum(int c) {
  if (c >= 48 && c <= 57)
    return (c);
  else if (c >= 65 && c <= 90)
    return (c);
  else if (c >= 97 && c <= 122)
    return (c);
  return (0);
}

int ft_isascii(int c) {
  if (c >= 0 && c <= 127)
    return (1);
  return (0);
}

int ft_isprint(int c) {
  if (c >= 32 && c <= 126)
    return (c);
  return (0);
}

int ft_toupper(int c) {
  if (c >= 97 && c <= 122)
    return (c - 32);
  return (c);
}

int ft_tolower(int c) {
  if (c >= 65 && c <= 90)
    return (c + 32);
  return (c);
}

int ft_isspace(int c) { return (c == ' ' || (c >= '\t' && c <= '\r')); }

int ft_isxdigit(int c) {
  // Check if c is a decimal digit
  if (c >= '0' && c <= '9') {
    return (1);
  }
  // Check if c is a lowercase hexadecimal digit
  if (c >= 'a' && c <= 'f') {
    return (1);
  }
  // Check if c is an uppercase hexadecimal digit
  if (c >= 'A' && c <= 'F') {
    return (1);
  }
  // Otherwise, not a hex digit
  return (0);
}

/* -----------------------------
   isnan implementations
   ----------------------------- */

static inline ft_uint64_t double_to_u64(double x) {
  ft_uint64_t bits;
  ft_memcpy(&bits, &x, sizeof(bits));
  return (bits);
}

static inline ft_uint32_t float_to_u32(float x) {
  ft_uint32_t bits;
  ft_memcpy(&bits, &x, sizeof(bits));
  return (bits);
}

static inline int isnan_double_custom(double x) {
  ft_uint64_t bits = double_to_u64(x);
  const ft_uint64_t EXP_MASK = 0x7FF0000000000000ULL;
  const ft_uint64_t FRAC_MASK = 0x000FFFFFFFFFFFFFULL;
  return (((bits & EXP_MASK) == EXP_MASK) && ((bits & FRAC_MASK) != 0ULL));
}

static inline int isnan_float_custom(float x) {
  ft_uint32_t bits = float_to_u32(x);
  const ft_uint32_t EXP_MASK = 0x7F800000U;
  const ft_uint32_t FRAC_MASK = 0x007FFFFFU;
  return (((bits & EXP_MASK) == EXP_MASK) && ((bits & FRAC_MASK) != 0U));
}

int ft_isnanf(float x) { return (isnan_float_custom(x)); }

int ft_isnan(double x) { return (isnan_double_custom(x)); }

int ft_isinf(double x) {
  double y = x - x;
  // se x-x dá NaN mas x não é NaN -> é inf
  return ((y != y && !(x != x)));
}

char *ft_substr(char const *s, unsigned int start, ft_size_t len) {
  ft_size_t i;
  char *buff;

  if (!s)
    return (FT_NULL);
  if (!(buff = (char *)ft_malloc(sizeof(char) * (len + 1))))
    return (FT_NULL);
  i = 0;
  if (start < ft_strlen(s)) {
    while (s[start + i] && i < len) {
      buff[i] = s[start + i];
      i++;
    }
  }
  buff[i] = '\0';
  return (buff);
}

char *ft_strjoin(char const *s1, char const *s2) {
  ft_size_t i;
  ft_size_t j;
  char *buff;

  if (!s1 || !s2)
    return (FT_NULL);
  if (!(buff = (char *)ft_malloc(sizeof(char) *
                                 (ft_strlen(s1) + ft_strlen(s2) + 1))))
    return (FT_NULL);
  i = 0;
  j = 0;
  while (i < ft_strlen(s1)) {
    buff[i] = s1[i];
    i++;
  }
  while (j < ft_strlen(s2)) {
    buff[i] = s2[j];
    i++;
    j++;
  }
  buff[i] = '\0';
  return (buff);
}

char *ft_strtrim(char const *s1, char const *set) {
  ft_size_t len_s1;
  char *buff;

  if (!s1 || !set)
    return (FT_NULL);
  while (ft_strchr(set, *s1) && *s1 != '\0')
    s1++;
  len_s1 = ft_strlen(s1);
  while (ft_strchr(set, *(s1 + len_s1)) && len_s1 > 0)
    len_s1--;
  if (!(buff = ft_substr(s1, 0, len_s1 + 1)))
    return (FT_NULL);
  return (buff);
}

static ft_size_t count_word(char const *s, char c) {
  ft_size_t i;
  ft_size_t count;

  i = 0;
  count = 0;
  while (s[i]) {
    if (s[i] != c) {
      count++;
      while (s[i] != c && s[i] != '\0')
        i++;
    } else
      i++;
  }
  return (count);
}

static char **free_buff(char **buff, ft_size_t j) {
  while (j-- > 0) {
    ft_free(buff[j]);
    buff[j] = FT_NULL;
  }
  ft_free(buff);
  buff = FT_NULL;
  return (FT_NULL);
}

char **ft_split(char const *s, char c) {
  char **buff;
  ft_size_t i;
  ft_size_t j;
  ft_size_t word_num;

  if (!s ||
      !(buff = (char **)ft_malloc(sizeof(char *) * (count_word(s, c) + 1))))
    return (FT_NULL);
  j = 0;
  word_num = count_word(s, c);
  while (*s && j < word_num) {
    while (*s == c && *s != '\0')
      s++;
    i = 0;
    while (*(s + i) != '\0' && *(s + i) != c)
      i++;
    if (!(buff[j++] = ft_substr(s, 0, i)))
      return (free_buff(buff, j - 1));
    s = s + i;
  }
  buff[j] = 0;
  return (buff);
}

void ft_split_free(char **arr) {
  if (!arr)
    return;
  for (int i = 0; arr[i]; i++)
    ft_free(arr[i]);
  ft_free(arr);
}

static int count_digits(int n) {
  int count = 1;
  unsigned int n_abs = (n < 0) ? -(unsigned int)n : (unsigned int)n;

  while (n_abs >= 10) {
    n_abs /= 10;
    count++;
  }
  return (count);
}

char *ft_itoa(int n) {
  int digits;
  int len;
  char *buff;
  unsigned int n_abs;

  digits = count_digits(n);
  len = digits + (n < 0); // +1 if negative (for the '-')
  buff = ft_malloc(sizeof(char) * (len + 1));
  if (!buff)
    return (FT_NULL);

  buff[len] = '\0';
  n_abs = (n < 0) ? -(unsigned int)n : (unsigned int)n;

  while (digits-- > 0) {
    buff[--len] = (n_abs % 10) + '0';
    n_abs /= 10;
  }

  if (n < 0)
    buff[0] = '-';

  return (buff);
}

char *ft_strmapi(char const *s, char (*f)(unsigned int, char)) {
  ft_size_t i;
  char *buff;

  i = 0;
  if (!s || !f ||
      !(buff = (char *)ft_malloc(sizeof(char) * (ft_strlen(s) + 1))))
    return (FT_NULL);
  while (i < ft_strlen(s)) {
    buff[i] = f(i, s[i]);
    i++;
  }
  buff[i] = '\0';
  return (buff);
}

void ft_putchar_fd(char c, int fd) { ft_write(fd, &c, 1); }

void ft_putstr_fd(char *s, int fd) {
  if (!s)
    return;
  ft_write(fd, s, ft_strlen(s));
}

void ft_putendl_fd(char *s, int fd) {
  if (!s)
    return;
  ft_write(fd, s, ft_strlen(s));
  ft_write(fd, "\n", 1);
}

void ft_putnbr_fd(int n, int fd) {
  unsigned int n2;

  if (n < 0)
    ft_putchar_fd('-', fd);
  n2 = n < 0 ? (unsigned int)(n * (-1)) : (unsigned int)n;
  if (n2 >= 10)
    ft_putnbr_fd(n2 / 10, fd);
  ft_putchar_fd(n2 % 10 + '0', fd);
}

t_list *ft_lstnew(void *content) {
  t_list *head;

  if (!(head = (t_list *)ft_malloc(sizeof(t_list))))
    return (FT_NULL);
  head->content = content;
  head->next = FT_NULL;
  return (head);
}

void ft_lstadd_front(t_list **lst, t_list *lstnew) {
  if (!*lst) {
    *lst = lstnew;
    return;
  }
  if (lstnew) {
    lstnew->next = *lst;
    *lst = lstnew;
  }
}

int ft_lstsize(t_list *lst) {
  t_list *curr;
  int count;

  count = 0;
  curr = lst;
  while (curr) {
    curr = curr->next;
    count++;
  }
  return (count);
}

t_list *ft_lstlast(t_list *lst) {
  t_list *curr;
  int count;

  count = ft_lstsize(lst);
  curr = lst;
  while (count > 1) {
    curr = curr->next;
    count--;
  }
  return (curr);
}

void ft_lstadd_back(t_list **lst, t_list *lstnew) {
  t_list *last;

  if (*lst == FT_NULL) {
    *lst = lstnew;
    return;
  }
  if (lstnew) {
    last = ft_lstlast(*lst);
    last->next = lstnew;
  }
}

void ft_lstdelone(t_list *lst, void (*del)(void *)) {
  if (!lst || !del)
    return;
  del(lst->content);
  ft_free(lst);
}

void ft_lstclear(t_list **lst, void (*del)(void *)) {
  t_list *temp;

  if (!lst)
    return;
  while (*lst) {
    del((*lst)->content);
    temp = *lst;
    *lst = (*lst)->next;
    ft_free(temp);
  }
  *lst = FT_NULL;
}

void ft_lstiter(t_list *lst, void (*f)(void *)) {
  if (lst && f) {
    while (lst != FT_NULL) {
      f(lst->content);
      lst = lst->next;
    }
  }
}

t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *)) {
  t_list *newelem;
  t_list *newlist;

  if (!lst || !f)
    return (FT_NULL);
  newlist = FT_NULL;
  while (lst) {
    if (!(newelem = ft_lstnew(f(lst->content)))) {
      ft_lstclear(&newlist, del);
      return (FT_NULL);
    }
    ft_lstadd_back(&newlist, newelem);
    lst = lst->next;
  }
  return (newlist);
}

t_list *ft_lstmapi(t_list *lst, void *(*f)(void *, int), void (*del)(void *)) {
  t_list *newlist = FT_NULL;
  t_list *newelem;
  int index = 0;

  if (!lst || !f)
    return (FT_NULL);
  while (lst) {
    newelem = ft_lstnew(f(lst->content, index));
    if (!newelem) {
      ft_lstclear(&newlist, del);
      return (FT_NULL);
    }
    ft_lstadd_back(&newlist, newelem);
    lst = lst->next;
    index++;
  }
  return (newlist);
}

void ft_print_list(t_list *lst) {
  while (lst) {
    ft_printf("%s\n", (char *)lst->content);
    lst = lst->next;
  }
}