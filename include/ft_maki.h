#ifndef FT_MAKI_H
#define FT_MAKI_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h> //ssize_t
#include <unistd.h>

#define FT_COLOR_RESET "\033[0m"
#define FT_COLOR_GREEN "\033[32m"
#define FT_COLOR_RED "\033[31m"
#define FT_COLOR_GRAY "\033[90m"
#define FT_COLOR_YELLOW "\033[33m"
#define FT_COLOR_CYAN "\033[36m"
#define FT_COLOR_BLUE "\033[34m"
#define FT_COLOR_LIGHT_GREEN "\033[92m"
#define FT_COLOR_MOGENTA "\033[35m"
#define FT_COLOR_ORANGE "\033[38;2;255;165;0m"
#define FT_BLOCK_SIZE sizeof(t_block)
#define FT_MMAP_THRESHOLD (128 * 1024) // 128 KB
#define FT_ALIGNMENT 16
#define ft_offsetof(type, member) ((size_t)&(((type *)0)->member))
#define FT_NULL ((void *)0)

// Is used for RAM magenement
typedef struct s_block {
  size_t size;
  int free;
  int is_mmap;
  struct s_block *next;
  struct s_block *prev;
} t_block;

// To implement my own bool control
typedef enum e_bool { FALSE = 0, TRUE = 1 } t_bool;

// Flags struct  for ft_printf
typedef struct s_flags {
  int width;
  int precision;
  bool precision_specified;
  bool left_align;
  bool zero_padding;
  int length;
} t_flags;

// linked list struct
typedef struct s_list {
  void *content;
  struct s_list *next;
} t_list;

// struct to ft_perror
typedef struct s_error {
  int code;
  const char *(*get_message)(void); // support to i18n
} t_error;

// ft_maki.c start
// make syscall
ssize_t ft_write(int fd, const void *buf, size_t len);
void ft_exit(long code);
ssize_t ft_read(int fd, void *buf, size_t count);
int ft_open(const char *pathname, int flags, int mode);
int ft_close(int fd);

char *ft_strcat(char *s1, const char *s2);
char *ft_strcpy(char *dst, const char *src);
char *ft_strncpy(char *dest, const char *src, unsigned int n);
size_t ft_strlen(const char *s);
void ft_putchar(char c);
void ft_putstr(const char *str);
int ft_atoi(const char *nptr);
int ft_atoi_base(const char *str, int base);
int ft_atoi_auto(const char *str);
double ft_atof(const char *s);

char *ft_strdup(const char *s);
void ft_reverse(char *str, int length);
int ft_itoa_str(int num, char *str);
int ft_strcmp(const char *s1, const char *s2);
char *ft_strtok(char *str, const char *delim);
char *ft_strpbrk(const char *s, const char *accept);
ssize_t ft_getline(char **lineptr, size_t *n, int fd);
// const char			*ft_get_error_message(int err);
void ft_set_language(const char *lang); // "en", "pt", etc.
void ft_perror(const char *str);
const char *ft_strerror(int err);
void ft_list_all_errors(void);
int ft_sscanf(const char *str, const char *format, ...);
char *ft_utoa_base(unsigned int value, char *buf, int base);
char *ft_itoa_base(int value, char *buf, int base);
int ft_sprintf(char *buf, const char *fmt, ...);

// RAM manager
void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_memset(void *ptr, int value, size_t num);
void *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_calloc(size_t nmemb, size_t size);
void *ft_realloc(void *ptr, size_t size);

// Internal helper functions used exclusively by ft_malloc for memory
// management. static size_t align(size_t size); static t_block
// *find_free_block(size_t size); static t_block *request_space(size_t size);
// static void coalesce(t_block *block);

// ft_maki.c end

// All these functions is for ft_printf start_comment
// BUFFER MANAGEMENT
static inline void ft_buf_set_fd(int fd);
static inline void ft_buf_flush(void);
static inline void ft_buf_putchar(char c);
static inline void ft_buf_putnchar(char c, int n);
static inline void ft_buf_putstr(const char *s, int len);

// PRINTF CORE UTILITIES
static inline int ft_printf_pad(char c, int count);
static inline int ft_printf_numlen(long long n);
static inline int ft_printf_unumlen(unsigned long n);
static inline int ft_printf_max(int a, int b);
static inline int ft_printf_putchar(char c);
static inline int ft_printf_putnchar(char c, int n);
static inline int ft_printf_putchar_base(char c, int width, int left);
static inline int ft_printf_putstr(char *s, t_flags flags);
static inline int ft_printf_putnbr(long long n, t_flags flags);
static inline int ft_printf_putunbr(unsigned int n, t_flags flags);
static inline int ft_printf_puthex(unsigned long n, t_flags flags,
                                   int uppercase);
static inline int ft_printf_putptr(void *ptr, t_flags flags);
static inline long long ft_printf_power10(int n);
static inline int ft_printf_putfloat(double n, t_flags flags);
static inline int ft_printf_putpercent(t_flags flags);
static inline t_flags ft_printf_parse_flags(const char **format);
static inline int ft_printf_dispatch(char spec, va_list *args, t_flags flags);

