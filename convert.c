#include "rush02.h"

// make_scale_key gera dinamicamente a chave da escala que o programa precisa buscar.
//
// group_index:
// 1 -> mil      -> "1000"
// 2 -> milhão   -> "1000000"
// 3 -> bilhão   -> "1000000000"
//
// a lógica é:
// "1" seguido de (group_index * 3) zeros
//
// isso é importante porque evita hardcodear todas as escalas no código.
// quem manda é o dicionário.
char	*make_scale_key(int group_index)
{
	char	*key;
	int		zeros;
	int		i;

	if (group_index <= 0)
		return (NULL);
	zeros = group_index * 3;
	key = (char *)malloc(zeros + 2);
	if (!key)
		return (NULL);
	key[0] = '1';
	i = 1;
	while (i <= zeros)
	{
		key[i] = '0';
		i++;
	}
	key[i] = '\0';
	return (key);
}

// is_chunk_zero verifica se um grupo inteiro é zero.
// ex:
// "000" -> true
// "001" -> false
//
// isso serve para pular grupos inúteis.
// por exemplo:
// 1 000 005
// o grupo do meio ("000") não deve gerar "zero thousand".
int	is_chunk_zero(char *chunk)
{
	int	i;

	i = 0;
	while (chunk[i])
	{
		if (chunk[i] != '0')
			return (0);
		i++;
	}
	return (1);
}

// chunk_from_number é só um wrapper para ft_substr.
// aqui a ideia semântica é deixar claro que estou extraindo
// um pedaço do número principal para tratar como grupo isolado.
char	*chunk_from_number(char *num, int start, int len)
{
	return (ft_substr(num, start, len));
}

// group_count calcula quantos grupos de até 3 dígitos existem no número.
//
// exemplos:
// "7"        -> 1 grupo
// "1234"     -> 2 grupos  ("1" | "234")
// "1234567"  -> 3 grupos  ("1" | "234" | "567")
static int	group_count(char *num)
{
	int	len;
	int	count;

	len = ft_strlen(num);
	count = len / 3;
	if (len % 3)
		count++;
	return (count);
}

// has_nonzero_group_after olha os grupos seguintes para descobrir
// se existe algum grupo não-zero depois do atual.
//
// isso é usado principalmente para decidir se vale colocar vírgula
// em inglês entre escalas maiores.
//
// ex:
// 1,234,567
// depois de "million" ainda existem grupos não-zero,
// então a vírgula faz sentido.
//
// se o que vier depois for só "000", não precisa.
static int	has_nonzero_group_after(char *num, int current_group, int groups,
		int first_len)
{
	int	start;
	int	len;
	char	*chunk;
	int	i;

	i = current_group + 1;
	while (i < groups)
	{
		// o primeiro grupo pode ter 1, 2 ou 3 dígitos.
		// os demais sempre têm 3.
		if (i == 0)
			len = first_len;
		else
			len = 3;
		start = first_len + ((i - 1) * 3);
		if (i == 0)
			start = 0;
		chunk = chunk_from_number(num, start, len);
		if (!chunk)
			return (0);
		if (!is_chunk_zero(chunk))
			return (free(chunk), 1);
		free(chunk);
		i++;
	}
	return (0);
}

// print_scale_word pega a escala do grupo atual.
//
// ex:
// se group_index == 2
// make_scale_key cria "1000000"
// depois procura isso no dicionário
// e imprime "million", "milhão", etc.
static int	print_scale_word(t_dict *dict, int size, int group_index, int *first)
{
	char	*scale_key;
	char	*scale_word;

	scale_key = make_scale_key(group_index);
	if (!scale_key)
		return (0);
	scale_word = dict_get_value(dict, size, scale_key);
	free(scale_key);
	if (!scale_word)
		return (0);
	print_word(scale_word, first);
	return (1);
}

// convert_number é o coração do programa.
//
// estratégia:
// 1. se for "0", imprime zero direto
// 2. divide o número em grupos de 3
// 3. converte cada grupo separadamente
// 4. depois de cada grupo não-zero, imprime a escala correspondente
//
// ex:
// 1234567
// -> "1" | "234" | "567"
// -> one million two hundred... thousand five hundred...
int	convert_number(t_dict *dict, int size, char *num, t_lang lang)
{
	int		groups;
	int		first_len;
	int		i;
	int		start;
	int		len;
	int		first;
	char	*chunk;
	int		skip_chunk;

	if (ft_strcmp(num, "0") == 0)
		return (ft_putstr(dict_get_value(dict, size, "0")), 1);
	groups = group_count(num);
	// first_len é o tamanho do primeiro grupo.
	// se o número não encaixa certinho em blocos de 3,
	// o primeiro bloco sobra com 1 ou 2 dígitos.
	first_len = ft_strlen(num) % 3;
	if (first_len == 0)
		first_len = 3;
	i = 0;
	start = 0;
	// first controla se já imprimi alguma palavra.
	// isso evita espaço sobrando no começo da saída.
	first = 1;
	while (i < groups)
	{
		len = 3;
		if (i == 0)
			len = first_len;
		chunk = chunk_from_number(num, start, len);
		if (!chunk)
			return (0);
		if (!is_chunk_zero(chunk))
		{
			skip_chunk = 0;
			// regra especial do português:
			// 1000 -> "mil"
			// e não "um mil"
			//
			// então se o chunk for "1" e a escala for mil,
			// eu pulo a impressão do chunk e deixo só a escala sair.
			if (lang == LANG_PT && ft_strcmp(chunk, "1") == 0 && groups - i - 1 == 1)
				skip_chunk = 1;
			if (!skip_chunk && !convert_chunk(dict, size, chunk, &first, lang))
				return (free(chunk), 0);
			if (groups - i - 1 > 0 && !print_scale_word(dict, size, groups - i - 1, &first))
				return (free(chunk), 0);
			// vírgula em inglês para deixar mais natural entre escalas grandes.
			// só adiciona se ainda existir grupo não-zero depois.
			if (lang == LANG_EN && groups - i - 1 > 0
				&& has_nonzero_group_after(num, i, groups, first_len))
				print_sep(",");
		}
		free(chunk);
		start += len;
		i++;
	}
	return (1);
}