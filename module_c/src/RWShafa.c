#include"../headers/RWShafa.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

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
    close(rleFD);

    return bufferSize+1;
}

int isRleCompression(char* rleBuffer,int index,int size)
{
    return rleBuffer[index]=='{' && index+2<size && rleBuffer[index+2]=='}';
}

int encodeSymbol(SFCodes bloco,char symbol,char* shafaBuffer,int* shafaIndex)
{
    char* code = NULL;
    int bitLen = 0;

    shafaBuffer[(*shafaIndex)++] = '(';
    shafaBuffer[*shafaIndex] = '\0';
    
    code = getCodeSF(bloco,symbol);
    strcpy(shafaBuffer+(*shafaIndex),code);
            
    bitLen = strlen(code);
    *shafaIndex+=bitLen;
    
    shafaBuffer[(*shafaIndex)++] = ')';  
    shafaBuffer[*shafaIndex] = '\0';

    return bitLen;
}

int encodeRle(SFCodes bloco,char* rleBuffer,int* rleIndex,char* shafaBuffer,int* shafaIndex,int* encodedBits)
{
    int times=0,isZeroByte=0,bytesRead;
    char c,*code;

    (*rleIndex) += 3;

    if(rleBuffer[*rleIndex]=='{')
    {
        (*rleIndex) += 4;
        isZeroByte=1;
    }
    else
    {
        c = rleBuffer[*rleIndex];
        (*rleIndex) += 2;
    }

    sscanf(rleBuffer+(*rleIndex),"%d",&times);
    bytesRead = times;

    while(times)
    {
        if(isZeroByte)
        {
            shafaBuffer[(*shafaIndex)++] = '(';
            shafaBuffer[(*shafaIndex)++] = '0';
            shafaBuffer[(*shafaIndex)++] = '0';
            shafaBuffer[(*shafaIndex)++] = '0';
            shafaBuffer[(*shafaIndex)++] = '0';
            shafaBuffer[(*shafaIndex)++] = '0';
            shafaBuffer[(*shafaIndex)++] = '0';
            shafaBuffer[(*shafaIndex)++] = '0';
            shafaBuffer[(*shafaIndex)++] = '0';
            shafaBuffer[(*shafaIndex)++] = ')';
            shafaBuffer[*shafaIndex] = '\0';
            
            (*encodedBits) += 8;
        }
        else
        {
            (*encodedBits) += encodeSymbol(bloco,c,shafaBuffer,shafaIndex);
        }
        
        times--;
    }

    while(rleBuffer[*rleIndex-1]!='}')
        (*rleIndex)++;

    return bytesRead;
}

int writeShafaFile(char* codFile,char* rleFile)
{
    char* rleBuffer = NULL,*shafaBuffer=NULL,*code=NULL,*filename;
    int rleBytes,bytesRead=0,nBlocos=0,shafaIndex,rleIndex,blocoAtual,encodedBits=0;
    SFCodes* sfBlocos = NULL;
    char c;
    int shafFD;

    if(!codFile || !rleFile) return 0;

    nBlocos = readCodFile(&sfBlocos,codFile);

    if(!nBlocos) return -1;

    rleBytes = getRleBuffer(rleFile,&rleBuffer) - 1 ; // -1 cause '\0' fim de string
    
    if(!rleBytes)
    {
        for(;!nBlocos;nBlocos--)
            freeSFCodes(sfBlocos[nBlocos-1]);
        
        return -2;
    }
    
    shafaBuffer = (char*) malloc( sizeof(char)*rleBytes*8 );
    shafaBuffer[0] = '\0';

    for(blocoAtual=0,shafaIndex=0,rleIndex=0 ; rleBuffer[rleIndex]!='\0' ; )
    {
        if(bytesRead==getBytes(sfBlocos[blocoAtual]))
        {
            addBitsI_bloco(sfBlocos[blocoAtual],encodedBits);
            bytesRead = encodedBits = 0;
            blocoAtual++;
        }
        
        if( isRleCompression(rleBuffer,rleIndex,rleBytes) )
        {
            bytesRead += encodeRle(sfBlocos[blocoAtual],rleBuffer,&rleIndex,shafaBuffer,&shafaIndex,&encodedBits);
        }
        else
        {
            encodedBits += encodeSymbol(sfBlocos[blocoAtual],rleBuffer[rleIndex],shafaBuffer,&shafaIndex);
            rleIndex++;
            bytesRead++;
        }
    }
    
    filename = (char*) malloc( sizeof(char)*(strlen(rleFile)+1) );
    filename[0] = '\0';
    strcat(filename,rleFile);
    strcat(filename,".shaf");

    shafFD = open(filename,O_CREAT | O_WRONLY | O_TRUNC);
    write(shafFD,shafaBuffer,shafaIndex);
    close(shafFD);

    return 1;
}
