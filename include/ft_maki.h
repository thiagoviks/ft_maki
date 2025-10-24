#ifndef FT_MAKI_H
#define FT_MAKI_H

#include <stdarg.h> // va_list

/*
** ============================================================================
** SECTION 1: BASIC TYPE DEFINITIONS
** ============================================================================
*/

#define FT_NULL ((void *)0)
typedef long int ft_off_t;

/* Size and pointer types */
typedef long unsigned int ft_size_t;
typedef long int ft_ssize_t;
typedef long long ft_time_t;

/* Signed integer types */
typedef signed char ft_int8_t;
typedef signed short int ft_int16_t;
typedef signed int ft_int32_t;
typedef signed long int ft_int64_t;
typedef signed long int ft_intptr_t;

/* Unsigned integer types */
typedef unsigned char ft_uint8_t;
typedef unsigned short int ft_uint16_t;
typedef unsigned int ft_uint32_t;
typedef unsigned long int ft_uint64_t;
typedef unsigned long int ft_uintptr_t;

/* Legacy unsigned types */
typedef unsigned char ft_u_char;
typedef unsigned short int ft_u_short;
typedef unsigned int ft_u_int;
typedef unsigned long int ft_u_long;

/* Boolean type */
typedef enum e_bool { ft_false = 0, ft_true = 1 } t_bool;

/* Utility macros */
#define ft_offsetof(type, member) ((ft_size_t) & (((type *)0)->member))

/*
** ============================================================================
** SECTION 2: ERROR HANDLING
** ============================================================================
*/

/* Error codes (POSIX compatible) */
#define FT_EPERM 1    /* Operation not permitted */
#define FT_ENOENT 2   /* No such file or directory */
#define FT_ESRCH 3    /* No such process */
#define FT_EINTR 4    /* Interrupted system call */
#define FT_EIO 5      /* I/O error */
#define FT_ENXIO 6    /* No such device or address */
#define FT_E2BIG 7    /* Argument list too long */
#define FT_ENOEXEC 8  /* Exec format error */
#define FT_EBADF 9    /* Bad file number */
#define FT_ECHILD 10  /* No child processes */
#define FT_EAGAIN 11  /* Try again */
#define FT_ENOMEM 12  /* Out of memory */
#define FT_EACCES 13  /* Permission denied */
#define FT_EFAULT 14  /* Bad address */
#define FT_ENOTBLK 15 /* Block device required */
#define FT_EBUSY 16   /* Device or resource busy */
#define FT_EEXIST 17  /* File exists */
#define FT_EXDEV 18   /* Cross-device link */
#define FT_ENODEV 19  /* No such device */
#define FT_ENOTDIR 20 /* Not a directory */
#define FT_EISDIR 21  /* Is a directory */
#define FT_EINVAL 22  /* Invalid argument */
#define FT_ENFILE 23  /* File table overflow */
#define FT_EMFILE 24  /* Too many open files */
#define FT_ENOTTY 25  /* Not a typewriter */
#define FT_ETXTBSY 26 /* Text file busy */
#define FT_EFBIG 27   /* File too large */
#define FT_ENOSPC 28  /* No space left on device */
#define FT_ESPIPE 29  /* Illegal seek */
#define FT_EROFS 30   /* Read-only file system */
#define FT_EMLINK 31  /* Too many links */
#define FT_EPIPE 32   /* Broken pipe */
#define FT_EDOM 33    /* Math argument out of domain */
#define FT_ERANGE 34  /* Math result not representable */

/* Global error variable */
extern int ft_errno;

/* Error handling structures and functions */
typedef struct s_error {
  int code;
  const char *(*get_message)(void); // i18n support
} t_error;

void ft_set_language(const char *lang);
void ft_perror(const char *str);
const char *ft_strerror(int err);
void ft_list_all_errors(void);

/*
** ============================================================================
** SECTION 3: SYSCALL WRAPPERS
** ============================================================================
*/

/* Syscall numbers */
#define FT_SYS_IOCTL 16
#define FT_SYS_GETTIMEOFDAY 96
#define FT_SYS_time 201

/* ioctl constants */
#define FT_TCGETS 0x5401

/* Time structures */
typedef struct s_timeval {
  long int ft_tv_sec;
  long int ft_tv_usec;
} t_timeval;

/* Low-level syscall wrapper */
long ft_syscall(long number, ...);

/* Syscall functions */
int ft_gettimeofday(struct s_timeval *tv, void *tz);
int ft_isatty(int fd);
ft_ssize_t ft_write(int fd, const void *buf, ft_size_t len);
ft_ssize_t ft_read(int fd, void *buf, ft_size_t count);
int ft_open(const char *pathname, int flags, int mode);
int ft_close(int fd);
void ft_exit(long code);
ft_time_t ft_time(ft_time_t *tloc);

/*
** ============================================================================
** SECTION 4: MEMORY MANAGEMENT
** ============================================================================
*/

/* Memory allocation constants */
#define FT_BLOCK_SIZE sizeof(t_block)
#define FT_MMAP_THRESHOLD (128 * 1024)
#define FT_ALIGNMENT 16

