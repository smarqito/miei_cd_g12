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

void swap(char v[], int s, int d)
{
    char temp = v[d];
    v[d] = v[s];
    v[s] = temp;
}

AsciiFreq snoc(AsciiFreq sfreq, int symbolID, int symbolFreq)
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
    for (i = 0; !feof(fp) && (t = fgetc(fp)) != divider; i++)
    {
        if (i < (size - 1))
        {
            buffer[i] = t;
        }
        else
        {
            char *tmp = malloc(2 * size * sizeof(char));
            size *= 2;
            strcpy(tmp, buffer);
            free(buffer);
            buffer = tmp;
            swap(buffer, i, i + 1);
            tmp[i] = t;
            // free(buffer);
        }
    }
    return buffer;
}

valoresGrupo divideStruct(AsciiFreq sfreq, int totalSimbolos)
{
    int contador = 0, e;
    int min = INT_MAX, temp;
    AsciiFreq pt = NULL;
    valoresGrupo valores; // = malloc(sizeof (valoresGrupo));

    for (pt = sfreq, e = 0; pt; pt = pt->prox, e++)
    {
        temp = abs(contador + pt->ascii_freq - (totalSimbolos / 2));
        if (temp < min)
        {
            contador += pt->ascii_freq;
            min = temp;
        }
        else
        {
            break;
        }
    }

    valores.nElementos = e;
    valores.nSimbolos = contador;
    return valores;
}

AsciiFreq representa(AsciiFreq sfreq, char valor, int div)
{
    AsciiFreq pt = sfreq;
    int i;
    while (div > 0)
    {
        for (i = 0; pt->representa[i]; i++)
            ;
        pt->representa[i] = valor;
        pt->representa[i + 1] = '\0';
        pt = pt->prox;
        div--;
    }
    return pt;
}

void encode(AsciiFreq sfreq, int div, int totalSimbolos)
{
    valoresGrupo d;
    AsciiFreq pt;
    if (div > 1)
    {
        d = divideStruct(sfreq, totalSimbolos);
        pt = representa(sfreq, '0', d.nElementos);
        encode(sfreq, d.nElementos, d.nSimbolos);
        representa(pt, '1', div - d.nElementos);
        encode(pt, div - d.nElementos, totalSimbolos - d.nSimbolos);
    }
}

char **listaAscii(AsciiFreq sfreq)
{
    char **ordenada;
    int i;
    ordenada = malloc(255 * sizeof(char *));
    while (sfreq)
    {
        ordenada[sfreq->ascii_valor] = sfreq->representa;
        sfreq = sfreq->prox;
    }
    return ordenada;
}

int escreveFicheiro(AsciiFreq sfreq, int totalFrequencias, char *fileName)
{
    FILE *fp = fopen(fileName, "a");
    int i, r = 0;
    char **ordenada = listaAscii(sfreq);
    if (fp)
    {
        fprintf(fp, "@%d@0", totalFrequencias);
        for (i = 0; i < 255; i++)
        {
            // printf("%s", ordenada[i]);
            ordenada[i] ? fprintf(fp, "%s%c", ordenada[i], ';')
                        : fprintf(fp, "%c", ';');
        }
        r = 1;
        fclose(fp);
    }
    return r;
}

int initFile(char *fileName, int rle,int nBlocos)
{
    FILE *fp = fopen(fileName, "w");
    int r = 0;
    if (fp)
    {
        fprintf(fp, "@%c@%d", rle ? 'R' : 'N', nBlocos);
        fclose(fp);
        r=1;
    }
    return r;
}

int moduloT(char *fileName)
{
    int i;
    int symbolFreq;
    char t;
    int rle, nBlocos, nBlocosInit, size;
    int simbolosNaoNulos, totalFrequencias;
    AsciiFreq tabFreq;
    rle = size = 0;
    FILE *fp = fopen(fileName, "r");
    fseek(fp, 1, 1); //avança um byte no ficheiro ('@)
    rle = seekFromFile(fp, '@')[0] == 'R' ? 1 : 0;
    nBlocos = nBlocosInit = atoi(seekFromFile(fp, '@'));
    fseek(fp, 1, -1);
    // está na posição @<R|N>@[*]@[*]@fp
    if (!rle)
    {
        while (nBlocos > 0)
        {
            tabFreq = NULL;
            simbolosNaoNulos = 0;
            size = atoi(seekFromFile(fp, '@'));
            for (i = 0, totalFrequencias = 0; i < 255; i++)
            {
                symbolFreq = atoi(seekFromFile(fp, ';'));
                if (symbolFreq)
                {
                    totalFrequencias += symbolFreq;
                    simbolosNaoNulos++;
                    tabFreq = snoc(tabFreq, i, symbolFreq);
                    // printf("indice %c valor %d\n", i, symbolFreq);
                }
            }
            // printf("Total de frequencias: %d", totalFrequencias);
            seekFromFile(fp, '@');
            encode(tabFreq, simbolosNaoNulos, size);
            // showAsciiFreq(tabFreq);
            // printf("::::::::::::::::::::::::::::::\n");
            if (nBlocosInit == nBlocos)
                initFile("ficheiro.txt.cod", rle, nBlocosInit);
            escreveFicheiro(tabFreq, totalFrequencias, "ficheiro.txt.cod");
            nBlocos--;
        }
    }
    else
    {
        //outra merda
    }
    fclose(fp);
    return 0;
}

// { 925498353, 966498266 }