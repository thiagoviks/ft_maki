#include "../include/ft_maki.h"

void    *ft_lst_to_upper_wrapper(void *data)
{
	if (!data)
		return NULL;

	char *str = ft_strdup((char *)data);
	if (!str)
		return NULL;

	for (int i = 0; str[i]; i++)
		str[i] = ft_toupper((unsigned char)str[i]);

	return str;
}

void    *ft_lst_to_upper_indice_wrapper(void *content, int i)
{
	char *str = (char *)content;
	char *res = ft_strdup(str);
	if (!res)
		return NULL;

	// só transforma a letra no índice `i` (se existir)
	if ((int)ft_strlen(res) > i)
		res[i] = ft_toupper((unsigned char)res[i]);
	return res;
}

void	*ft_lst_tolower_wrapper(void *data)
{
    int *original = (int *)data;
    int *copy = ft_malloc(sizeof(int));
    if (!copy)
        return NULL;
    *copy = ft_tolower(*original);
    return copy;
}

void	*ft_lst_strtrim_wrapper(void *data)
{
    if (!data) return NULL;
    return ft_strtrim((char *)data, " ");
}

void	*ft_lst_strdup_wrapper(void *data)
{
    if (!data) return NULL;
    return ft_strdup((char *)data);
}

void	ft_lst_del(void *data)
{
    ft_free(data);
}

void	ft_lst_print_int_wrapper(void *data)
{
    ft_printf("%d ", *(int *)data);
}

void	ft_lst_print_str_wrapper(void *data)
{
    ft_printf("%s\n", (char *)data);
}

void	ft_lst_print_float_wrapper(void *data)
{
    ft_printf("%f\n", *(float *)data);
}

void	ft_lst_print_double_wrapper(void *data)
{
    ft_printf("%f\n", *(double *)data);
}