/* Memory block structure (internal use) */
typedef struct s_block {
  ft_size_t size;
  int free;
  int is_mmap;
  struct s_block *next;
  struct s_block *prev;
} t_block;

/* Memory allocation functions */
void *ft_malloc(ft_size_t size);
void ft_free(void *ptr);
void *ft_calloc(ft_size_t nmemb, ft_size_t size);
void *ft_realloc(void *ptr, ft_size_t size);

/* Memory manipulation functions */
void *ft_memset(void *ptr, int value, ft_size_t num);
void *ft_memcpy(void *dest, const void *src, ft_size_t n);
void *ft_memccpy(void *dest, const void *src, int c, ft_size_t n);
void *ft_memmove(void *dest, const void *src, ft_size_t n);
void *ft_memchr(const void *s, int c, ft_size_t n);
int ft_memcmp(const void *s1, const void *s2, ft_size_t n);
void ft_bzero(void *s, ft_size_t n);

/*
** ============================================================================
** SECTION 5: STRING MANIPULATION
** ============================================================================
*/

/* String length and basic operations */
ft_size_t ft_strlen(const char *s);
char *ft_strcpy(char *dst, const char *src);
char *ft_strncpy(char *dest, const char *src, unsigned int n);
char *ft_strcat(char *s1, const char *s2);
char *ft_strdup(const char *s);

/* String comparison and searching */
int ft_strcmp(const char *s1, const char *s2);
int ft_strncmp(const char *s1, const char *s2, ft_size_t n);
char *ft_strchr(const char *s, int c);
char *ft_strrchr(const char *s, int c);
char *ft_strnstr(const char *big, const char *little, ft_size_t len);
char *ft_strpbrk(const char *s, const char *accept);
unsigned long ft_strcspn(const char *__s, const char *__reject);

/* String copying with size limit */
ft_size_t ft_strlcpy(char *dst, const char *src, ft_size_t size);
ft_size_t ft_strlcat(char *dst, const char *src, ft_size_t size);

/* String tokenization and parsing */
char *ft_strtok(char *str, const char *delim);

/* String allocation and transformation */
char *ft_substr(char const *s, unsigned int start, ft_size_t len);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_strtrim(char const *s1, char const *set);
char **ft_split(char const *s, char c);
void ft_split_free(char **arr);
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));

/*
** ============================================================================
** SECTION 6: CHARACTER CLASSIFICATION AND CONVERSION
** ============================================================================
*/

/* Character type checking */
int ft_isalpha(int c);
int ft_isdigit(int c);
int ft_isalnum(int c);
int ft_isascii(int c);
int ft_isprint(int c);
int ft_isspace(int c);
int ft_isxdigit(int c);

/* Character conversion */
int ft_toupper(int c);
int ft_tolower(int c);

/*
** ============================================================================
** SECTION 7: NUMBER CONVERSION AND MATH UTILITIES
** ============================================================================
*/

/* String to number conversion */
int ft_atoi(const char *nptr);
int ft_atoi_base(const char *str, int base);
int ft_atoi_auto(const char *str);
double ft_atof(const char *s);

/* Number to string conversion */
char *ft_itoa(int n);
int ft_itoa_str(int num, char *str);
char *ft_itoa_base(int value, char *buf, int base);
char *ft_utoa_base(unsigned int value, char *buf, int base);

/* Math utilities */
int ft_isnanf(float x);
int ft_isnan(double x);
int ft_isinf(double x);

/* Random number generation */
void ft_srandom(unsigned int seed);
long ft_random(void);
int ft_rand_r(unsigned int *seedp);

/*
** ============================================================================
** SECTION 8: INPUT/OUTPUT - BASIC
** ============================================================================
*/

/* Standard file descriptors */
#define FT_STDIN_FILENO 0
#define FT_STDOUT_FILENO 1
#define FT_STDERR_FILENO 2
#define FT_EOF (-1)

/* File open flags */
#define FT_O_RDONLY 00
#define FT_O_WRONLY 01
#define FT_O_RDWR 02
#define FT_O_CREAT 0100
#define FT_O_TRUNC 01000
#define FT_O_APPEND 02000

/* Basic output functions */
void ft_putchar(char c);
void ft_putstr(const char *str);
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
void ft_putendl_fd(char *s, int fd);
void ft_putnbr_fd(int n, int fd);

/* Line input */
ft_ssize_t ft_getline(char **lineptr, ft_size_t *n, int fd);

/*
** ============================================================================
** SECTION 9: INPUT/OUTPUT - BUFFERED (FILE STREAMS)
** ============================================================================
*/

/* Buffer configuration */
#define FT_BUFSIZ 1024

/* Buffering modes */
#define FT_IOFBF 0 /* Full buffering */
#define FT_IOLBF 1 /* Line buffering */
#define FT_IONBF 2 /* No buffering */

/* Stream modes */
#define FT_MODE_READ 0
#define FT_MODE_WRITE 1

