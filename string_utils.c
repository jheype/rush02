#include "rush02.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;

	// Conta quantos caracteres existem na string até encontrar o '\0'.
	//
	// O detalhe importante é esse "str &&":
	// ele protege contra segfault caso alguém passe NULL.
	// Se str for NULL, o while nem entra.
	while (str && str[i])
		i++;

	// Retorna o tamanho lógico da string,
	// ou seja, quantos caracteres vêm antes do terminador '\0'.
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;

	// Vai comparando as duas strings posição por posição.
	// Enquanto ambas ainda não chegaram ao fim
	// e os caracteres forem iguais, continua andando.
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	// Quando sair do loop, aconteceu uma destas coisas:
	// 1) os caracteres diferiram
	// 2) uma das strings acabou
	//
	// O retorno é a diferença entre os bytes.
	// Isso segue o comportamento clássico do strcmp:
	// - 0  => strings iguais
	// - <0 => s1 "menor" que s2
	// - >0 => s1 "maior" que s2
	//
	// O cast para unsigned char evita problemas com chars assinados,
	// garantindo comparação consistente pelos valores ASCII reais.
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;

	// Mesma lógica do strcmp, mas com um teto:
	// ele só compara até n caracteres no máximo.
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	// Se i chegou exatamente em n,
	// significa que os primeiros n caracteres eram iguais.
	// Então, para a função, isso já basta para considerar empate.
	if (i == n)
		return (0);

	// Caso contrário, retorna a diferença no primeiro ponto onde divergiu,
	// ou onde uma string acabou antes da outra.
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strdup(char *src)
{
	char	*dest;
	int		i;

	// Reserva memória suficiente para copiar a string inteira
	// + 1 byte para o '\0' final.
	//
	// Exemplo:
	// "abc" tem tamanho 3, então aloca 4 bytes.
	dest = (char *)malloc(ft_strlen(src) + 1);
	if (!dest)
		return (NULL);

	i = 0;

	// Copia caractere por caractere da origem para o destino.
	// Aqui não copia o '\0' ainda; isso é feito depois do loop.
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}

	// Fecha a nova string com '\0'.
	// Sem isso, o destino não seria uma string C válida.
	dest[i] = '\0';

	// Retorna o ponteiro para essa nova região de memória.
	return (dest);
}

char	*ft_substr(char *src, int start, int len)
{
	char	*dest;
	int		i;

	// Se src for NULL, não existe string para fatiar.
	// Se len for negativo, o pedido é inválido.
	if (!src || len < 0)
		return (NULL);

	// Aloca espaço para exatamente "len" caracteres
	// mais o '\0' no final.
	dest = (char *)malloc(len + 1);
	if (!dest)
		return (NULL);

	i = 0;

	// Copia um trecho da string original, começando em "start"
	// e pegando "len" caracteres.
	//
	// Exemplo:
	// src = "abcdef"
	// start = 2
	// len = 3
	// resultado = "cde"
	while (i < len)
	{
		dest[i] = src[start + i];
		i++;
	}

	// Encerra a substring como string válida em C.
	dest[i] = '\0';

	return (dest);
}

int	ft_is_space(char c)
{
	// Verifica se o caractere é um espaço em branco.
	//
	// ' '        => espaço comum
	// 9 a 13     => caracteres de controle clássicos:
	//               \t, \n, \v, \f, \r
	//
	// Isso é útil para ignorar lixo nas bordas de uma string.
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_is_digit(char c)
{
	// Verifica se o caractere está no intervalo ASCII de '0' até '9'.
	// Se estiver, retorna verdadeiro; senão, falso.
	return (c >= '0' && c <= '9');
}