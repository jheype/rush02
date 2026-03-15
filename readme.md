# rush02

## Fase 1:
O usuário digita no terminal algo como:

```bash
./rush-02 "42"
```

ou

```bash
./rush-02 "dicionario.dict" "42"
```

O sistema operacional cria o processo, reserva memória RAM para ele e empurra essas strings para dentro do programa através do `argv`.

A função `main` acorda.

A primeira atitude dela é levantar os escudos e fazer a triagem inicial: verificar se chegaram exatamente **2 ou 3 argumentos**.

- Se vier **2 argumentos**, ela entende que o usuário quer usar o dicionário padrão.
- Se vier **3 argumentos**, ela entende que o primeiro é o dicionário customizado e o segundo é o número.
- Se vier qualquer outra coisa, ela já bloqueia na entrada, imprime `"Error\n"` e encerra o processo ali mesmo.

---

## Fase 2:
Com a quantidade de argumentos validada, o `main` foca na string do número.

Ele chama `is_valid_number`, que faz uma varredura caractere por caractere na string recebida.  
Essa função funciona como uma blitz: ela confere se **cada byte da string está entre `'0'` e `'9'`**.

Se aparecer qualquer coisa fora disso, como:

- letra
- sinal
- ponto
- espaço
- qualquer símbolo estranho

a função retorna falha, o `main` imprime `"Error\n"` e o programa morre ali.

Se passar pela fiscalização, o `main` chama `normalize_number`.

O papel dessa função é arrancar os zeros inúteis da esquerda, porque internamente o programa não quer trabalhar com lixo visual como:

```
00042
```

Ele quer:

```
42
```

Então `normalize_number` faz um `malloc`, aloca um terreno novo na heap, copia só a parte útil da string e devolve para o `main` um ponteiro limpo chamado `norm`.

Exemplo:

```
"00042" -> "42"
"0000"  -> "0"
```

---

## Fase 3:
Agora começa a fase de engolir o dicionário.

O `main` chama `read_file`, e essa função vai conversar direto com o kernel usando chamadas de sistema:

- `open`
- `read`
- `close`

Primeiro ela usa `open` para abrir o arquivo do dicionário.

Como o programa não sabe antecipadamente o tamanho desse arquivo, ele não pode simplesmente reservar um espaço fixo e torcer para caber. Então ele cria um **buffer dinâmico na heap**.

A lógica é:

- começa com uma capacidade inicial
- vai lendo pedaços do arquivo com `read`
- vai jogando esses bytes para dentro do buffer
- se o buffer ameaçar lotar, entra a função de crescimento

Essa função de crescimento faz o seguinte:

1. dobra a capacidade antiga  
2. faz um novo `malloc`  
3. copia os bytes do buffer velho para o novo  
4. dá `free` no bloco antigo  
5. atualiza o ponteiro para o novo terreno  

Quando o arquivo chega ao fim, `read_file` fecha o descritor com `close` e devolve uma string gigantesca contendo o dicionário inteiro cru dentro da memória.

Ou seja: agora o programa não está mais olhando para um arquivo no disco.  
Ele está olhando para um **textão inteiro já carregado na RAM**.

---

## Fase 4:
Com o dicionário bruto inteiro na memória, o `main` chama `parse_dict` para transformar esse caos textual em uma estrutura organizada.

Aqui começa o trabalho de parser de verdade.

O `parse_dict` percorre a string inteira linha por linha.  
Cada linha tem o formato geral:

```
[chave] : [valor]
```

Então ele vai procurando:

- o começo da linha
- os dois pontos `:`
- o fim da linha `\n`

Quando acha isso, ele separa a linha em duas partes:

- a parte da esquerda vira a **key**
- a parte da direita vira o **value**

Só que antes de guardar, ele ainda passa a tesoura com `ft_strtrim_spaces`, removendo espaços inúteis antes ou depois das palavras.

Exemplo:

```
20 : twenty
```

vira internamente:

```
key   = "20"
value = "twenty"
```

Esses pares são guardados sequencialmente dentro de um **array de structs `t_dict`**.

Na memória fica algo como:

```
dict[0].key
dict[0].value

dict[1].key
dict[1].value

dict[2].key
dict[2].value
```

