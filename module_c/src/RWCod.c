#include"../headers/RWCod.h"

int readCodFile(SFCodes** blocosSF, char* fileName)
{
    int nBlocos,symbol=0,sizeBloco,blocoAtual=0;
    FILE *fp = fopen(fileName,"r");
    char codeSF[9], c;
    int lenCodeSF = 0;

    if(!fp) return 0;

    codeSF[0] = '\0';

    fseek(fp,3,SEEK_SET);
    fscanf(fp,"%d",&nBlocos);

    *blocosSF = (SFCodes*) malloc( sizeof(SFCodes)*nBlocos );
    fseek(fp,1,SEEK_CUR); // consome @ de bytes bloco

    while(blocoAtual<nBlocos)
    {
        (*blocosSF)[blocoAtual] = initializeSFCodes();

        symbol = 0;

        fscanf(fp,"%d",&sizeBloco);
        setBytes( (*blocosSF)[blocoAtual],sizeBloco );

        fseek(fp,1,SEEK_CUR); // consome @ de inicio bloco
        while( symbol<256 ) // leitura bloco individual
        {
            c=fgetc(fp);

            if( (c==';' || c=='@') && lenCodeSF==0)
            {
                symbol++;
            }
            else if(c==';' || c=='@')
            {
                setCodeSF( (*blocosSF)[blocoAtual],symbol,codeSF );
                lenCodeSF = 0;
                codeSF[0] = '\0';
                symbol++;
            }
            else
            {
                codeSF[lenCodeSF++] = c;
                codeSF[lenCodeSF] = '\0';
            }
        }

        blocoAtual++;
    }

    fclose(fp);

    return nBlocos;
}
