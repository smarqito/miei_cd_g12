#ifndef __STRUCTSFCODES__
#define __STRUCTSFCODES__

#include<stdlib.h>
#include<string.h>

// estrutura para codigos binarios para symbolos de um bloco
typedef struct sfc
{
    char** codes;
    int nBytes;
    int nBits_compressed;
} *SFCodes;

int getIndexOfSymbol(char);
char getSymbolOfIndex(int);
SFCodes initializeSFCodes(void);
SFCodes freeSFCodes(SFCodes);
void setBytes(SFCodes,int);
int getBytes(SFCodes);
char* getCodeSF(SFCodes,char);
void setCodeSF(SFCodes,int,char*);
void addBits_bloco(SFCodes,char*);
int getBits_bloco(SFCodes);
float getCompression_bloco(SFCodes);

#endif
