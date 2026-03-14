#include "rush02.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[1]);
}

int	ft_is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_is_digit(char c)
{
	return (c >= '0' && c <= '9');
}
