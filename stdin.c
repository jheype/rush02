#include "rush02.h"

// grow_line_buffer existe para resolver um problema clássico:
// eu começo lendo a linha com um buffer pequeno (64 bytes), mas a pessoa pode digitar
// um número muito maior do que isso.
// então, quando o buffer enche, eu preciso criar outro maior e copiar o conteúdo antigo pra ele.
//
// char **buf:
// eu preciso de ponteiro para ponteiro porque não quero só mexer no conteúdo da string.
// eu quero trocar o endereço que a variável buf está guardando.
// ou seja: depois do malloc novo, *buf precisa passar a apontar pro novo bloco da heap.
//
// int *cap:
// mesma ideia. eu quero dobrar a capacidade original e essa mudança precisa refletir fora da função.
static int	grow_line_buffer(char **buf, int *cap)
{
	char	*new_buf;
	int		i;
	int		old_cap;

	old_cap = *cap;
	*cap *= 2;
	new_buf = (char *)malloc(*cap);
	if (!new_buf)
		return (0);
	i = 0;
	while (i < old_cap)
	{
		// aqui eu copio byte por byte do buffer antigo para o novo.
		// a ideia é preservar tudo que já foi lido da stdin até agora.
		new_buf[i] = (*buf)[i];
		i++;
	}
	// depois da cópia, o bloco antigo não serve mais.
	// então eu libero ele para evitar vazamento de memória.
	free(*buf);
	// agora buf passa a apontar pro novo bloco maior.
	*buf = new_buf;
	return (1);
}

// read_stdin_line lê 1 linha da entrada padrão usando read.
//
// o subject do bônus pede que, quando o argumento for "-", o programa leia números
// da stdin, um por linha.
// então essa função é tipo um mini "get_next_line" simplificado.
//
// read(0, &c, 1):
// 0 é o file descriptor da entrada padrão.
// &c é o endereço onde o caractere lido vai ser guardado.
// 1 significa: leia 1 byte por vez.
//
// ler 1 byte por vez não é a forma mais performática do mundo,
// mas aqui deixa a lógica muito simples e segura para esse rush:
// eu paro exatamente quando encontro '\n' ou EOF.
char	*read_stdin_line(void)
{
	char	c;
	char	*buf;
	int		cap;
	int		len;
	int		bytes;

	// começo com 64 bytes só como chute inicial.
	// se precisar de mais, grow_line_buffer dobra o tamanho.
	cap = 64;
	len = 0;
	buf = (char *)malloc(cap);
	if (!buf)
		return (NULL);
	bytes = read(0, &c, 1);
	while (bytes > 0)
	{
		// '\n' marca o fim da linha atual.
		// eu não guardo o '\n' dentro da string final.
		if (c == '\n')
			break ;
		// len + 1 >= cap:
		// preciso garantir espaço não só pro novo caractere,
		// mas também pro '\0' final da string C.
		if (len + 1 >= cap && !grow_line_buffer(&buf, &cap))
			return (free(buf), NULL);
		// guarda o caractere lido e anda 1 posição no buffer.
		buf[len++] = c;
		bytes = read(0, &c, 1);
	}
	// se read retornou <= 0 e eu não li nenhum caractere,
	// significa fim real da entrada.
	// então devolvo NULL para o caller saber que acabou tudo.
	if (bytes <= 0 && len == 0)
		return (free(buf), NULL);
	// fecha a string no padrão C.
	buf[len] = '\0';
	return (buf);
}

// process_stdin_mode é o modo bônus:
// quando o usuário roda ./rush-02 -
// o programa entra num loop lendo uma linha por vez da stdin.
//
// fluxo:
// 1. lê uma linha
// 2. valida se é número positivo válido
// 3. normaliza (remove zeros à esquerda desnecessários)
// 4. converte
// 5. imprime resultado
// 6. repete até EOF
int	process_stdin_mode(t_dict *dict, int size, t_lang lang)
{
	char	*line;
	char	*norm;

	line = read_stdin_line();
	while (line)
	{
		// se a linha não for um número válido, segue a regra do subject:
		// imprimir Error\n
		if (!is_valid_number(line))
			ft_putstr("Error\n");
		else
		{
			// normalize_number existe para tratar coisas como:
			// "00042" -> "42"
			// "0000" -> "0"
			norm = normalize_number(line);
			// se falhar normalização ou conversão, trato como Dict Error.
			// a conversão pode falhar se faltar alguma chave necessária no dicionário.
			if (!norm || !convert_number(dict, size, norm, lang))
				ft_putstr("Dict Error\n");
			else
				ft_putstr("\n");
			free(norm);
		}
		// cada linha veio de malloc dentro de read_stdin_line,
		// então cada iteração precisa dar free nela.
		free(line);
		line = read_stdin_line();
	}
	return (1);
}