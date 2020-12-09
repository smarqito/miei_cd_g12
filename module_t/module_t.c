#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "module_t.h"

void showAsciiFreq(AsciiFreq tabFreq)
{
    if (tabFreq)
    {
        printf("Simbolo: %c\nFrequência: %d\n", tabFreq->ascii_valor, tabFreq->ascii_freq);
        printf("Representa: %s\n", tabFreq->representa);
        printf("-----\n");
        showAsciiFreq(tabFreq->prox);
    }
}

AsciiFreq snoc (AsciiFreq sfreq, int symbolID, int symbolFreq)
{
    AsciiFreq *sitio, *ant = NULL;
    sitio = &sfreq;
    AsciiFreq novo = malloc(sizeof(Nodo));

    while (*sitio && (*sitio)->ascii_freq > symbolFreq)
    {
        sitio = &((*sitio)->prox);
    }

    novo->ascii_valor = symbolID;
    novo->ascii_freq = symbolFreq;
    novo->representa[0] = '\0';
    novo->prox = (*sitio);
    (*sitio) = novo;

    return sfreq;
}

// percorrer ficheiro até primeira ocorrência de dividider

char *seekFromFile(FILE *fp, char divider)
{
    int i;
    char *temp = malloc(50 * sizeof(char)), t;
    for (i = 0; (t = fgetc(fp)) != divider; i++)
    {
        temp[i] = t;
    }
    temp[i] = '\0';
    return temp;
}

valoresGrupo divideStruct (AsciiFreq sfreq, int totalSimbolos) {
    int contador = 0, e;
    int min=INT_MAX, temp;
    AsciiFreq pt = NULL;
    valoresGrupo valores;// = malloc(sizeof (valoresGrupo));

    for(pt = sfreq, e=0; pt ; pt = pt->prox, e++) {
        temp = abs(contador + pt->ascii_freq - (totalSimbolos / 2));
        if(temp < min) {
            contador += pt->ascii_freq;
            min = temp;
        } else {
            break;
        }
    }

    valores.nElementos = e;
    valores.nSimbolos = contador;
    return valores;
}

AsciiFreq representa (AsciiFreq sfreq, char valor, int div) {
    AsciiFreq pt = sfreq;
    int i;
    while(div>0) {
        for(i = 0; pt->representa[i]; i++);
        pt->representa[i] = valor;
        pt->representa[i+1] = '\0';
        pt = pt->prox;
        div--;
    }
    return pt;
}

void encode (AsciiFreq sfreq, int div, int totalSimbolos) {
    valoresGrupo d;
    AsciiFreq pt;
    if(div > 1) {
        d = divideStruct(sfreq, totalSimbolos);
        pt = representa(sfreq,'0',d.nElementos);
        encode(sfreq, d.nElementos, d.nSimbolos);
        representa(pt,'1',div - d.nElementos);
        encode(pt,div-d.nElementos,totalSimbolos-d.nSimbolos);
    }
}

int moduloT (char *fileName)
{
    int i;
    int symbolFreq;
    char t;
    int rle, nBlocos, size;
    int simbolosNaoNulos = 0;
    AsciiFreq tabFreq = NULL;
    rle = size = 0;
    FILE *fp = fopen(fileName, "r");
    fseek(fp,1,1); //avança um byte no ficheiro ('@)
    rle = seekFromFile(fp, '@')[0] == 'R' ? 1 : 0;
    nBlocos = atoi(seekFromFile(fp, '@'));
    size = atoi(seekFromFile(fp, '@'));
    // está na posição @<R|N>@[*]@[*]@fp
    if (!rle)
    {
        for (i = 0; i < 256; i++)
        {
            symbolFreq = atoi(seekFromFile(fp, ';'));
            if (symbolFreq)
            {   
                simbolosNaoNulos++;
                tabFreq = snoc(tabFreq, i, symbolFreq);
                // printf("indice %c valor %d\n", i, symbolFreq);
            }
        }
        encode(tabFreq, simbolosNaoNulos, size);
        printf("::::::::::::::::::::::::::::::\n");
        showAsciiFreq(tabFreq);
        printf("::::::::::::::::::::::::::::::\n");
    }
    else
    {
        //outra merda
    }
    fclose(fp);
    return 1;
}