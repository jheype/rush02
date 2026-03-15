// Como a alocação dinâmica (malloc) e o File Descriptor funcionam aqui?
// Diferente do Rush 01 (onde o tabuleiro era sempre 4x4 = 64 bytes cravados), 
// no Rush 02 não fazemos ideia do tamanho do arquivo .dict. Pode ter 100 bytes ou 10 Megabytes.
// Por isso, usamos a struct t_buf. Ela gerencia um "terreno" na memória heap.
// Se o terreno (buf.cap) encher, o grow_buffer é chamado para "comprar um terreno 2x maior",
// copiar os dados para lá, e devolver o terreno velho para o sistema operacional (free), 
// evitando o temido Memory Leak.

#include "rush02.h"

static int	grow_buffer(t_buf *buf)
{
	char	*new_data;
	int		i;

	buf->cap *= 2; // Dobra a capacidade atual matematicamente (ex: de 1024 para 2048)
	new_data = (char *)malloc(buf->cap); // Pede ao SO um novo bloco de memória RAM contíguo com esse novo tamanho
	if (!new_data) // Se a RAM estiver cheia e o malloc falhar, ele retorna NULL. O if captura isso e devolve 0 (erro)
		return (0);
	i = 0;
	while (i < buf->len)
	{
		new_data[i] = buf->data[i]; // Varre a memória antiga byte a byte e faz uma cópia idêntica no novo terreno
		i++;
	}
	free(buf->data); // Libera a memória para o SO, se não fizermos isso, o PC trava por falta de RAM com arquivos grandes.
	buf->data = new_data; // Atualiza o ponteiro oficial. Agora o buffer olha para a nova "mansão" de memória.
	return (1); // Sinal verde.
}

char	*read_file(char *filename)
{
	int		fd;
	int		bytes;
	t_buf	buf;

	fd = open(filename, O_RDONLY); // Invoca a System Call do Kernel para abrir o arquivo em modo apenas leitura. O Kernel devolve um "crachá" (File Descriptor)
	if (fd < 0) // Se o arquivo não existir ou não tiver permissão, o Kernel devolve um número negativo (-1).
		return (NULL);
	buf.cap = 1024; // Define o tamanho inicial do nosso terreno na memória para 1024 bytes (1 Kilobyte)
	buf.len = 0; // Quantidade de bytes reais lidos começa em 0
	buf.data = (char *)malloc(buf.cap); // Aloca fisicamente os primeiros 1024 bytes
	if (!buf.data)
		return (close(fd), NULL); // Se falhar logo de cara, devolve o crachá pro Kernel (close) e morre.
	bytes = 1; // Inicializa com 1 apenas para forçar a entrada no loop while abaixo
	while (bytes > 0)
	{
		if (buf.len + 512 >= buf.cap && !grow_buffer(&buf)) // Radar de colisão: se o que já tenho (len) + o que vou ler (512) estourar o limite (cap), eu invoco a expansão.
			return (close(fd), free(buf.data), NULL); // Se o grow_buffer falhar (falta de RAM), ele fecha o arquivo, passa o rodo no buffer e morre.
		
		// A MATEMÁTICA DO PONTEIRO: buf.data + buf.len
		// buf.data é o endereço base (ex: 1000). Se já lemos 512 bytes (buf.len = 512),
		// 1000 + 512 = 1512. O read vai injetar os novos dados a partir do endereço 1512, sem sobrescrever o passado.
		bytes = read(fd, buf.data + buf.len, 512); 
		
		if (bytes < 0) // Se der pau na leitura física do disco
			return (close(fd), free(buf.data), NULL);
		buf.len += bytes; // Pega o número real de bytes que entraram (pode ser menos de 512 se o arquivo estiver acabando) e soma no placar
	}
	close(fd); // O arquivo acabou de ser lido (read retornou 0). Devolvemos o crachá pro Kernel.
	
	if (buf.len + 1 >= buf.cap && !grow_buffer(&buf)) // O '+ 1' garante que temos espaço na RAM para colocar o \0 (caractere nulo)
		return (free(buf.data), NULL);
	buf.data[buf.len] = '\0'; // Crava o delimitador nulo exatamente no final do texto lido. Agora aquele amontoado de bytes virou uma String em C.
	return (buf.data); // Devolve o ponteiro da string gigante para o main
}