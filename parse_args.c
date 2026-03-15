#include "rush02.h"

int	is_valid_number(char *str)
{
	int	i;

	// Se o ponteiro for NULL, significa que nem existe string para validar.
	// Se str[0] for '\0', significa que a string existe, mas está vazia ("").
	// Em ambos os casos, não dá para considerar isso um número válido.
	if (!str || !str[0])
		return (0);

	i = 0;

	// Vai andando caractere por caractere dentro da string,
	// até bater no '\0', que marca o final dela na memória.
	while (str[i])
	{
		// Cada posição str[i] guarda um char.
		// ft_is_digit verifica se esse char está entre '0' e '9' na tabela ASCII.
		// Se aparecer qualquer coisa fora disso (letra, espaço, sinal, etc),
		// já aborta e retorna 0, dizendo: "isso não é um número puro".
		if (!ft_is_digit(str[i]))
			return (0);

		// Avança para a próxima casa da string.
		i++;
	}

	// Se percorreu a string inteira e todos os caracteres eram dígitos,
	// então é um número válido.
	return (1);
}

char	*normalize_number(char *str)
{
	int	i;

	i = 0;

	// Aqui a ideia é "pular" os zeros à esquerda.
	// Exemplo:
	// "00042" -> queremos começar a olhar a partir do '4'
	//
	// Mas tem um detalhe importante:
	// o loop só pula o zero se existir outro caractere depois dele.
	// Isso evita transformar "0" em string vazia.
	//
	// Exemplos:
	// "00042" -> para no '4'
	// "0000"  -> para no último '0', preservando "0"
	while (str[i] == '0' && str[i + 1])
		i++;

	// str + i não cria uma nova string;
	// apenas aponta para outro lugar dentro da mesma string original.
	//
	// Exemplo:
	// se str = "00042" e i = 3,
	// então (str + i) aponta para a substring "42".
	//
	// ft_strdup entra em ação para criar uma CÓPIA nova em memória heap,
	// com malloc, para que o resto do programa possa usar essa string
	// de forma segura e independente da original.
	return (ft_strdup(str + i));
}