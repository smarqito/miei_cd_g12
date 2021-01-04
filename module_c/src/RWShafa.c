#include"../headers/RWShafa.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
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

    *buffer = (char*) malloc( sizeof(char)*(bufferSize + 1) );
    
    if(!(*buffer)) return -1;

    read(rleFD,*buffer,bufferSize);
    close(rleFD);

    return bufferSize+1;
}

int isRleCompression(char* rleBuffer,int index,int size)
{
    return rleBuffer[index]=='{' && index+2<size && rleBuffer[index+1]=='0' && rleBuffer[index+2]=='}';
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

int encodeRle(int* bytesRead,SFCodes bloco,char* rleBuffer,int* rleIndex,char* shafaBuffer,int* shafaIndex,
                int* encodedBits,int bytesBloco,int bytesCount)
{
    int times=0;
    char *code;
    static int overByted = 0, isZeroByte = 0;
    static char c;
    

    (*rleIndex) += 3;

    if(overByted)
    {
        (*rleIndex) -= 3;
    }
    else if(rleBuffer[*rleIndex]=='{')
    {
        (*rleIndex) += 4;
        isZeroByte=1;
    }
    else
    {
        c = rleBuffer[*rleIndex];
        (*rleIndex) += 2;
        isZeroByte=0;
    }
    
    if(overByted)
    {
        times = overByted;
        overByted = 0;
    }
    else
        sscanf(rleBuffer+(*rleIndex),"%d",&times);
    
    bytesCount += times;
    
    if(bytesCount>bytesBloco)
    {
        times = bytesBloco - (bytesCount-times);
        overByted += bytesCount-bytesBloco;
    }

    *bytesRead += times;

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
    
    if(!overByted)
        while(rleBuffer[*rleIndex-1]!='}')
            (*rleIndex)++;
    
    return overByted;
}

int writeShafaFile(SFCodes* sfBlocos,int nBlocos,char* rleBuffer,int rleBytes,char* rleFile)
{
    char* shafaBuffer=NULL,*code=NULL,*filename;
    int bytesRead=0,shafaIndex,rleIndex,blocoAtual,encodedBits=0,bytesBlocoMaior,isOverByted=0;
    char c,sizeBloco[15];
    int shafFD;

    sizeBloco[0] = '\0';

    bytesBlocoMaior = getBytes(sfBlocos[0]);
    for(blocoAtual=1;blocoAtual<nBlocos;blocoAtual++)
    {
        if( getBytes(sfBlocos[blocoAtual]) > bytesBlocoMaior )
            bytesBlocoMaior = getBytes(sfBlocos[blocoAtual]);
    }

    shafaBuffer = (char*) malloc( sizeof(char)*bytesBlocoMaior*15 );
    shafaBuffer[0] = '\0';

    filename = (char*) malloc( sizeof(char)*(strlen(rleFile)+6) );
    filename[0] = '\0';
    strcat(filename,rleFile);
    strcat(filename,".shaf");

    shafFD = open(filename,O_CREAT | O_WRONLY | O_TRUNC, 0666);
    
    sprintf(sizeBloco,"@%d",nBlocos);

    write(shafFD,sizeBloco,strlen(sizeBloco));
    sizeBloco[0] = '\0';
    
    for(blocoAtual=0,shafaIndex=0,rleIndex=0 ; ; )
    {
        if(bytesRead==getBytes(sfBlocos[blocoAtual]))
        {
            addBitsI_bloco(sfBlocos[blocoAtual],encodedBits);

            sprintf(sizeBloco,"@%d",bytesRead,blocoAtual);
            write(shafFD,sizeBloco,strlen(sizeBloco));
            sizeBloco[0] = '\0';

            write(shafFD,shafaBuffer,shafaIndex);

            shafaBuffer[0] = '\0';
            shafaIndex = 0;

            bytesRead = encodedBits = 0;
            blocoAtual++;

            if(blocoAtual==nBlocos)
                break;
        }
        
        if( isRleCompression(rleBuffer,rleIndex,rleBytes) || isOverByted )
        {
            isOverByted = encodeRle(&bytesRead,sfBlocos[blocoAtual],rleBuffer,&rleIndex,
                                    shafaBuffer,&shafaIndex,&encodedBits,getBytes(sfBlocos[blocoAtual]),bytesRead);
        }
        else
        {
            encodedBits += encodeSymbol(sfBlocos[blocoAtual],rleBuffer[rleIndex],shafaBuffer,&shafaIndex);
            rleIndex++;
            bytesRead++;
        }
    }
    
    sprintf(sizeBloco,"@0");
    write(shafFD,sizeBloco,2);

    close(shafFD);
    
    free(filename);
    free(shafaBuffer);

    return 1;
}
