#include "rush02.h"

// print_word imprime uma palavra respeitando a lógica de espaçamento.
// se ainda não saiu nada, imprime direto.
// se já saiu algo antes, coloca um espaço antes da nova palavra.
//
// a variável first funciona como flag:
// 1 -> ainda não imprimi nada
// 0 -> já imprimi pelo menos uma palavra
void	print_word(char *word, int *first)
{
	if (!word)
		return ;
	if (!*first)
		write(1, " ", 1);
	ft_putstr(word);
	*first = 0;
}

// print_sep imprime conectores literais como:
// "-"
// ","
// " e"
// " and"
//
// diferente de print_word, aqui eu não quero inserir espaço automático.
// quem chama já passa o separador no formato exato que quer.
void	print_sep(char *sep)
{
	if (!sep)
		return ;
	ft_putstr(sep);
}

// print_units trata um único dígito.
// ex:
// '4' -> procura "4" no dicionário e imprime "four", "quatro", etc.
//
// se o caractere for '0', essa função não imprime nada,
// porque zero dentro de um chunk parcial normalmente significa "não existe termo aqui".
static int	print_units(t_dict *dict, int size, char c, int *first)
{
	char	key[2];
	char	*word;

	if (c == '0')
		return (1);
	key[0] = c;
	key[1] = '\0';
	word = dict_get_value(dict, size, key);
	if (!word)
		return (0);
	print_word(word, first);
	return (1);
}

// print_units_compact é parecida com print_units,
// mas aqui eu não quero o espaço automático de print_word.
//
// isso é útil em casos como:
// forty-two
// quarante-deux
//
// se eu usasse print_word depois de um hífen,
// sairia "forty- two", o que estaria errado.
static int	print_units_compact(t_dict *dict, int size, char c, int *first)
{
	char	key[2];
	char	*word;

	if (c == '0')
		return (1);
	key[0] = c;
	key[1] = '\0';
	word = dict_get_value(dict, size, key);
	if (!word)
		return (0);
	ft_putstr(word);
	*first = 0;
	return (1);
}

// print_teens trata os números de 10 a 19.
// aqui não existe decomposição "10 + 3".
// o algoritmo busca diretamente:
// "13" -> thirteen / treze / treize
static int	print_teens(t_dict *dict, int size, char *two, int *first)
{
	char	key[3];
	char	*word;

	key[0] = two[0];
	key[1] = two[1];
	key[2] = '\0';
	word = dict_get_value(dict, size, key);
	if (!word)
		return (0);
	print_word(word, first);
	return (1);
}

// print_tens_and_units_en cuida da lógica das dezenas em inglês.
//
// casos:
// 05 -> five
// 13 -> thirteen
// 40 -> forty
// 42 -> forty-two
//
// repara que em inglês, entre dezena e unidade,
// o natural é usar hífen em muitos casos.
static int	print_tens_and_units_en(t_dict *dict, int size, char *two, int *first)
{
	char	key[3];
	char	*word;

	if (two[0] == '0')
		return (print_units(dict, size, two[1], first));
	if (two[0] == '1')
		return (print_teens(dict, size, two, first));
	key[0] = two[0];
	key[1] = '0';
	key[2] = '\0';
	word = dict_get_value(dict, size, key);
	if (!word)
		return (0);
	print_word(word, first);
	if (two[1] != '0')
	{
		print_sep("-");
		return (print_units_compact(dict, size, two[1], first));
	}
	return (1);
}

// print_tens_and_units_pt cuida das dezenas em português.
//
// casos:
// 05 -> cinco
// 13 -> treze
// 40 -> quarenta
// 42 -> quarenta e dois
//
// diferente do inglês, em português a ligação padrão é " e ".
static int	print_tens_and_units_pt(t_dict *dict, int size, char *two, int *first)
{
	char	key[3];
	char	*word;

	if (two[0] == '0')
		return (print_units(dict, size, two[1], first));
	if (two[0] == '1')
		return (print_teens(dict, size, two, first));
	key[0] = two[0];
	key[1] = '0';
	key[2] = '\0';
	word = dict_get_value(dict, size, key);
	if (!word)
		return (0);
	print_word(word, first);
	if (two[1] != '0')
	{
		print_sep(" e");
		return (print_units(dict, size, two[1], first));
	}
	return (1);
}

// print_tens_and_units_fr cuida das dezenas em francês.
//
// aqui eu tratei a forma funcional para os casos que o dicionário suporta.
// há algumas irregularidades naturais no francês,
// mas essa lógica já cobre a ideia do bônus.
//
// um caso especial aqui é quando a unidade é 1 e a dezena não é 8,
// porque certas formações francesas usam "et".
// ex:
// 21 -> vingt et un
static int	print_tens_and_units_fr(t_dict *dict, int size, char *two, int *first)
{
	char	key[3];
	char	*word;

	if (two[0] == '0')
		return (print_units(dict, size, two[1], first));
	if (two[0] == '1')
		return (print_teens(dict, size, two, first));
	if (two[1] == '1' && two[0] != '8')
	{
		key[0] = two[0];
		key[1] = '0';
		key[2] = '\0';
		word = dict_get_value(dict, size, key);
		if (!word)
			return (0);
		print_word(word, first);
		print_sep(" et");
		return (print_units(dict, size, two[1], first));
	}
	key[0] = two[0];
	key[1] = '0';
	key[2] = '\0';
	word = dict_get_value(dict, size, key);
	if (!word)
		return (0);
	print_word(word, first);
	if (two[1] != '0')
	{
		print_sep("-");
		return (print_units_compact(dict, size, two[1], first));
	}
	return (1);
}

