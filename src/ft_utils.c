#include "../include/ft_maki.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		*((unsigned char *)dest + i) = *((unsigned char *)src + i);
		if (*((unsigned char *)src + i) == (unsigned char)c)
		{
			return (dest + i + 1);
			break ;
		}
		i++;
	}
	return (NULL);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (dest == src || !n)
		return (dest);
	if (dest < src)
	{
		while (i < n)
		{
			*((char *)dest + i) = *((char *)src + i);
			i++;
		}
	}
	else
	{
		while (n > 0)
		{
			*((char *)dest + n - 1) = *((char *)src + n - 1);
			n--;
		}
	}
	return (dest);
}

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		if (*((unsigned char *)s + i) == (unsigned char)c)
			return ((void *)s + i);
		i++;
	}
	return (NULL);
}

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		if (*((unsigned char *)s1 + i) == *((unsigned char *)s2 + i))
			i++;
		else
			return ((*((unsigned char *)s1 + i))
					- *((unsigned char *)s2 + i));
	}
	return (0);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	count;

	if (!dst || !src)
		return (0);
	count = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (count < size - 1 && src[count])
	{
		dst[count] = src[count];
		count++;
	}
	dst[count] = '\0';
	return (ft_strlen(src));
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	size_dest;
	size_t	size_src;

	size_dest = ft_strlen(dest);
	size_src = ft_strlen(src);
	if (size == 0)
		return (size_src);
	i = 0;
	if (size >= size_dest)
	{
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

char	*ft_strchr(const char *s, int c)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return ((char *)s + i);
		i++;
	}
	if (c == '\0')
		return ((char *)s + i);
	return (0);
}

char	*ft_strrchr(const char *s, int c)
{
	int i;

	i = 0;
	while (s[i] != '\0')
		i++;
	if (c == 0)
		return ((char *)s + i);
	i--;
	while (i >= 0 && s[i] != '\0')
	{
		if (s[i] == c)
			return ((char *)s + i);
		i--;
	}
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t little_len;

	if (*little == '\0')
		return ((char *)big);
	little_len = ft_strlen((char *)little);
	while (*big != '\0' && len-- >= little_len)
	{
		if (*big == *little && ft_memcmp(big, little, little_len) == 0)
			return ((char *)big);
		big++;
	}
	return (NULL);
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] != '\0' && s2[i] != '\0')
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i != n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

int		ft_isalpha(int c)
{
	if (c >= 65 && c <= 90)
		return (c);
	else if (c >= 97 && c <= 122)
		return (c);
	return (0);
}

int		ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (c);
	return (0);
}

int		ft_isalnum(int c)
{
	if (c >= 48 && c <= 57)
		return (c);
	else if (c >= 65 && c <= 90)
		return (c);
	else if (c >= 97 && c <= 122)
		return (c);
	return (0);
}

int		ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}

int		ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (c);
	return (0);
}

int		ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		return (c - 32);
	return (c);
}

