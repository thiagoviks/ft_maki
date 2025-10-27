#ifndef FT_MAKI_H
#define FT_MAKI_H

#include <stdarg.h> // va_list

/*
** ============================================================================
** SECTION 1: BASIC TYPE DEFINITIONS
** ============================================================================
*/

#define FT_NULL ((void *)0)
#define FT_EXIT_FAILURE 1 /* Failing exit status.  */
#define FT_EXIT_SUCCESS 0 /* Successful exit status.  */
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

// errno.h

#define FT_EDEADLK 35      /* Resource deadlock would occur */
#define FT_ENAMETOOLONG 36 /* File name too long */
#define FT_ENOLCK 37       /* No record locks available */

/*
 * This error code is special: arch syscall entry code will return
 * -ENOSYS if users try to call a syscall that doesn't exist.  To keep
 * failures of syscalls that really do exist distinguishable from
 * failures due to attempts to use a nonexistent syscall, syscall
 * implementations should refrain from returning -ENOSYS.
 */
#define FT_ENOSYS 38 /* Invalid system call number */

#define FT_ENOTEMPTY 39       /* Directory not empty */
#define FT_ELOOP 40           /* Too many symbolic links encountered */
#define FT_EWOULDBLOCK EAGAIN /* Operation would block */
#define FT_ENOMSG 42          /* No message of desired type */
#define FT_EIDRM 43           /* Identifier removed */
#define FT_ECHRNG 44          /* Channel number out of range */
#define FT_EL2NSYNC 45        /* Level 2 not synchronized */
#define FT_EL3HLT 46          /* Level 3 halted */
#define FT_EL3RST 47          /* Level 3 reset */
#define FT_ELNRNG 48          /* Link number out of range */
#define FT_EUNATCH 49         /* Protocol driver not attached */
#define FT_ENOCSI 50          /* No CSI structure available */
#define FT_EL2HLT 51          /* Level 2 halted */
#define FT_EBADE 52           /* Invalid exchange */
#define FT_EBADR 53           /* Invalid request descriptor */
#define FT_EXFULL 54          /* Exchange full */
#define FT_ENOANO 55          /* No anode */
#define FT_EBADRQC 56         /* Invalid request code */
#define FT_EBADSLT 57         /* Invalid slot */

#define FT_EDEADLOCK EDEADLK

