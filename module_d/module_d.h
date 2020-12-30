#ifndef MODULE_D
#define MODULE_D

typedef struct sfc{
    char** codes;
    int nBytes;
    int nBite_compressed;
} *SFCodes;

int readCodFile(SFCodes **blocosSF, char *fileName);

#endif