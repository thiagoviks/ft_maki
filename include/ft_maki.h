#ifndef FT_MAKI_H
#define FT_MAKI_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h> //ssize_t

#define COLOR_RESET			"\033[0m"
#define COLOR_GREEN			"\033[32m"   // Verde para 'S'
#define COLOR_RED			"\033[31m"   // Vermelho para 'G'
#define COLOR_GRAY			"\033[90m"   // Cinza para '#'
#define COLOR_YELLOW    	"\033[33m"   // Amarelo para '*'
#define COLOR_CYAN			"\033[36m"   // Ciano para '.'
#define COLOR_BLUE			"\033[34m"   // Azul para '~'
#define COLOR_LIGHT_GREEN   "\033[92m"   // Verde claro para 'g'
#define COLOR_MOGENTA		"\033[35m"  // Magenta para 'm'
#define COLOR_ORANGE		"\033[38;2;255;165;0m"
#define BLOCK_SIZE sizeof(t_block)
#define MMAP_THRESHOLD (128 * 1024) // 128 KB
#define ALIGNMENT 16

//Is used for RAM magenement
typedef struct		s_block {
    size_t size;
    int free;
    int is_mmap;
    struct s_block *next;
    struct s_block *prev;
}					t_block;

// Flags struct  for ft_printf
typedef struct		s_flags {
    int width;
    int precision;
    bool precision_specified;
    bool left_align;
    bool zero_padding;
}					t_flags;

// linked list struct
typedef struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

//ft_maki.c start
//make syscall
ssize_t				ft_write(int fd, const void *buf, size_t len);
void				ft_exit(long code);
ssize_t				ft_read(int fd, void *buf, size_t count);
int					ft_open(const char *pathname, int flags, int mode);
int					ft_close(int fd);

size_t				ft_strlen(const char *s);
void				ft_putstr(const char *str);
int					ft_atoi(const char *nptr);
char				*ft_strdup(const char *s);
void				ft_reverse(char *str, int length);
int					ft_itoa_str(int num, char *str);

//RAM manager
void				*ft_malloc(size_t size);
void				ft_free(void *ptr);
void 				*ft_memset(void *ptr, int value, size_t num);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
void 				*ft_realloc(void *ptr, size_t size);

// Internal helper functions used exclusively by ft_malloc for memory management.
// static size_t align(size_t size);
// static t_block *find_free_block(size_t size);
// static t_block *request_space(size_t size);
// static void coalesce(t_block *block);

//ft_maki.c end

//All these functions is for ft_printf start_comment
int     			ft_printf_putchar(char c);
int     			ft_printf(const char *format, ...);
int     			ft_printf_putstr(char *s, t_flags flags);
int     			ft_printf_putnbr(long long n, t_flags flags);
int     			ft_printf_putunbr(unsigned int n, t_flags flags);
int     			ft_printf_puthex(unsigned long n, t_flags flags, int uppercase);
int     			ft_printf_putptr(void *ptr, t_flags flags);
int     			ft_printf_putfloat(double n, t_flags flags);
int     			ft_printf_putpercent(t_flags flags);
t_flags				ft_printf_parse_flags(const char **format);
int     			ft_printf_numlen(long long n);
int     			ft_printf_unumlen(unsigned long n);
int     			ft_printf_max(int a, int b);
int     			ft_printf_pad(char c, int count);
long long 			ft_printf_power10(int n);
int 				ft_printf_putchar_base(char c, int width, int left);
//All these functions is for ft_printf end_comment


//ft_utils.c start
void				ft_bzero(void *s, size_t n);
void				*ft_memccpy(void *dest, const void *src, int c, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
char				*ft_strnstr(const char *big, const char *little, size_t len);
int					ft_strncmp(const char *s1, const char *s2, size_t n);

int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_toupper(int c);
int					ft_tolower(int c);

char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));

void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list 				*ft_lstmapi(t_list *lst, void *(*f)(void *, int), void (*del)(void *));
void 				ft_print_list(t_list *lst);
//ft_utils.c end

//ft_lst_wrappers.c start
//Wrappers to use ft_function in ft_lst_functions
void				*ft_lst_to_upper_wrapper(void *data);
void				*ft_lst_to_upper_indice_wrapper(void *content, int i);
void				*ft_lst_tolower_wrapper(void *data);
void				*ft_lst_strtrim_wrapper(void *data);
void				*ft_lst_strdup_wrapper(void *data);
void				ft_lst_del(void *data);
void				ft_lst_print_int_wrapper(void *data);
void				ft_lst_print_str_wrapper(void *data);
void				ft_lst_print_float_wrapper(void *data);
void				ft_lst_print_double_wrapper(void *data);
//ft_lst_wrappers.c end

#endif

