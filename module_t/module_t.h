#ifndef MODULE_T
#define MODULE_T

/*
 * @def SAsciiFreq
 * @brief Struct para armazenar os dados sobre os símbolos
 */
typedef struct SAsciiFreq {
    int ascii_valor;
    int ascii_freq;
    char representa[10];
    
    struct SAsciiFreq *prox;
} Nodo, *AsciiFreq;

typedef struct ValoresGrupo {
    int nSimbolos; // n.º de 
    int nElementos;
} valoresGrupo;
/*
 * @brief   Recebe um apontador e coloca num array os elementos até ao divider
 * 
 * @param fp Apontador para o ficheiro a ler
 * @param divider Carater que termina a pesquisa
 * 
 * @return Apontador para array na memória com os elementos pretendidos
 */
char *seekFromFile (FILE *fp, char divider);


int moduloT (char *fileName);

#endif