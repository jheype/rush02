#include "rush02.h"

char	*ft_strtrim_spaces(char *str)
{
	int	start;
	int	end;

	// "start" vai andar da esquerda para a direita
	// procurando onde começa o conteúdo real.
	start = 0;

	// "end" começa no último caractere válido da string
	// (antes do '\0') e vai andar da direita para a esquerda
	// procurando onde termina o conteúdo real.
	end = ft_strlen(str) - 1;

	// Enquanto o caractere atual da esquerda existir
	// e for um espaço em branco, avança.
	//
	// Exemplo:
	// "   42  "
	//  ^
	// start vai pulando até cair no '4'
	while (str[start] && ft_is_space(str[start]))
		start++;

	// Agora limpa do lado direito.
	// Enquanto end ainda não cruzou com start
	// e o caractere atual for espaço, recua.
	//
	// Exemplo:
	// "   42  "
	//       ^
	// end vai voltando até parar no '2'
	while (end >= start && ft_is_space(str[end]))
		end--;

	// Depois que delimitamos o conteúdo útil,
	// extraímos exatamente esse intervalo.
	//
	// end - start + 1 = quantidade real de caracteres a copiar
	//
	// Exemplo:
	// str = "   hello   "
	// start = 3
	// end = 7
	// len = 7 - 3 + 1 = 5
	// resultado = "hello"
	return (ft_substr(str, start, end - start + 1));
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;
	int		len1;
	int		len2;
	int		i;
	int		j;

	// Mede o tamanho das duas strings para saber
	// exatamente quantos bytes precisam ser alocados.
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);

	// Reserva espaço para juntar as duas
	// e ainda colocar o '\0' no final.
	res = (char *)malloc(len1 + len2 + 1);

	// Se falhou o malloc, a função decide limpar s1
	// antes de ir embora.
	//
	// Isso geralmente é feito porque s1 costuma ser uma string
	// acumuladora construída dinamicamente ao longo do programa,
	// então, se a concatenação falhar, ela não será mais usada.
	if (!res)
		return (free(s1), NULL);

	i = 0;

	// Copia s1 inteira para o começo de res.
	while (i < len1)
	{
		res[i] = s1[i];
		i++;
	}

	j = 0;

	// Agora continua preenchendo res a partir de onde s1 terminou,
	// colando s2 logo em seguida.
	//
	// O res[i++] = s2[j++] faz duas coisas ao mesmo tempo:
	// - grava s2[j] em res[i]
	// - depois incrementa i e j
	while (j < len2)
		res[i++] = s2[j++];

	// Fecha a string final com '\0'.
	res[i] = '\0';

	// Libera s1 porque agora o conteúdo dela já foi incorporado em res.
	// Isso evita vazamento de memória.
	free(s1);

	// Retorna a nova string concatenada.
	return (res);
}