int		ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		return (c + 32);
	return (c);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*buff;

	if (!s)
		return (NULL);
	if (!(buff = (char *)ft_malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	if (start < ft_strlen(s))
	{
		while (s[start + i] && i < len)
		{
			buff[i] = s[start + i];
			i++;
		}
	}
	buff[i] = '\0';
	return (buff);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*buff;

	if (!s1 || !s2)
		return (NULL);
	if (!(buff = (char *ft_malloc(sizeof(char) *
					(ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (i < ft_strlen(s1))
	{
		buff[i] = s1[i];
		i++;
	}
	while (j < ft_strlen(s2))
	{
		buff[i] = s2[j];
		i++;
		j++;
	}
	buff[i] = '\0';
	return (buff);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len_s1;
	char	*buff;

	if (!s1 || !set)
		return (NULL);
	while (ft_strchr(set, *s1) && *s1 != '\0')
		s1++;
	len_s1 = ft_strlen(s1);
	while (ft_strchr(set, *(s1 + len_s1)) && len_s1 > 0)
		len_s1--;
	if (!(buff = ft_substr(s1, 0, len_s1 + 1)))
		return (NULL);
	return (buff);
}

static size_t	count_word(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else
			i++;
	}
	return (count);
}

static char	**free_buff(char **buff, size_t j)
{
	while (j-- > 0)
	{
		ft_free(buff[j]);
		buff[j] = NULL;
	}
	ft_free(buff);
	buff = NULL;
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**buff;
	size_t	i;
	size_t	j;
	size_t	word_num;

	if (!s ||\
			!(buff = (char **)ft_malloc(sizeof(char *) * (count_word(s, c) + 1))))
		return (NULL);
	j = 0;
	word_num = count_word(s, c);
	while (*s && j < word_num)
	{
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

static int	count_digits(int n)
{
	int count = 1;
	unsigned int n_abs = (n < 0) ? -(unsigned int)n : (unsigned int)n;

	while (n_abs >= 10)
	{
		n_abs /= 10;
		count++;
	}
	return count;
}

char	*ft_itoa(int n)
{
	int				digits;
	int				len;
	char			*buff;
	unsigned int	n_abs;

	digits = count_digits(n);
	len = digits + (n < 0); // +1 if negative (for the '-')
	buff = ft_malloc(sizeof(char) * (len + 1));
	if (!buff)
		return NULL;

	buff[len] = '\0';
	n_abs = (n < 0) ? -(unsigned int)n : (unsigned int)n;

	while (digits-- > 0)
	{
		buff[--len] = (n_abs % 10) + '0';
		n_abs /= 10;
	}

	if (n < 0)
		buff[0] = '-';

	return buff;
}

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	char	*buff;

	i = 0;
	if (!s || !f || !(buff = (char *)ft_malloc(sizeof(char) * (ft_strlen(s) + 1))))
		return (NULL);
	while (i < ft_strlen(s))
	{
		buff[i] = f(i, s[i]);
		i++;
	}
	buff[i] = '\0';
	return (buff);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	n2;

	if (n < 0)
		ft_putchar_fd('-', fd);
	n2 = n < 0 ? (unsigned int)(n * (-1)) : (unsigned int)n;
	if (n2 >= 10)
		ft_putnbr_fd(n2 / 10, fd);
	ft_putchar_fd(n2 % 10 + '0', fd);
}

t_list		*ft_lstnew(void *content)
{
	t_list	*head;

	if (!(head = (t_list *)ft_malloc(sizeof(t_list))))
		return (NULL);
	head->content = content;
	head->next = NULL;
	return (head);
}

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	if (new)
	{
		new->next = *lst;
		*lst = new;
	}
}

int		ft_lstsize(t_list *lst)
{
	t_list	*curr;
	int		count;

	count = 0;
	curr = lst;
	while (curr)
	{
		curr = curr->next;
		count++;
	}
	return (count);
}

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*curr;
	int		count;

	count = ft_lstsize(lst);
	curr = lst;
	while (count > 1)
	{
		curr = curr->next;
		count--;
	}
	return (curr);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list *last;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	if (new)
	{
		last = ft_lstlast(*lst);
		last->next = new;
	}
}

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	ft_free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list *temp;

	if (!lst)
		return ;
	while (*lst)
	{
		del((*lst)->content);
		temp = *lst;
		*lst = (*lst)->next;
		free(temp);
	}
	*lst = NULL;
}

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst && f)
	{
		while (lst != NULL)
		{
			f(lst->content);
			lst = lst->next;
		}
	}
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *newelem;
	t_list *newlist;

	if (!lst || !f)
		return (NULL);
	newlist = NULL;
	while (lst)
	{
		if (!(newelem = ft_lstnew(f(lst->content))))
		{
			ft_lstclear(&newlist, del);
			return (NULL);
		}
		ft_lstadd_back(&newlist, newelem);
		lst = lst->next;
	}
	return (newlist);
}