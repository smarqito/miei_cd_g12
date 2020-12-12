
/** 
 * @file
 * @brief Definição das funções utilizadas no Módulo B/T 
 */

#ifndef MODULE_T
#define MODULE_T

/**
 * @struct SAsciiFreq
 * @brief Informações sobre os símbolos no ficheiro
 *        Nodo -> sturct SAsciiFreq 
 */

/**
 * @typedef AsciiFreq 
 * @brief Apontador para uma lista com os dados de um símbolo
 */ 
typedef struct SAsciiFreq {
    int ascii_valor; //!< @var ascii_valor Valor em decimal do símbolo lido
    int ascii_freq; //!< @var ascii_freq Valor da frequência desse símbolo
    char representa[10]; //!< @var representa Array para gurdar a codificação do símbolo
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
 * @param s/d Inteiros cuja posição vai ser trocada 
 */
void swap (char v[], int s, int d);

/**
 * @brief Função que atualiza a lista ligada com a informação de um novo símbolo
 *        Atualiza os campos 'ascii_valor' e 'ascii_freq' para um determinado símbolo
 *        Inicia a string 'representa' com o caractér terminador de strings - '\0' (para depois ser trocado pelo código)
 *        Organiza a lista ligada de modo decrescente das frequências
 * 
 * @param sfreq Apontador para a lista ligada
 * @param symbolID Inteiro com o o valor do símbolo lido em decimal (segundo a tabela ascii)
 * @param symbolFreq Inteiro com a frequência do símbolo 
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
 *        Com a lista ordenada (de modo decrescente) indica onde será feita a divisão
 * 
 * @param sfreq Apontador para a lista ligada
 * @param totalSimbolos Número total de símbolos neste ficheiro(ou no bloco atual caso seja mais que 1) 
 * 
 * @return valores Apontador para uma lista com as informações da divisão
 */

valoresGrupo divideStruct (AsciiFreq sfreq, int totalSimbolos);

/**
 * @brief Função que coloca na em representa um determinado caractér
 *        Usada no 'encode' para calcular o código final de cada um dos símbolos 
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
 *        Segue a codificação de Shannon-Fano
 * 
 * @param sfreq Apontador para a lista ligada
 * @param div Índice da representação da divisão
 * @param totalSimbolos Número total de símbolos neste ficheiro
 */
void encode (AsciiFreq sfreq, int div, int totalSimbolos);

/**
 * @brief Percorre o ficheiro e calcula a representação de cada símbolo (para cada frequência)
 *        Considerando se tem 1 ou mais blocos 
 *        Coloca em representa (string) de cada lista ligada o respetivo código do símbolo
 * 
 * @param fileName Ficheiro do tipo .freq ou .rle.freq que vai ser lido 
 * 
 * @return 0, caso tudo corra bem 
 */ 

int moduloT (char *fileName);

#endif