#include"./headers/RWCod.h"

int main()
{
    SFCodes* blocos;
    int nBlocos,i,j;
    int c;
    char *strB1,*strB2;

    puts("\nTESTE LEITURA FICHEIRO COD");

    nBlocos = readCodFile(&blocos,"../exemploFicheiros/aaa/aaa.txt.cod");

    printf("\nNumero de blocos %d\n",nBlocos);
    printf("Bytes Bloco 1 %d\n",getBytes(blocos[0]));
    printf("Bytes Bloco 2 %d\n\n",getBytes(blocos[1]));

    for(i=0;i<256;i++)
    {

        strB1 = blocos[0]->codes[i];
        strB2 = blocos[1]->codes[i];

        if(!strB1 && !strB2) continue;

        printf("ASCII Nº %d (char %c) com cod binaria SF:\nBloco1-> ",i,getSymbolOfIndex(i));

        if(strB1) printf("%s",strB1);
        printf("\nBloco2-> ");
        if(strB2) printf("%s",strB2);
        puts("\n");
    }

    return 0;
}