#define FT_EBFONT 59       /* Bad font file format */
#define FT_ENOSTR 60       /* Device not a stream */
#define FT_ENODATA 61      /* No data available */
#define FT_ETIME 62        /* Timer expired */
#define FT_ENOSR 63        /* Out of streams resources */
#define FT_ENONET 64       /* Machine is not on the network */
#define FT_ENOPKG 65       /* Package not installed */
#define FT_EREMOTE 66      /* Object is remote */
#define FT_ENOLINK 67      /* Link has been severed */
#define FT_EADV 68         /* Advertise error */
#define FT_ESRMNT 69       /* Srmount error */
#define FT_ECOMM 70        /* Communication error on send */
#define FT_EPROTO 71       /* Protocol error */
#define FT_EMULTIHOP 72    /* Multihop attempted */
#define FT_EDOTDOT 73      /* RFS specific error */
#define FT_EBADMSG 74      /* Not a data message */
#define FT_EOVERFLOW 75    /* Value too large for defined data type */
#define FT_ENOTUNIQ 76     /* Name not unique on network */
#define FT_EBADFD 77       /* File descriptor in bad state */
#define FT_EREMCHG 78      /* Remote address changed */
#define FT_ELIBACC 79      /* Can not access a needed shared library */
#define FT_ELIBBAD 80      /* Accessing a corrupted shared library */
#define FT_ELIBSCN 81      /* .lib section in a.out corrupted */
#define FT_ELIBMAX 82      /* Attempting to link in too many shared libraries */
#define FT_ELIBEXEC 83     /* Cannot exec a shared library directly */
#define FT_EILSEQ 84       /* Illegal byte sequence */
#define FT_ERESTART 85     /* Interrupted system call should be restarted */
#define FT_ESTRPIPE 86     /* Streams pipe error */
#define FT_EUSERS 87       /* Too many users */
#define FT_ENOTSOCK 88     /* Socket operation on non-socket */
#define FT_EDESTADDRREQ 89 /* Destination address required */
#define FT_EMSGSIZE 90     /* Message too long */
#define FT_EPROTOTYPE 91   /* Protocol wrong type for socket */
#define FT_ENOPROTOOPT 92  /* Protocol not available */
#define FT_EPROTONOSUPPORT 93 /* Protocol not supported */
#define FT_ESOCKTNOSUPPORT 94 /* Socket type not supported */
#define FT_EOPNOTSUPP 95    /* Operation not supported on transport endpoint */
#define FT_EPFNOSUPPORT 96  /* Protocol family not supported */
#define FT_EAFNOSUPPORT 97  /* Address family not supported by protocol */
#define FT_EADDRINUSE 98    /* Address already in use */
#define FT_EADDRNOTAVAIL 99 /* Cannot assign requested address */
#define FT_ENETDOWN 100     /* Network is down */
#define FT_ENETUNREACH 101  /* Network is unreachable */
#define FT_ENETRESET 102    /* Network dropped connection because of reset */
#define FT_ECONNABORTED 103 /* Software caused connection abort */
#define FT_ECONNRESET 104   /* Connection reset by peer */
#define FT_ENOBUFS 105      /* No buffer space available */
#define FT_EISCONN 106      /* Transport endpoint is already connected */
#define FT_ENOTCONN 107     /* Transport endpoint is not connected */
#define FT_ESHUTDOWN 108    /* Cannot send after transport endpoint shutdown */
#define FT_ETOOMANYREFS 109 /* Too many references: cannot splice */
#define FT_ETIMEDOUT 110    /* Connection timed out */
#define FT_ECONNREFUSED 111 /* Connection refused */
#define FT_EHOSTDOWN 112    /* Host is down */
#define FT_EHOSTUNREACH 113 /* No route to host */
#define FT_EALREADY 114     /* Operation already in progress */
#define FT_EINPROGRESS 115  /* Operation now in progress */
#define FT_ESTALE 116       /* Stale file handle */
#define FT_EUCLEAN 117      /* Structure needs cleaning */
#define FT_ENOTNAM 118      /* Not a XENIX named type file */
#define FT_ENAVAIL 119      /* No XENIX semaphores available */
#define FT_EISNAM 120       /* Is a named type file */
#define FT_EREMOTEIO 121    /* Remote I/O error */
#define FT_EDQUOT 122       /* Quota exceeded */

#define FT_ENOMEDIUM 123    /* No medium found */
#define FT_EMEDIUMTYPE 124  /* Wrong medium type */
#define FT_ECANCELED 125    /* Operation Canceled */
#define FT_ENOKEY 126       /* Required key not available */
#define FT_EKEYEXPIRED 127  /* Key has expired */
#define FT_EKEYREVOKED 128  /* Key has been revoked */
#define FT_EKEYREJECTED 129 /* Key was rejected by service */

/* for robust mutexes */
#define FT_EOWNERDEAD 130      /* Owner died */
#define FT_ENOTRECOVERABLE 131 /* State not recoverable */

#define FT_ERFKILL 132 /* Operation not possible due to RF-kill */

#define FT_EHWPOISON 133 /* Memory page has hardware error */

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

#define FT_SEEK_SET 0 /* Seek from beginning of file */
#define FT_SEEK_CUR 1 /* Seek from current position */
#define FT_SEEK_END 2 /* Seek from end of file */

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
** SECTION 8-9: FUTEX STUFF
** ============================================================================
*/

/* Timespec structure for futex timeout */
struct ft_timespec {
  long ft_tv_sec;  /* seconds */
  long ft_tv_nsec; /* nanoseconds */
};

/* Mutex structure - single futex word is all we need */
typedef struct s_ft_mutex {
  int futex_word; /* 0=unlocked, 1=locked no waiters, 2=locked with waiters */
} ft_mutex_t;

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
  ft_mutex_t lock;   // mutex for thread-safe operations
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
char *ft_fgets(char *str, int size, T_FT_FILE *stream);

// ft_fseek

/* Syscall number for lseek - architecture dependent */
#define FT_SYS_lseek 8 /* x86_64 syscall number */

// errno.h start
#define FT_EDEADLK 35      /* Resource deadlock would occur */
#define FT_ENAMETOOLONG 36 /* File name too long */
#define FT_ENOLCK 37       /* No record locks available */