// NON-INLINED FUNCTIONS (EXPORTED SYMBOLS)
int ft_vdprintf(int fd, const char *format, va_list *args);
int ft_dprintf(int fd, const char *format, ...);
int ft_fprintf(FILE *stream, const char *format, ...);
int ft_printf(const char *format, ...);
// All these functions is for ft_printf end_comment

// ft_utils.c start
void ft_bzero(void *s, size_t n);
void *ft_memccpy(void *dest, const void *src, int c, size_t n);
void *ft_memmove(void *dest, const void *src, size_t n);
void *ft_memchr(const void *s, int c, size_t n);
int ft_memcmp(const void *s1, const void *s2, size_t n);
size_t ft_strlcpy(char *dst, const char *src, size_t size);
size_t ft_strlcat(char *dst, const char *src, size_t size);
char *ft_strchr(const char *s, int c);
char *ft_strrchr(const char *s, int c);
char *ft_strnstr(const char *big, const char *little, size_t len);
int ft_strncmp(const char *s1, const char *s2, size_t n);

int ft_isalpha(int c);
int ft_isdigit(int c);
int ft_isalnum(int c);
int ft_isascii(int c);
int ft_isprint(int c);
int ft_toupper(int c);
int ft_tolower(int c);
int ft_isspace(int c);
int ft_isxdigit(int c);
int ft_isnanf(float x);
int ft_isnan(double x);
int ft_isinf(double x);

char *ft_substr(char const *s, unsigned int start, size_t len);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_strtrim(char const *s1, char const *set);
char **ft_split(char const *s, char c);
void ft_split_free(char **arr);
char *ft_itoa(int n);
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));

void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
void ft_putendl_fd(char *s, int fd);
void ft_putnbr_fd(int n, int fd);

t_list *ft_lstnew(void *content);
void ft_lstadd_front(t_list **lst, t_list *lstnew);
int ft_lstsize(t_list *lst);
t_list *ft_lstlast(t_list *lst);
void ft_lstadd_back(t_list **lst, t_list *lstnew);
void ft_lstdelone(t_list *lst, void (*del)(void *));
void ft_lstclear(t_list **lst, void (*del)(void *));
void ft_lstiter(t_list *lst, void (*f)(void *));
t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list *ft_lstmapi(t_list *lst, void *(*f)(void *, int), void (*del)(void *));
void ft_print_list(t_list *lst);
// ft_utils.c end

// ft_lst_wrappers.c start
// Wrappers to use ft_function in ft_lst_functions
void *ft_lst_to_upper_wrapper(void *data);
void *ft_lst_to_upper_indice_wrapper(void *content, int i);
void *ft_lst_tolower_wrapper(void *data);
void *ft_lst_strtrim_wrapper(void *data);
void *ft_lst_strdup_wrapper(void *data);
void ft_lst_del(void *data);
void ft_lst_print_int_wrapper(void *data);
void ft_lst_print_str_wrapper(void *data);
void ft_lst_print_float_wrapper(void *data);
void ft_lst_print_double_wrapper(void *data);
// ft_lst_wrappers.c end

// ft_fflush.c start

#define ft_size_t long unsigned int
#define ft_ssize_t long int
#define FT_BUFSIZ 1024

#define FT_MODE_READ 0
#define FT_MODE_WRITE 1

#define FT_IOFBF 0 // Full buffering
#define FT_IOLBF 1 // Line buffering
#define FT_IONBF 2 // No buffering

/* Standard file descriptors.  */
#define FT_STDIN_FILENO 0  /* Standard input.  */
#define FT_STDOUT_FILENO 1 /* Standard output.  */
#define FT_STDERR_FILENO 2 /* Standard error output.  */
#define FT_EOF (-1)

#define FT_O_RDONLY 00
#define FT_O_WRONLY 01
#define FT_O_RDWR 02
#define FT_O_CREAT 0100  /* Not fcntl.  */
#define FT_O_TRUNC 01000 /* Not fcntl.  */
#define FT_O_APPEND 02000

typedef struct T_FT_FILE {
  int fd;
  char buffer[FT_BUFSIZ];
  ft_size_t buf_pos;
  ft_size_t buf_len;
  int mode;     // FT_MODE_READ or FT_MODE_WRITE
  int buf_mode; // FT_IOFBF, FT_IOLBF, FT_IONBF
  int error;
  int eof;
  struct T_FT_FILE *next;
} T_FT_FILE;

extern T_FT_FILE *ft_stdout;
extern T_FT_FILE *ft_stdin;

int ft_ferror(T_FT_FILE *fp);
int ft_feof(T_FT_FILE *fp);
void ft_clearerr(T_FT_FILE *fp);
int ft_setvbuf(T_FT_FILE *fp, int mode);
T_FT_FILE *ft_fopen(const char *filename, const char *mode);
int ft_fputc(char c, T_FT_FILE *fp);
int ft_fgetc(T_FT_FILE *fp);
int ft_fflush(T_FT_FILE *fp);
int ft_fclose(T_FT_FILE *fp);

// ft_fflush.c end
#endif
