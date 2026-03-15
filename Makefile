NAME = rush-02

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c \
	parse_args.c \
	dict.c \
	dict_parse.c \
	dict_utils.c \
	convert.c \
	convert_group.c \
	stdin.c \
	string_utils.c \
	string_utils2.c \
	output.c \
	free.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
