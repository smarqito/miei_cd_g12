#include"../headers/RWShafa.h"

int getRleBuffer(char* rleFile,char** buffer)
{
    int bufferSize;
    struct stat* rleInfo;
    int rleFD;

    rleFD = open(rleFile,O_RDONLY);

    if(rleFD<0) return 0;

    rleInfo = (struct stat*) malloc( sizeof(struct stat) );

    fstat(rleFD,rleInfo);
    bufferSize = rleInfo->st_size;
    free(rleInfo);

    *buffer = (char*) malloc( sizeof(char)*bufferSize + 1 );
    
    if(!(*buffer)) return -1;

    read(rleFD,*buffer,bufferSize);
    close(fp);

    return bufferSize+1;
}


int writeShafaFile(char* codFile,char* rleFile)
{
    char* rleBuffer = NULL,shafaBuffer=NULL,code;
    int rleBytes,bytesRead=0;nBlocos=0,shafaIndex,rleIndex,blocoAtual;
    SFCodes* sfBlocos = NULL;

    if(!codFile || !rleFile) return 0;

    nBlocos = readCodFile(&sfBlocos,codFile);

    if(!nBlocos) return 0;

    rleBytes = getRleBuffer(rleFile,&rleBuffer) - 1 ; // -1 cause '\0' fim de string
    
    if(!rleBytes)
    {
        for(;!nBlocos;nBlocos--)
            freeSFCodes(sfBlocos[nBlocos-1]);
        
        return -1;
    }
    
    shafaBuffer = (char*) malloc( sizeof(char)*rleBytes*8 );
    shafaBuffer[0] = '\0';

    blocoAtual = 0;
    for(shafaIndex=0,rleIndex=0 ; rleBuffer[rleIndex]!='\0' ; shafaIndex+=strlen(code), bytesRead++)    {
        if(bytesRead==getBytes(sfBlocos[blocoAtual]))
        {
            bytesRead=0;
            blocoAtual++;
        }
        
        if(rleIndex<rleBytes-2)

        code = getCodeSF(sfBlocos[blocoAtual],rleBuffer[rleIndex]);
        shafaBuffer[s]

    }


    
}
