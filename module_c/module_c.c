#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "module_c.h"

int bitsToBytes(int bits)
{
    return bits/8;
}

char* moduleC(char* codFile,char* rleFile)
{
    
    int nBlocos=0,rleBytes=0,i,bytesTotal=0,bitsTotal=0,bytesBloco=0,bitsBloco=0;
    SFCodes* sfBlocos = NULL;
    char* rleBuffer = NULL,*filename=NULL;
    double timeSpent = 0.0;
    clock_t begin = clock(),end;

    if(!codFile || !rleFile)
        return NULL;

    rleBytes = getRleBuffer(rleFile,&rleBuffer) - 1 ;
    nBlocos = readCodFile(&sfBlocos,codFile);

    if(!rleBytes && nBlocos>0)
    {
        for(;!nBlocos;nBlocos--)
            freeSFCodes(sfBlocos[nBlocos-1]);

        return NULL;
    }

    if(!nBlocos && rleBuffer!=NULL)
    {
        free(rleBuffer);
        return NULL;
    }

    writeShafaFile(sfBlocos,nBlocos,rleBuffer,rleBytes,rleFile);
    
    end = clock();
    timeSpent = (double) (end-begin) / CLOCKS_PER_SEC;
    
    puts("Manuel Jorge Mimoso Carvalho, a69856");
    puts("Bento Rafael Vaz Costa, a93522");
    puts("\nMIEI/CD MODULO C:");
    printf("Numero de blocos %d\n",nBlocos);
    
    for(i=0;i<nBlocos;i++)
    {
        bytesBloco = getBytes(sfBlocos[i]);
        bitsBloco = getBits_bloco(sfBlocos[i]);
        bytesTotal += bytesBloco;
        bitsTotal += bitsBloco;
        printf("(Bloco %d) Tamanho antes/depois %d/%d\n",i+1,bytesBloco,bitsBloco);
    }

    printf("Compressao total %d\n%",(bitsToBytes(bitsTotal)/bytesTotal)*100);
    printf("Tempo de execuçao %lf\n",timeSpent);

    filename = (char*) malloc( sizeof(char)*(strlen(rleFile)+6) );
    filename[0] = '\0';
    strcat(filename,rleFile);
    strcat(filename,".shaf");
    
    printf("Ficheiro gerado -> %s\nEND\n\n",filename);
    
    free(rleBuffer);
    for(;!nBlocos;nBlocos--)
        freeSFCodes(sfBlocos[nBlocos-1]);

    return filename;
}
