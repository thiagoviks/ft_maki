// #include "include/ft_maki.h"
#include <ft_maki.h>

/*
Test only

int main() {
    const char *msg = "Mini libc funcionando!\n";
   // ft_write(1, msg, ft_strlen(msg));
   ft_putstr(msg);

   char *a_char = (char *)ft_malloc(10);
   a_char[0] = 'A'; a_char[1] = '\n'; a_char[2] = '\0';
   ft_putstr(a_char);
   ft_free(a_char);

   char *b_char = (char *)ft_malloc(10); // deve reutilizar o mesmo bloco
   b_char[0] = 'B'; b_char[1] = '\n'; b_char[2] = '\0';
   ft_putstr_fd(b_char, 1);
   ft_free(b_char);

   t_list *lst = ft_lstnew(ft_strdup("hello"));
   lst->next = ft_lstnew(ft_strdup("world"));
   lst->next->next = ft_lstnew(ft_strdup("42"));

   t_list *upper_lst = ft_lstmap(lst, toupper_wrapper, ft_free);

   ft_printf("Original list:\n");
   print_list(lst);

   ft_printf("\nMapped UpperList:\n");
   print_list(upper_lst);

   t_list *mapped = ft_lstmapi(lst, my_mapper, ft_free);
   ft_printf("\nMapped List:\n");
   print_list(mapped);

   ft_lstclear(&lst, ft_free);
   ft_lstclear(&upper_lst, ft_free);

   ft_printf(COLOR_ORANGE "--------------------\n");
   int a = 'a', b = 'b', c = 'c';
   t_list *lst2 = ft_lstnew(&a);
   ft_lstadd_back(&lst, ft_lstnew(&b));
   ft_lstadd_back(&lst, ft_lstnew(&c));

   t_list *mapped2 = ft_lstmap(lst2, toupper_wrapper, del_int);
   ft_lstiter(mapped2, print_int);
   ft_printf("\n");
   ft_lstclear(&mapped2, del_int);

   // Lista com char*
   t_list *strlst = ft_lstnew("hello");
   ft_lstadd_back(&strlst, ft_lstnew("world"));
   ft_lstadd_back(&strlst, ft_lstnew("42"));

   t_list *dup = ft_lstmap(strlst, strdup_wrapper, del_str);
   ft_lstiter(dup, print_str);
   ft_lstclear(&dup, del_str);

   char buf[128];
   ssize_t n;


   ft_write(1, "Digite algo: ", 14);

   n = ft_read(0, buf, 127);
   if (n > 0)
   {
       buf[n] = '\0';

       ft_write(1, "Voce digitou: ", 15);
       ft_write(1, buf, n);
   }
   else
   {
       ft_write(2, "Erro ao ler entrada\n", 22);
   }

   ft_printf(COLOR_ORANGE"--------------------\n" COLOR_RESET );

   ft_exit(0);
    return (0);
}
*/

// More tests
// I will code a proper suite of tests later
// I have an ideia of a mini_cli using this library, and I will implemented now
int main(void) {
  t_list *words = FT_NULL;
  words = ft_lstnew(ft_strdup("  hello "));
  ft_lstadd_back(&words, ft_lstnew(ft_strdup(" world")));
  ft_lstadd_back(&words, ft_lstnew(ft_strdup("42  ")));

  // Aplicar ft_strtrim
  t_list *trimmed = ft_lstmap(words, ft_lst_strtrim_wrapper, ft_lst_del);
  ft_printf("Trimmed:\n");
  ft_lstiter(trimmed, ft_lst_print_str_wrapper);

  // Aplicar ft_strdup + toupper
  t_list *upper = ft_lstmap(words, ft_lst_to_upper_wrapper, ft_lst_del);
  ft_printf("\nUpper:\n");
  ft_lstiter(upper, ft_lst_print_str_wrapper);

  // Limpar
  ft_lstclear(&words, ft_lst_del);
  ft_lstclear(&trimmed, ft_lst_del);
  ft_lstclear(&upper, ft_lst_del);

  // Teste com int
  int a = 'a', b = 'b', c = 'c';
  t_list *chars = ft_lstnew(&a);
  ft_lstadd_back(&chars, ft_lstnew(&b));
  ft_lstadd_back(&chars, ft_lstnew(&c));

  t_list *upper_chars = ft_lstmap(chars, ft_lst_tolower_wrapper, ft_lst_del);
  ft_printf("\nChars to lower:\n");
  ft_lstiter(upper_chars, ft_lst_print_int_wrapper);
  ft_printf("\n");

  ft_lstclear(&upper_chars, ft_lst_del);
  // ft_lstclear(&chars, ft_free); // original é stack, não libera
  ft_perror("test ft_perror");
  return 0;
}

// int main(void)
// {
//     ft_set_language("en");
//     ft_printf("US\n");
//     ft_list_all_errors();
// }