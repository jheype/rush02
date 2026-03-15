// O Caminhão de Lixo: free_dict
// A missão aqui é impedir o Memory Leak (vazamento de memória). 
// Como a RAM tá cheia de ponteiros guardados dentro das structs, a gente não pode 
// simplesmente dar um free() no array principal. Temos que descer limpando gaveta por gaveta.

#include "rush02.h"

void	free_dict(t_dict *dict, int size)
{
	int	i;

	if (!dict) // O Escudo Anti-SegFault: Se o ponteiro vier nulo (ex: se a leitura do arquivo falhou lá no início), a CPU aborta a missão. Tentar dar free em endereço vazio quebra o programa na hora.
		return ;
		
	i = 0;
	while (i < size) // A Varredura da RAM: O processador inicia o loop. Vai descer a memória batendo de porta em porta, do índice 0 até o limite exato registrado no 'size'.
	{
		// A Limpeza Cirúrgica: Antes de destruir a gaveta, a CPU entra nela e pega o endereço de memória exato da chave (ex: o terreno da string "1000").
		free(dict[i].key); 
		
		// Faz a mesma coisa com o valor (ex: o terreno da string "thousand"). Passa o trator e devolve esses micro-terrenos pro Sistema Operacional.
		free(dict[i].value); 
		
		i++;
	}
	
	// O loop acabou. Todas as strings internas foram dizimadas e as gavetas estão vazias. 
	// Só agora a CPU tem o sinal verde para dar o free final no array principal. Memória RAM 100% imaculada.
	free(dict); 
}