#echo 'export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
#source ~/.bashrc


NAME = ft_maki
SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

CC = clang
CFLAGS = -Wall -Wextra -Werror -fPIC -I$(INCLUDE_DIR)

.PHONY: all clean fclean re test install

all: $(OBJ_DIR) lib$(NAME).a lib$(NAME).so

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

lib$(NAME).a: $(OBJ)
	ar rcs $@ $^

lib$(NAME).so: $(OBJ)
	$(CC) -shared -Wl,-z,defs -o $@ $^

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f lib$(NAME).a lib$(NAME).so main

re: fclean all

test: all
	$(CC) main.c -I$(INCLUDE_DIR) -L. -l$(NAME) -o main -no-pie

install: all
	sudo cp lib$(NAME).a /usr/local/lib/
	sudo cp lib$(NAME).so /usr/local/lib/
	sudo cp $(INCLUDE_DIR)/ft_maki.h /usr/local/include/
	sudo ldconfig
	echo "✅ ft_maki installed successfully!"
	echo "You can now use:"
	echo "#include <ft_maki.h>"
	echo "and compile with: gcc main.c -lft_maki"
