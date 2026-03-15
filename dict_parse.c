// Como um array de Structs (t_dict) funciona na memória?
// O t_dict guarda duas strings: char *key (o número) e char *value (a palavra).
// O read_file nos deu uma string de texto gigante contínua. 
// O objetivo aqui é varrer essa string gigante procurando os '\n' (quebras de linha) e os ':' (dois pontos).
// Nós fatiamos esse texto em pedaços menores (usando malloc para cada chave e valor)
// e guardamos os endereços (ponteiros) nessas gavetas (array de structs t_dict).

#include "rush02.h"

static int	count_valid_lines(char *content)
{
	int	i;
	int	count;
	int	has_text;

	i = 0;
	count = 0;
	has_text = 0;
	while (content[i]) // Iteração clássica: varre a string inteira procurando o \0 final
	{
		if (content[i] != '\n') // Se encontrou algo que não é um enter, avisa que a linha atual tem conteúdo real
			has_text = 1;
		if (content[i] == '\n')
		{
			if (has_text) // Se bateu no enter E tinha texto nela, conta +1 linha válida
				count++;
			has_text = 0; // Reseta o rastreador para a próxima linha
		}
		i++;
	}
	if (has_text) // Verifica se a última linha do arquivo não tinha \n no final, mas tinha texto
		count++;
	return (count); // Retorna quantas linhas úteis (sem ser linhas em branco) existem. Isso dita o tamanho do array de structs!
}

static int	append_entry(t_dict *dict, int *idx, char *key, char *value)
{
	dict[*idx].key = key; // Guarda o ponteiro da chave limpa na gaveta atual (idx) do array de structs
	dict[*idx].value = value; // Guarda o ponteiro do valor limpo na mesma gaveta
	(*idx)++; // Incrementa o índice desreferenciando o ponteiro (*idx), para apontar para a próxima struct livre no próximo ciclo
	return (1);
}

static int	parse_line(char *line, t_dict *dict, int *idx)
{
	int		colon;
	char	*key_raw;
	char	*val_raw;
	char	*key;
	char	*value;
	int		i;

	colon = -1;
	i = 0;
	while (line[i])
	{
		if (line[i] == ':') // Procura a exata posição na memória onde está o caractere separador ':'
		{
			if (colon != -1) // Se já tinha achado um ':' antes nessa linha, é erro de sintaxe do arquivo. (Ex: "10: ten: erro")
				return (0);
			colon = i; // Grava a posição (índice) do separador
		}
		i++;
	}
	if (colon == -1) // Se leu a linha inteira e não achou os dois pontos, o dicionário tá mal formatado
		return (0);
		
	// ft_substr cria novas strings alocando memória (malloc). 
	// Do índice 0 até o colon é a chave. Do colon + 1 até o final é o valor.
	key_raw = ft_substr(line, 0, colon);
	val_raw = ft_substr(line, colon + 1, ft_strlen(line) - colon - 1); 
	
	if (!key_raw || !val_raw)
		return (free(key_raw), free(val_raw), 0);
		
	// O dicionário pode ter espaços lixo (Ex: " 10  :  ten "). 
	// ft_strtrim_spaces passa a tesoura e cria uma NOVA string cravada ("10" e "ten")
	key = ft_strtrim_spaces(key_raw);
	value = ft_strtrim_spaces(val_raw);
	
	// Como geramos strings novas e limpas, o espaço das strings raw sujas não serve mais. Passamos o trator:
	free(key_raw);
	free(val_raw);
	
	if (!key || !value || !key[0] || !value[0]) // Se a chave ou valor estiverem vazios depois de limpar
		return (free(key), free(value), 0);
		
	i = 0;
	while (key[i])
		if (!ft_is_digit(key[i++])) // A chave DEVE ser um número. Se tiver "abc: dez", barra na hora.
			return (free(key), free(value), 0);
			
	return (append_entry(dict, idx, key, value)); // Manda salvar nas gavetas da struct
}

static int	fill_dict(char *content, t_dict *dict)
{
	int		start;
	int		end;
	int		idx;
	char	*line;

	start = 0;
	idx = 0;
	while (1)
	{
		end = start;
		while (content[end] && content[end] != '\n') // O end vai andando na RAM até bater na parede de um \n (fim de uma linha)
			end++;
		if (end > start) // Se a linha não era apenas um \n sozinho (linha em branco)
		{
			line = ft_substr(content, start, end - start); // Extrai essa linha inteira do content gigante
			if (!line || !parse_line(line, dict, &idx)) // Manda a linha para ser fatiada pelo parse_line
				return (free(line), 0); // Se o parse_line reclamar de sintaxe errada, passamos o rodo e abortamos
			free(line); // Linha já foi fatiada e salva na struct, então apagamos esse pedaço temporário da RAM
		}
		if (!content[end]) // Se o end bateu no \0 do arquivo gigante, acabou o rolê.
			break ;
		start = end + 1; // O start pula o \n e se posiciona no começo da próxima linha
	}
	return (idx); // Retorna a quantidade de gavetas (structs) que foram realmente preenchidas
}

t_dict	*parse_dict(char *content, int *size)
{
	t_dict	*dict;
	int		real_size;

	*size = count_valid_lines(content); // Conta as linhas reais para saber o tamanho exato do array
	if (*size <= 0)
		return (NULL);
		
	// ALOCAÇÃO DO ARRAY DE STRUCTS:
	// Pede memória para o número de linhas contadas * o peso em bytes de uma t_dict (que são 2 ponteiros, geralmente 16 bytes em 64-bits)
	dict = (t_dict *)malloc(sizeof(t_dict) * (*size)); 
	if (!dict)
		return (NULL);
		
	real_size = fill_dict(content, dict); // Invoca a engenharia pesada para popular esse array recém-criado
	if (!real_size)
		return (free_dict(dict, *size), NULL); // Se deu ruim, invoca o limpador geral (free_dict) que varre limpando todas as strings de chaves/valores de dentro do array
	
	*size = real_size;
	if (!has_required_basic_keys(dict, *size)) // Último check de segurança: o dicionário tem os números bases vitais pro jogo?
		return (free_dict(dict, *size), NULL); // Se não tiver, apaga a matriz inteira e cancela o programa
		
	return (dict); // Devolve o array bidimensional de structs organizadinho para o main
}