NAME = rush-02

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c \
	parse_args.c \
	string_utils.c \
	output.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