/* File stream structure */

typedef struct S_FT_FILE {
  int fd;
  char buffer[FT_BUFSIZ];
  ft_size_t buf_pos;
  ft_size_t buf_len;
  ft_off_t file_pos; // track buffer position in file
  int mode;          // FT_MODE_READ or FT_MODE_WRITE
  int buf_mode;      // FT_IOFBF, FT_IOLBF, FT_IONBF
  int error;
  int eof;
  int buf_dirty;     // 1 if buffer contains data to flush
  int last_op_write; // to track read->write transitions
  struct S_FT_FILE *next;
} T_FT_FILE;
/* Standard streams */
extern T_FT_FILE *ft_stdout;
extern T_FT_FILE *ft_stdin;
extern T_FT_FILE *ft_stderr;

#define ft_stdout ft_stdout
#define ft_stdin ft_stdin
#define ft_stderr ft_stderr

/* File stream operations */
T_FT_FILE *ft_fopen(const char *filename, const char *mode);
int ft_fclose(T_FT_FILE *fp);
int ft_fflush(T_FT_FILE *fp);
int ft_fileno(T_FT_FILE *fp);

/* Character I/O on streams */
int ft_fputc(char c, T_FT_FILE *fp);
int ft_fgetc(T_FT_FILE *fp);

/* Stream status and control */
int ft_ferror(T_FT_FILE *fp);
int ft_feof(T_FT_FILE *fp);
void ft_clearerr(T_FT_FILE *fp);
int ft_setvbuf(T_FT_FILE *fp, int mode);

/*
** ============================================================================
** SECTION 10: FORMATTED INPUT/OUTPUT (PRINTF FAMILY)
** ============================================================================
*/

/* Printf flags structure */
typedef struct s_flags {
  int width;
  int precision;
  t_bool precision_specified;
  t_bool left_align;
  t_bool zero_padding;
  int length;
} t_flags;

/* Printf family functions */
int ft_printf(const char *format, ...);
int ft_dprintf(int fd, const char *format, ...);
int ft_fprintf(T_FT_FILE *stream, const char *format, ...);
int ft_sprintf(char *buf, const char *fmt, ...);
int ft_snprintf(char *str, ft_size_t size, const char *fmt, ...);
int ft_vdprintf(int fd, const char *format, va_list *args);

/* Scanf family */
int ft_sscanf(const char *str, const char *format, ...);

/*
** ============================================================================
** SECTION 11: LINKED LIST UTILITIES
** ============================================================================
*/

/* Linked list structure */
typedef struct s_list {
  void *content;
  struct s_list *next;
} t_list;

/* List creation and manipulation */
t_list *ft_lstnew(void *content);
void ft_lstadd_front(t_list **lst, t_list *lstnew);
void ft_lstadd_back(t_list **lst, t_list *lstnew);
int ft_lstsize(t_list *lst);
t_list *ft_lstlast(t_list *lst);

/* List iteration and transformation */
void ft_lstiter(t_list *lst, void (*f)(void *));
t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list *ft_lstmapi(t_list *lst, void *(*f)(void *, int), void (*del)(void *));

/* List cleanup */
void ft_lstdelone(t_list *lst, void (*del)(void *));
void ft_lstclear(t_list **lst, void (*del)(void *));

/* List utilities */
void ft_print_list(t_list *lst);

/*
** ============================================================================
** SECTION 12: LIST WRAPPER FUNCTIONS
** ============================================================================
*/

/* Content transformation wrappers */
void *ft_lst_to_upper_wrapper(void *data);
void *ft_lst_to_upper_indice_wrapper(void *content, int i);
void *ft_lst_tolower_wrapper(void *data);
void *ft_lst_strtrim_wrapper(void *data);
void *ft_lst_strdup_wrapper(void *data);

/* Content deletion wrapper */
void ft_lst_del(void *data);

/* Content printing wrappers */
void ft_lst_print_int_wrapper(void *data);
void ft_lst_print_str_wrapper(void *data);
void ft_lst_print_float_wrapper(void *data);
void ft_lst_print_double_wrapper(void *data);

/*
** ============================================================================
** SECTION 13: TERMINAL/DISPLAY UTILITIES
** ============================================================================
*/

/* ANSI color codes */
#define FT_COLOR_RESET "\033[0m"
#define FT_COLOR_RED "\033[31m"
#define FT_COLOR_GREEN "\033[32m"
#define FT_COLOR_YELLOW "\033[33m"
#define FT_COLOR_BLUE "\033[34m"
#define FT_COLOR_MOGENTA "\033[35m"
#define FT_COLOR_CYAN "\033[36m"
#define FT_COLOR_GRAY "\033[90m"
#define FT_COLOR_LIGHT_GREEN "\033[92m"
#define FT_COLOR_ORANGE "\033[38;2;255;165;0m"

/*
** ============================================================================
** SECTION 14: STRING UTILITIES (ADDITIONAL)
** ============================================================================
*/

void ft_reverse(char *str, int length);

#endif /* FT_MAKI_H */