/*
 * This error code is special: arch syscall entry code will return
 * -ENOSYS if users try to call a syscall that doesn't exist.  To keep
 * failures of syscalls that really do exist distinguishable from
 * failures due to attempts to use a nonexistent syscall, syscall
 * implementations should refrain from returning -ENOSYS.
 */
#define FT_ENOSYS 38 /* Invalid system call number */

#define FT_ENOTEMPTY 39       /* Directory not empty */
#define FT_ELOOP 40           /* Too many symbolic links encountered */
#define FT_EWOULDBLOCK EAGAIN /* Operation would block */
#define FT_ENOMSG 42          /* No message of desired type */
#define FT_EIDRM 43           /* Identifier removed */
#define FT_ECHRNG 44          /* Channel number out of range */
#define FT_EL2NSYNC 45        /* Level 2 not synchronized */
#define FT_EL3HLT 46          /* Level 3 halted */
#define FT_EL3RST 47          /* Level 3 reset */
#define FT_ELNRNG 48          /* Link number out of range */
#define FT_EUNATCH 49         /* Protocol driver not attached */
#define FT_ENOCSI 50          /* No CSI structure available */
#define FT_EL2HLT 51          /* Level 2 halted */
#define FT_EBADE 52           /* Invalid exchange */
#define FT_EBADR 53           /* Invalid request descriptor */
#define FT_EXFULL 54          /* Exchange full */
#define FT_ENOANO 55          /* No anode */
#define FT_EBADRQC 56         /* Invalid request code */
#define FT_EBADSLT 57         /* Invalid slot */

#define FT_EDEADLOCK EDEADLK

#define FT_EBFONT 59       /* Bad font file format */
#define FT_ENOSTR 60       /* Device not a stream */
#define FT_ENODATA 61      /* No data available */
#define FT_ETIME 62        /* Timer expired */
#define FT_ENOSR 63        /* Out of streams resources */
#define FT_ENONET 64       /* Machine is not on the network */
#define FT_ENOPKG 65       /* Package not installed */
#define FT_EREMOTE 66      /* Object is remote */
#define FT_ENOLINK 67      /* Link has been severed */
#define FT_EADV 68         /* Advertise error */
#define FT_ESRMNT 69       /* Srmount error */
#define FT_ECOMM 70        /* Communication error on send */
#define FT_EPROTO 71       /* Protocol error */
#define FT_EMULTIHOP 72    /* Multihop attempted */
#define FT_EDOTDOT 73      /* RFS specific error */
#define FT_EBADMSG 74      /* Not a data message */
#define FT_EOVERFLOW 75    /* Value too large for defined data type */
#define FT_ENOTUNIQ 76     /* Name not unique on network */
#define FT_EBADFD 77       /* File descriptor in bad state */
#define FT_EREMCHG 78      /* Remote address changed */
#define FT_ELIBACC 79      /* Can not access a needed shared library */
#define FT_ELIBBAD 80      /* Accessing a corrupted shared library */
#define FT_ELIBSCN 81      /* .lib section in a.out corrupted */
#define FT_ELIBMAX 82      /* Attempting to link in too many shared libraries */
#define FT_ELIBEXEC 83     /* Cannot exec a shared library directly */
#define FT_EILSEQ 84       /* Illegal byte sequence */
#define FT_ERESTART 85     /* Interrupted system call should be restarted */
#define FT_ESTRPIPE 86     /* Streams pipe error */
#define FT_EUSERS 87       /* Too many users */
#define FT_ENOTSOCK 88     /* Socket operation on non-socket */
#define FT_EDESTADDRREQ 89 /* Destination address required */
#define FT_EMSGSIZE 90     /* Message too long */
#define FT_EPROTOTYPE 91   /* Protocol wrong type for socket */
#define FT_ENOPROTOOPT 92  /* Protocol not available */
#define FT_EPROTONOSUPPORT 93 /* Protocol not supported */
#define FT_ESOCKTNOSUPPORT 94 /* Socket type not supported */
#define FT_EOPNOTSUPP 95    /* Operation not supported on transport endpoint */
#define FT_EPFNOSUPPORT 96  /* Protocol family not supported */
#define FT_EAFNOSUPPORT 97  /* Address family not supported by protocol */
#define FT_EADDRINUSE 98    /* Address already in use */
#define FT_EADDRNOTAVAIL 99 /* Cannot assign requested address */
#define FT_ENETDOWN 100     /* Network is down */
#define FT_ENETUNREACH 101  /* Network is unreachable */
#define FT_ENETRESET 102    /* Network dropped connection because of reset */
#define FT_ECONNABORTED 103 /* Software caused connection abort */
#define FT_ECONNRESET 104   /* Connection reset by peer */
#define FT_ENOBUFS 105      /* No buffer space available */
#define FT_EISCONN 106      /* Transport endpoint is already connected */
#define FT_ENOTCONN 107     /* Transport endpoint is not connected */
#define FT_ESHUTDOWN 108    /* Cannot send after transport endpoint shutdown */
#define FT_ETOOMANYREFS 109 /* Too many references: cannot splice */
#define FT_ETIMEDOUT 110    /* Connection timed out */
#define FT_ECONNREFUSED 111 /* Connection refused */
#define FT_EHOSTDOWN 112    /* Host is down */
#define FT_EHOSTUNREACH 113 /* No route to host */
#define FT_EALREADY 114     /* Operation already in progress */
#define FT_EINPROGRESS 115  /* Operation now in progress */
#define FT_ESTALE 116       /* Stale file handle */
#define FT_EUCLEAN 117      /* Structure needs cleaning */
#define FT_ENOTNAM 118      /* Not a XENIX named type file */
#define FT_ENAVAIL 119      /* No XENIX semaphores available */
#define FT_EISNAM 120       /* Is a named type file */
#define FT_EREMOTEIO 121    /* Remote I/O error */
#define FT_EDQUOT 122       /* Quota exceeded */

