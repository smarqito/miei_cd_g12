/** 
 * @file
 * @brief Definição das funções utilizadas no Módulo A/F;
 */

#ifndef MODULE_F
#define MODULE_F

/**
 * @brief Função que verifica se vale a pena ou não fazer a compressão;
 * 
 * @param rleSize Tamanho do ficheiro em rle;
 * @param originalSize Tamanho do ficheiro original;
 * 
 * @return Retorna 1 se for possível, 0 se contrário;
 */
int checkCompression(int rleSize, int originalSize);

/**
 * @brief Função que faz compressão rle num ficheiro de texto;
 * 
 * @param filename Apontador para a string com o nome do ficheiro;
 * @param fileStr Apontador para uma string que guarda o conteúdo do ficheiro em causa;
 * @param blockSize Identifica o tamanho em bits do bloco em causa
 * @param forceRLE Inteiro que informa se é necessário reforçar a compressão ou não;
 * 
 * @return Retorna o tamanho do primeiro bloco;
 */
int rle (unsigned char *filename, char *fileStr, unsigned long blockSize, int forceRLE);

/**
 * @brief Função que calcula a frequência do ficheiro de texto;
 * 
 * @param blockNum Identifica o bloco em causa;
 * @param blockSize Identifica o tamanho em bits do bloco em causa;
 * @param lastBlock Identifica o tamanho em bits do último bloco;
 * @param filename Apontador para a string com o nome do ficheiro;
 */
void freqN (long long blockNum, unsigned long blockSize, long lastBlock, char *filename);

/**
 * @brief Função que calcula a frequência do ficheiro rle;
 * 
 * @param file Apontador para o ficheiro de texto;
 * @param RLE Apontador para o ficheiro em rle;
 * @param force Inteiro que nos dá permissão de forçar a compressão rle ou não;
 * @param blockNum Identifica o bloco em causa;
 * @param blockSize Identifica o tamanho em bits do bloco em causa;
 * @param lastBlock Identifica o tamanho em bits do último bloco;
 */
void freqR (FILE *file, FILE *RLE, int force, long long blockNum, unsigned long blockSize, unsigned long lastBlock);

/**
 * @brief Função principal deste módulo, onde divide os blocos em bits e mediante essa divisão, faz a compressão rle e cálcula as frequências;
 * 
 * @param bsize Identifica o tamanho em bits do bloco em causa;
 * @param forceRLE Inteiro que nos dá permissão de forçar a compressão rle ou não;
 * @param filename Apontador para a string com o nome do ficheiro;
 * 
 * @return Apontador para uma string;
 */
char *moduleF(char bSize, int forceRLE, unsigned char *filename);

#endif
