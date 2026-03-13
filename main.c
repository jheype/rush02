#include "rush02.h"

static void	print_error(char *msg)
{
	write(2, msg, ft_strlen(msg));
}

int	main(int argc, char **argv)
{
	char	*dict_file;
	char	*num;

	if (argc == 2)
	{
		dict_file = "numbers.dict";
		num = argv[1];
	}
	else if (argc == 3)
	{
		dict_file = argv[1];
		num = argv[2];
	}
	else
		return (print_error("Error\n"), 1);
	if (!is_valid_number(num))
		return (print_error("Error\n"), 1);
	return (0);
}
