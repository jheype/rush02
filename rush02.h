#ifndef RUSH02_H
# define RUSH02_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

// t_dict é a estrutura central do dicionário.
// cada entrada guarda:
// key   -> o número em forma de string ("1000")
// value -> a palavra correspondente ("thousand", "mil", etc)
//
// usar string na key ao invés de int é essencial aqui,
// porque o subject exige suporte a números maiores que unsigned int.
// então o programa inteiro pensa o número como texto.
typedef struct s_dict
{
	char	*key;
	char	*value;
}t_dict;

// t_buf é uma estrutura auxiliar de buffer dinâmico.
//
// data -> ponteiro para o bloco da heap
// len  -> quantos bytes úteis já estão escritos
// cap  -> capacidade total atual do buffer
//
// esse tipo de struct é útil quando quero crescer conteúdo dinamicamente,
// sem ficar passando muitos parâmetros separados.
typedef struct s_buf
{
	char	*data;
	int		len;
	int		cap;
}t_buf;

// t_lang representa o idioma detectado pelo programa.
// isso permite que o mesmo motor principal de conversão
// use regras de formatação diferentes dependendo do dicionário.
//
// LANG_EN -> inglês
// LANG_PT -> português
// LANG_FR -> francês
typedef enum e_lang
{
	LANG_EN,
	LANG_PT,
	LANG_FR
}t_lang;

// main organiza o fluxo principal do programa:
// ler args, carregar dicionário, detectar idioma e converter.
int			main(int argc, char **argv);

// valida se a string recebida é um número positivo válido.
// aqui o código não aceita sinais, ponto, letras etc.
int			is_valid_number(char *str);

// normaliza o número removendo zeros à esquerda inúteis.
// ex:
// "00042" -> "42"
// "0000"  -> "0"
char		*normalize_number(char *str);

// lê o arquivo do dicionário inteiro para a heap e devolve uma string grande.
char		*read_file(char *filename);

// faz o parse do conteúdo bruto do dicionário.
// transforma o texto do arquivo num array de t_dict.
t_dict		*parse_dict(char *content, int *size);

// procura uma key específica dentro do array de dicionário.
// ex: procurar "1000" e receber "thousand".
char		*dict_get_value(t_dict *dict, int size, char *key);

// verifica se o dicionário tem pelo menos o conjunto mínimo de chaves base.
// isso evita tentar converter usando um dicionário incompleto.
int			has_required_basic_keys(t_dict *dict, int size);

// detecta o idioma olhando o conteúdo do dicionário.
// por exemplo:
// "1000" -> "mil"    => português
// "1000" -> "mille"  => francês
// senão assume inglês.
t_lang		detect_lang(t_dict *dict, int size);

// converte o número inteiro (como string) para texto por extenso.
int			convert_number(t_dict *dict, int size, char *num, t_lang lang);

// converte apenas um chunk (grupo de até 3 dígitos).
// ex:
// "42"
// "105"
// "999"
int			convert_chunk(t_dict *dict, int size, char *chunk, int *first, t_lang lang);

// cria a chave da escala dinamicamente.
// ex:
// group_index 1 -> "1000"
// group_index 2 -> "1000000"
char		*make_scale_key(int group_index);

// verifica se o chunk é composto só por '0'.
// ex:
// "000" -> true
// "042" -> false
int			is_chunk_zero(char *chunk);

// extrai um pedaço da string numérica principal.
// usado para separar o número em grupos.
char		*chunk_from_number(char *num, int start, int len);

// imprime uma palavra respeitando a lógica de espaço entre termos.
// first funciona como flag para saber se já saiu algo antes.
void		print_word(char *word, int *first);

// imprime separadores literais como "-", ",", " e", " and".
void		print_sep(char *sep);

// libera toda a memória alocada para o dicionário.
void		free_dict(t_dict *dict, int size);

// lê uma linha da stdin no modo bônus.
char		*read_stdin_line(void);

// processa entrada padrão linha por linha no modo bônus.
int			process_stdin_mode(t_dict *dict, int size, t_lang lang);

// funções básicas de string reimplementadas porque nesse projeto
// não podemos depender da libc completa como num projeto comum.
int			ft_strlen(char *str);
int			ft_strcmp(char *s1, char *s2);
int			ft_strncmp(char *s1, char *s2, int n);
char		*ft_strdup(char *src);
char		*ft_substr(char *src, int start, int len);
char		*ft_strtrim_spaces(char *str);
int			ft_is_space(char c);
int			ft_is_digit(char c);
void		ft_putstr(char *str);
char		*ft_strjoin_free(char *s1, char *s2);

#endif