Antes de liberar o parse, o programa ainda roda um último check de segurança:

```
has_required_basic_keys
```

Essa função confere se o dicionário possui pelo menos o conjunto mínimo vital para o motor funcionar:

- `0` a `19`
- dezenas (`20`, `30`, `40`...)
- `100`
- `1000`

Se faltar qualquer peça essencial, o programa responde:

```
Dict Error
```

---

## Fase 5:
Agora o motor principal liga.

O `main` já tem em mãos:

- o número limpo (`norm`)
- o array de structs do dicionário (`t_dict`)
- o tamanho do dicionário

Então ele chama:

```
convert_number()
```

Essa função é o cérebro do rush.

Ela **não converte o número para int**, porque o projeto exige suportar números muito maiores que `unsigned int`.

Então a estratégia é **100% textual**.

O número é quebrado em blocos de até **3 dígitos**.

Exemplo:

```
1234567
```

vira

```
1 | 234 | 567
```

Cada bloco é convertido separadamente.

Para cada bloco, `convert_number` chama:

```
convert_chunk()
```

Essa função faz a engenharia interna do trio:

- centena
- dezena
- unidade

Buscando as palavras diretamente no dicionário.

---

## Fase 6:
Depois que o bloco foi traduzido, o programa precisa anexar a escala correta.

Exemplo:

```
1 -> million
234 -> thousand
567 -> (sem escala)
```

Para isso entra a função:

```
make_scale_key()
```

Ela cria dinamicamente uma string com:

```
'1' + N zeros
```

Exemplos:

```
1000
1000000
1000000000
```

Essa chave é usada para procurar no dicionário a palavra correspondente:

```
thousand
million
billion
```

---

## Fase 7:
Conforme `convert_chunk` vai descobrindo as palavras certas, ele não fica montando uma string final gigante na memória para só imprimir depois.

Ele já vai despejando tudo em tempo real.

Cada palavra encontrada é enviada para `print_word`.

Essa função gerencia a lógica do espaçamento:

- evita espaço sobrando no começo
- garante separação correta entre palavras
- respeita conectores como `-`, `,`, `e`, `and`, dependendo da lógica do idioma

Depois disso ela usa a chamada de sistema:

```
write()
```

para enviar os bytes diretamente para o terminal.

Ou seja: o número por extenso **vai sendo escrito na tela em tempo real conforme o algoritmo converte**.

---

## Fase 8 (Bônus – stdin):
Se o usuário rodar o programa assim:

```
./rush-02 -
```

o programa entra em **modo leitura contínua da entrada padrão**.

Ele começa a usar:

```
read(0, ...)
```

para ler números digitados no terminal **linha por linha**.

Exemplo:

```
42
forty-two
0
zero
```

O loop só termina quando o terminal recebe **EOF (Ctrl+D)**.

---

## Fase 9 (Bônus – múltiplos idiomas):
O programa também pode detectar automaticamente o idioma do dicionário.

Ele faz isso analisando a palavra associada à chave:

```
1000
```

Exemplos:

```
thousand -> inglês
mil      -> português
mille    -> francês
```

Dependendo disso, o motor muda a lógica de conectores e formatação.

---

## Fase 10 (Bônus – conectores):
O programa também suporta conectores naturais da língua.

Exemplos:

Inglês

```
forty-two
one hundred and five
1,234,567
```

Português

```
quarenta e dois
cento e cinco
```

Francês

```
quarante-deux
vingt et un
```

---

## Fase Final: limpeza da memória

Depois que a conversão termina, o `main` retoma o controle.

Nesse ponto a heap está cheia de blocos alocados:

- número normalizado
- conteúdo bruto do dicionário
- keys do dicionário
- values do dicionário
- buffers auxiliares

Então o programa inicia a faxina:

```
free(norm)
free_dict(dict)
```

Cada bloco da heap é devolvido para o sistema.

Nenhum byte fica perdido.

Sem vazamento de memória.

---

## Encerramento

Com tudo limpo, o `main` chega à última linha e retorna:

```
return 0;
```

O sistema operacional destrói o processo e libera todos os recursos restantes.

O programa morre.

Mas morre **limpo**.