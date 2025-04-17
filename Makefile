#to compile use your program
#clang main.c $(pkg-config --cflags --libs ft_maki) -o main

NAME = libft_maki.a
CC = clang
CFLAGS = -Wall -Wextra -Werror -nostdlib -static -fno-stack-protector -nodefaultlibs -fPIC
SRC = src/ft_maki.c src/ft_printf.c src/ft_utils.c src/ft_lst_wrapper.c
OBJ = $(SRC:.c=.o)
INCLUDES = -Iinclude

.PHONY: all clean fclean re install install-pkgconfig uninstall

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

install: $(NAME)
	install -Dm644 $(NAME) $(PREFIX)/lib/$(NAME)
	install -Dm644 include/ft_maki.h $(PREFIX)/include/ft_maki.h

install-pkgconfig: install
	@echo "Instalando ft_maki.pc em $(PREFIX)/lib/pkgconfig"
	sed "s|@PREFIX@|$(PREFIX)|g" ft_maki.pc.in > ft_maki.pc
	install -Dm644 ft_maki.pc $(PREFIX)/lib/pkgconfig/ft_maki.pc

uninstall:
	rm -f $(PREFIX)/lib/$(NAME)
	rm -f $(PREFIX)/include/ft_maki.h
	rm -f $(PREFIX)/lib/pkgconfig/ft_maki.pc