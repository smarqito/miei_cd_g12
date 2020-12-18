#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void swap (char v[], int s, int d) {
    char temp = v[d];
    v[d] = v[s];
    v[s] = temp;
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
    int i, size = 1;
    char *buffer = malloc(sizeof(char)), t; // 
    buffer[0] = '\0';
    for (i = 0; !feof(fp) && (t = fgetc(fp)) != divider ; i++)
    {
        if(i<(size-1)) {
            buffer[i] = t;
        } else {
            char *tmp = malloc(2*size*sizeof(char));
            size *= 2;
            strcpy(tmp,buffer);
            free(buffer);
            buffer = tmp;
            swap(buffer,i,i+1);
            tmp[i] = t;
            // free(buffer);
            
        }
    }
    return buffer;
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

void ordenaLista(AsciiFreq *sfreq){
    AsciiFreq ant, pt;
    int auxS, auxF;
    char *auxR; 

    for(pt = *sfreq; pt; pt = pt->prox)
        for(ant = (*sfreq)->prox; ant; ant = ant->prox)
            if(ant->ascii_valor < pt->ascii_valor){
                auxS = pt->ascii_valor;
                pt->ascii_valor = ant->ascii_valor;
                ant->ascii_valor = auxS;
                auxF = pt->ascii_freq;
                pt->ascii_freq = ant->ascii_freq;
                ant->ascii_freq = auxF;
                auxR = pt->representa;
                strcpy(ant->representa, pt->representa);
                strcpy(auxR, ant->representa);
            }           
}

int escreverT (AsciiFreq sfreq, int rle, int nBlocos, char *filename){
    int i = 0;
    FILE *fp = fopen(filename,"w");

    if(!rle){
        AsciiFreq pt = sfreq;
        ordenaLista(&pt);

        fprintf(fp,"@N@%d", nBlocos);
        
        while(i < nBlocos){
            fprintf(fp,"@%d@0", 2048);
            for(int j = 0; j < 255; j++){
                if(j == pt->ascii_valor && pt->prox == NULL)
                    fprintf(fp, ";%s;0", pt->representa);
                else if(j == pt->ascii_valor)
                    fprintf(fp, ";%s", pt->representa);
                else 
                    fprintf(fp, ";");
            }
            i++;
        }
        fprintf(fp,"@0");

    }else{
            //para rle 
    }

    fclose(fp);
    return 0;
}


int moduloT (char *fileName)
{
    int i;
    int symbolFreq;
    char t;
    int rle, nBlocos1,nBlocos2, size;
    int simbolosNaoNulos;
    AsciiFreq tabFreq;
    rle = size = 0;
    FILE *fp = fopen(fileName, "r");
    fseek(fp,1,1); //avança um byte no ficheiro ('@)
    rle = seekFromFile(fp, '@')[0] == 'R' ? 1 : 0;
    nBlocos1 = atoi(seekFromFile(fp, '@'));
    nBlocos2 = nBlocos1;
    fseek(fp,1,-1);
    // está na posição @<R|N>@[*]@[*]@fp
    if (!rle)
    {
        while(nBlocos1 > 0) {
            tabFreq = NULL;
            simbolosNaoNulos = 0;
            size = atoi(seekFromFile(fp, '@'));
            for (i = 0; i < 255; i++)
            {
                symbolFreq = atoi(seekFromFile(fp, ';'));
                if (symbolFreq)
                {   
                    simbolosNaoNulos++;
                    tabFreq = snoc(tabFreq, i, symbolFreq);
                    // printf("indice %c valor %d\n", i, symbolFreq);
                }
            }
            seekFromFile(fp,'@');
            encode(tabFreq, simbolosNaoNulos, size);
            showAsciiFreq(tabFreq);
            printf("::::::::::::::::::::::::::::::\n");
            nBlocos1--;
        }
        escreverT(tabFreq, nBlocos2, 0, "teste.txt.cod");
    }
    else
    {
        //outra merda
    }
    fclose(fp);
    return 0;
}

// { 925498353, 966498266 }