#include "rush02.h"

// dict_get_value faz busca linear no array de structs do dicionário.
//
// como o dicionário é pequeno, não vale a pena complicar com hash map.
// a estratégia aqui é:
// andar do índice 0 até size - 1
// comparar cada key com a key procurada
// quando achar, devolver o ponteiro para o value correspondente.
char	*dict_get_value(t_dict *dict, int size, char *key)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (ft_strcmp(dict[i].key, key) == 0)
			return (dict[i].value);
		i++;
	}
	return (NULL);
}

// has_key é só um helper interno.
// ele reaproveita dict_get_value para responder um booleano:
// existe ou não existe essa chave no dicionário?
static int	has_key(t_dict *dict, int size, char *key)
{
	return (dict_get_value(dict, size, key) != NULL);
}

// has_required_basic_keys verifica se o dicionário possui o núcleo mínimo
// necessário para qualquer conversão base.
//
// sem isso, o programa poderia parsear um arquivo válido em formato,
// mas inútil na prática porque faltaria algo como "7" ou "1000".
//
// mandatory guarda as chaves mais básicas:
// 0..19
// dezenas principais
// 100
// 1000
int	has_required_basic_keys(t_dict *dict, int size)
{
	char	*mandatory[] = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		"10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
		"20", "30", "40", "50", "60", "70", "80", "90", "100", "1000"
	};
	int		count;
	int		i;

	// sizeof(array) / sizeof(array[0]) é um padrão clássico em C
	// para descobrir quantos elementos fixos existem num array estático.
	count = sizeof(mandatory) / sizeof(mandatory[0]);
	i = 0;
	while (i < count)
	{
		if (!has_key(dict, size, mandatory[i]))
			return (0);
		i++;
	}
	return (1);
}

// detect_lang tenta descobrir o idioma com base na palavra da escala 1000.
//
// isso funciona porque:
// inglês    -> thousand
// português -> mil
// francês   -> mille
//
// se não reconhecer, assume inglês como fallback.
t_lang	detect_lang(t_dict *dict, int size)
{
	char	*thousand;

	thousand = dict_get_value(dict, size, "1000");
	if (!thousand)
		return (LANG_EN);
	if (ft_strcmp(thousand, "mil") == 0)
		return (LANG_PT);
	if (ft_strcmp(thousand, "mille") == 0)
		return (LANG_FR);
	return (LANG_EN);
}