#define FT_ENOMEDIUM 123    /* No medium found */
#define FT_EMEDIUMTYPE 124  /* Wrong medium type */
#define FT_ECANCELED 125    /* Operation Canceled */
#define FT_ENOKEY 126       /* Required key not available */
#define FT_EKEYEXPIRED 127  /* Key has expired */
#define FT_EKEYREVOKED 128  /* Key has been revoked */
#define FT_EKEYREJECTED 129 /* Key was rejected by service */

/* for robust mutexes */
#define FT_EOWNERDEAD 130      /* Owner died */
#define FT_ENOTRECOVERABLE 131 /* State not recoverable */

#define FT_ERFKILL 132 /* Operation not possible due to RF-kill */

#define FT_EHWPOISON 133 /* Memory page has hardware error */

// errno.h end

// stat.h flags start
/* Encoding of the file mode.  */

#define FT_S_IFMT 0170000 /* These bits determine file type.  */

/* File types.  */
#define FT_S_IFDIR 0040000  /* Directory.  */
#define FT_S_IFCHR 0020000  /* Character device.  */
#define FT_S_IFBLK 0060000  /* Block device.  */
#define FT_S_IFREG 0100000  /* Regular file.  */
#define FT_S_IFIFO 0010000  /* FIFO.  */
#define FT_S_IFLNK 0120000  /* Symbolic link.  */
#define FT_S_IFSOCK 0140000 /* Socket.  */

/* Protection bits.  */

#define FT_S_ISUID 04000 /* Set user ID on execution.  */
#define FT_S_ISGID 02000 /* Set group ID on execution.  */
#define FT_S_ISVTX 01000 /* Save swapped text after use (sticky).  */
#define FT_S_IREAD 0400  /* Read by owner.  */
#define FT_S_IWRITE 0200 /* Write by owner.  */
#define FT_S_IEXEC 0100  /* Execute by owner.  */

#define FT_S_ISTYPE(mode, mask) (((mode) & FT_S_IFMT) == (mask))
#define FT_S_ISBLK(mode) FT_S_ISTYPE((mode), FT_S_IFBLK)

// stat.h flags end

/* Simplified file table - in real kernel this is per-process */
#define FT_MAX_FDS 1024
ft_off_t ft_lseek(int fd, ft_off_t offset, int whence);
int ft_fseek(T_FT_FILE *stream, ft_off_t offset, int whence);
ft_off_t ft_ftell(T_FT_FILE *stream);
void ft_rewind(T_FT_FILE *stream);
ft_size_t ft_fread(void *ptr, ft_size_t size, ft_size_t nmemb,
                   T_FT_FILE *stream);
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