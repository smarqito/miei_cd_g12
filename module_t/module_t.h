
/** 
 * @file
 * @brief Definição das funções utilizadas no Módulo B/T 
 */

#ifndef MODULE_T
#define MODULE_T

/**
 * @struct SAsciiFreq
 * @brief Informações sobre os símbolos no ficheiro
 *        Nodo -> struct SAsciiFreq 
 */

/**
 * @typedef AsciiFreq 
 * @brief Apontador para uma lista com os dados de um símbolo
 */ 
typedef struct SAsciiFreq {
    int ascii_valor; //!< @var ascii_valor Valor em decimal do símbolo lido
    int ascii_freq; //!< @var ascii_freq Valor da frequência desse símbolo
    char *representa; //!< @var representa Array para gurdar a codificação do símbolo
    int tamanhoRepresenta;
    struct SAsciiFreq *prox; //!< @var prox Apontador para a próxima lista 
} Nodo, *AsciiFreq;

/**
 * @struct ValoresGrupo 
 * @brief Utilizada no cálculo da divisão da lista ligada com as informações dos símbolos
 *        valoresGrupo -> Alias para reduzir a dimensão 
 */
typedef struct ValoresGrupo {
    int nSimbolos; //!< @var nSimbolos Nº de símbolos total na divisão - Soma das frequências dos símbolos que serão postos a 0 (segundo a tabela Shannon-Fano)
    int nElementos; //!< @var nElementos Nº de Elementos/símbolos diferentes na divisão
} valoresGrupo;

/**
 * @brief Função auxiliar para trocar a posição de 2 elementos 
 * 
 * @param v Array de inteiros 
 * @param s Índice de origem 
 * @param d Índice de destino 
 */
void swap (char v[], int s, int d);

/**
 * @brief Insere um novo símbolo por ordem decrescente das frequências
 * 
 * @param sfreq Apontador para a lista ligada
 * @param symbolID Valor do símbolo lido em decimal (segundo a tabela ascii)
 * @param symbolFreq Frequência do símbolo 
 * 
 * @return sfreq Apontador para o ínicio da lista ligada (agora ordenada decrescentemente)    
 */
AsciiFreq snoc (AsciiFreq sfreq, int symbolID, int symbolFreq);

/*
 * @brief Recebe um apontador e coloca num array os elementos até ao divider
 * 
 * @param fp Apontador para o ficheiro a ler
 * @param divider Carater que termina a pesquisa
 * 
 * @return Apontador para array na memória com os elementos pretendidos
 */
char *seekFromFile (FILE *fp, char divider);

/**
 * @brief Função que calcula a menor divisão duma lista de frequências
 * 
 * @param sfreq Apontador para a lista ligada
 * @param totalSimbolos Número total de símbolos neste ficheiro(ou no bloco atual caso seja mais que 1) 
 * 
 * @return valores Apontador para uma lista com as informações da divisão
 */

valoresGrupo divideStruct (AsciiFreq sfreq, int totalSimbolos);

/**
 * @brief Recebe um caractér e coloca-o na string representa 
 * 
 * @param sfreq Apontador para a lista ligada
 * @param valor Caractér que vai ser colocado na String 
 * @param div Representa onde se encontra a divisão (o índice)
 * 
 * @return sfreq Apontador para o ínicio da lista ligada
 */ 
AsciiFreq representa (AsciiFreq sfreq, char valor, int div);

/**
 * @brief Função que cue calcula os códigos para cada símbolo
 * 
 * @param sfreq Apontador para a lista ligada
 * @param div Índice da representação da divisão
 * @param totalSimbolos Número total de símbolos neste ficheiro
 */
void encode (AsciiFreq sfreq, int div, int totalSimbolos);

/**
 * @brief Função que cria uma array de strings 
 * 
 * @param sfreq Apontador para a lista ligada
 * 
 * @return ordenada Array de strings 
 */
char **listaAscii(AsciiFreq sfreq);

/**
 * @brief Função que escreve o ficheiro .cod com as informações necessárias
 * 
 * @param sfreq Apontador para a lista ligada
 * @param totalFrequencias Nº de elementos total num bloco  
 * @param filename String com o nome do ficheiro .cod a ser criado
 * 
 * @return r 1 ou 0, caso tenha sido imprimido com sucesso no ficheiro a informaçao inicial 
 */
int escreveFicheiro(AsciiFreq sfreq, int totalFrequencias, char *fileName);

/**
 * @brief Imprime no ficheiro .cod as informações iniciais
 *        Imprime se é ou não RLE e o nº de blocos
 * 
 * @param filename String com o nome do ficheiro .cod a ser criado 
 * @param rle Inteiro que indica o tipo de ficheiro (RLE ou Não RLE)
 * @param nBlocos Nº de blocos do ficheiro
 * 
 * @return r 1 ou 0, caso tenha sido imprimido com sucesso no ficheiro a informaçao inicial 
 */
int initFile(char *fileName, int rle, int nBlocos);

/**
 * @brief Imprime no ficheiro .cod as informações finais (@0)
 * 
 * @param filename String com o nome do ficheiro .cod a ser criado 
 */
void endFile(char *filenmame);

/**
 * @brief Percorre o ficheiro e calcula a representação de cada símbolo (para cada frequência)
 * 
 * @param fileName Ficheiro do tipo .freq ou .rle.freq que vai ser lido 
 * 
 * @return 0, caso tudo corra bem 
 */ 
char* moduloT (char *fileName);

#endif