// convert_chunk_en converte um grupo de até 3 dígitos em inglês.
//
// exemplos:
// "7"   -> seven
// "42"  -> forty-two
// "101" -> one hundred and one
//
// a lógica da centena inglesa é composta:
// unidade + hundred
//
// por isso:
// 3 + hundred -> three hundred
static int	convert_chunk_en(t_dict *dict, int size, char *chunk, int *first)
{
	char	*hundred;
	char	two[3];
	int		len;

	len = ft_strlen(chunk);
	hundred = dict_get_value(dict, size, "100");
	if (!hundred)
		return (0);
	if (len == 1)
		return (print_units(dict, size, chunk[0], first));
	if (len == 2)
		return (print_tens_and_units_en(dict, size, chunk, first));
	if (chunk[0] != '0')
	{
		if (!print_units(dict, size, chunk[0], first))
			return (0);
		print_word(hundred, first);
		// em inglês britânico fica mais natural colocar "and"
		// quando existe resto depois da centena.
		if (chunk[1] != '0' || chunk[2] != '0')
			print_sep(" and");
	}
	two[0] = chunk[1];
	two[1] = chunk[2];
	two[2] = '\0';
	return (print_tens_and_units_en(dict, size, two, first));
}

// convert_chunk_pt converte um grupo de até 3 dígitos em português.
//
// aqui a centena NÃO segue a mesma lógica inglesa.
// não é "um cem".
//
// em português:
// 100 -> cem
// 101..199 -> cento e ...
// 200 -> duzentos
// 300 -> trezentos
// etc.
//
// por isso o dicionário português precisa de entradas específicas como:
// 100: cem
// 101: cento
// 200: duzentos
// 300: trezentos
static int	convert_chunk_pt(t_dict *dict, int size, char *chunk, int *first)
{
	char	key[4];
	char	two[3];
	char	*word;
	int		len;

	len = ft_strlen(chunk);
	if (len == 1)
		return (print_units(dict, size, chunk[0], first));
	if (len == 2)
		return (print_tens_and_units_pt(dict, size, chunk, first));
	if (ft_strcmp(chunk, "100") == 0)
	{
		word = dict_get_value(dict, size, "100");
		if (!word)
			return (0);
		print_word(word, first);
		return (1);
	}
	if (chunk[0] != '0')
	{
		// para 1xx eu uso a chave especial "101" como marcador de "cento".
		// isso evita sair algo torto como "um cem".
		if (chunk[0] == '1')
			word = dict_get_value(dict, size, "101");
		else
		{
			// para 2xx, 3xx, 4xx...
			// monto a chave "200", "300", "400"...
			key[0] = chunk[0];
			key[1] = '0';
			key[2] = '0';
			key[3] = '\0';
			word = dict_get_value(dict, size, key);
		}
		if (!word)
			return (0);
		print_word(word, first);
		if (chunk[1] != '0' || chunk[2] != '0')
			print_sep(" e");
	}
	two[0] = chunk[1];
	two[1] = chunk[2];
	two[2] = '\0';
	return (print_tens_and_units_pt(dict, size, two, first));
}

// convert_chunk_fr converte um grupo de até 3 dígitos em francês.
//
// a centena aqui segue um comportamento mais parecido com a inglesa:
// unidade + cent
//
// então:
// 3 + cent -> trois cent
//
// depois ele converte o resto das dezenas/unidades.
static int	convert_chunk_fr(t_dict *dict, int size, char *chunk, int *first)
{
	char	*hundred;
	char	two[3];
	int		len;

	len = ft_strlen(chunk);
	hundred = dict_get_value(dict, size, "100");
	if (!hundred)
		return (0);
	if (len == 1)
		return (print_units(dict, size, chunk[0], first));
	if (len == 2)
		return (print_tens_and_units_fr(dict, size, chunk, first));
	if (chunk[0] != '0')
	{
		if (!print_units(dict, size, chunk[0], first))
			return (0);
		print_word(hundred, first);
	}
	two[0] = chunk[1];
	two[1] = chunk[2];
	two[2] = '\0';
	return (print_tens_and_units_fr(dict, size, two, first));
}

// convert_chunk é o dispatcher.
//
// ele não converte diretamente.
// ele só escolhe qual lógica específica de idioma deve ser usada.
int	convert_chunk(t_dict *dict, int size, char *chunk, int *first, t_lang lang)
{
	if (lang == LANG_PT)
		return (convert_chunk_pt(dict, size, chunk, first));
	if (lang == LANG_FR)
		return (convert_chunk_fr(dict, size, chunk, first));
	return (convert_chunk_en(dict, size, chunk, first));
}