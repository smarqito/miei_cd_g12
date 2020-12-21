#include "../headers/SFCodes.h"

int getIndexOfSymbol(char c)
{
    int index = (int) c;

    if(c>=0) return c;

    return c*(-1) + 2*(128+c);
}

char getSymbolOfIndex(int index)
{
    char c;

    if(index<=127) return (char) index;

    return (char) (index-256);
}

SFCodes initializeSFCodes()
{
    int i;
    SFCodes bloco = (SFCodes) malloc(sizeof(struct sfc));

    bloco->codes = (char**) malloc( sizeof(char*)*256 );
    
    for(i=0;i<256;i++)  
        bloco->codes[i] = NULL;

    bloco->nBytes = bloco->nBits_compressed = 0;

    return bloco;
}

SFCodes freeSFCodes(SFCodes bloco)
{
    int i;

    for(i=0;i<256;i++)
        free(bloco->codes[i]);
    
    free(bloco);

    return NULL;
}

void setBytes(SFCodes bloco, int bytes)
{ 
    bloco->nBytes = bytes;
}

int getBytes(SFCodes bloco)
{ 
    return bloco->nBytes;
}

char* getCodeSF(SFCodes bloco, char c)
{
    return bloco->codes[getIndexOfSymbol(c)];
}

void setCodeSF(SFCodes bloco, int index, char* code)
{
    bloco->codes[index] = strdup(code);
}

void addBits_bloco(SFCodes bloco, char* code)
{
    bloco->nBits_compressed += strlen(code);
}

int getBits_bloco(SFCodes bloco)
{
    return bloco->nBits_compressed;
}

float getCompression_bloco(SFCodes bloco)
{
    return bloco->nBits_compressed / (bloco->nBytes * 8.0);
}
