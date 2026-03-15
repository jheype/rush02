#include "rush02.h"

// print_error centraliza a escrita das mensagens de erro.
// usei write diretamente porque o subject só autoriza um conjunto limitado de funções.
// ft_strlen entra aqui para eu saber quantos bytes escrever.
static void	print_error(char *msg)
{
	write(1, msg, ft_strlen(msg));
}

// load_dict_and_run concentra o fluxo real do programa depois que os argumentos
// já foram interpretados.
//
// ideia:
// 1. ler arquivo do dicionário
// 2. parsear conteúdo
// 3. detectar idioma
// 4. decidir se vai converter um argumento único ou entrar em modo stdin
static int	load_dict_and_run(char *dict_file, char *num)
{
	char	*content;
	t_dict	*dict;
	int		size;
	char	*norm;
	t_lang	lang;

	content = read_file(dict_file);
	if (!content)
		return (print_error("Dict Error\n"), 1);
	dict = parse_dict(content, &size);
	free(content);
	if (!dict)
		return (print_error("Dict Error\n"), 1);
	// detect_lang olha o próprio dicionário e escolhe a lógica de formatação.
	lang = detect_lang(dict, size);
	// bônus:
	// se o "número" for "-", ao invés de converter argv direto,
	// o programa passa a ler várias linhas da stdin.
	if (ft_strcmp(num, "-") == 0)
	{
		process_stdin_mode(dict, size, lang);
		free_dict(dict, size);
		return (0);
	}
	// se não for stdin mode, valida o argumento numérico normalmente.
	if (!is_valid_number(num))
		return (free_dict(dict, size), print_error("Error\n"), 1);
	norm = normalize_number(num);
	if (!norm)
		return (free_dict(dict, size), print_error("Error\n"), 1);
	// convert_number devolve 0 se faltar alguma chave necessária
	// ou se a conversão não puder ser completada com o dicionário atual.
	if (!convert_number(dict, size, norm, lang))
		print_error("Dict Error\n");
	else
		write(1, "\n", 1);
	free(norm);
	free_dict(dict, size);
	return (0);
}

// main só decide a forma de uso do programa.
//
// argc == 2:
// ./rush-02 42
//
// argc == 3:
// ./rush-02 numbers.dict 42
//
// qualquer outro caso sai como Error.
int	main(int argc, char **argv)
{
	char	*dict_file;
	char	*num;

	if (argc == 2)
	{
		// sem dicionário customizado, usa o padrão.
		dict_file = "numbers.dict";
		num = argv[1];
	}
	else if (argc == 3)
	{
		// com dois argumentos:
		// argv[1] = dicionário
		// argv[2] = número ou "-"
		dict_file = argv[1];
		num = argv[2];
	}
	else
		return (print_error("Error\n"), 1);
	return (load_dict_and_run(dict_file, num));
}