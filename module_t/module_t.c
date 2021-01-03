#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "module_t.h"


void printAuthor(int blocos)
{
    printf("José Malheiro - a93271 && Marco Sousa - a62608\n");
    printf("MIEI / CD, 2020-2021\n");
    printf("Módulo: t\n");
    printf("Número de blocos: %d\n", blocos);
    printf("Tamanho dos blocos analisados no ficheiro de símbolos: ");
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
    novo->representa = malloc(sizeof(char));
    novo->representa[0] = '\0';
    novo->tamanhoRepresenta = 1;
    novo->prox = (*sitio);
    (*sitio) = novo;

    return sfreq;
}

// percorrer ficheiro até primeira ocorrência de dividider ou fim do ficheiro

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
        }
    }
    return buffer;
}

valoresGrupo divideStruct(AsciiFreq sfreq, int totalSimbolos)
{
    int contador = 0, e;
    int min = INT_MAX, temp;
    AsciiFreq pt = NULL;
    valoresGrupo valores;

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

AsciiFreq representa(AsciiFreq sfreq, char valor, int simbNaoNulos)
{
    AsciiFreq pt = sfreq;
    int i;
    while (simbNaoNulos > 0)
    {
        for (i = 0; pt->representa[i]; i++)
            ;
        if (i == (pt->tamanhoRepresenta - 1))
        {
            char *novoRepresenta = malloc(pt->tamanhoRepresenta * 2 * sizeof(char));
            strcpy(novoRepresenta, pt->representa);
            pt->representa = novoRepresenta;
        }
        pt->representa[i] = valor;
        pt->representa[i + 1] = '\0';
        pt = pt->prox;
        simbNaoNulos--;
    }
    return pt;
}

void encode(AsciiFreq sfreq, int simbNaoNulos, int totalSimbolos)
{
    valoresGrupo d;
    AsciiFreq pt;
    if (simbNaoNulos > 1)
    {
        d = divideStruct(sfreq, totalSimbolos);
        pt = representa(sfreq, '0', d.nElementos);
        encode(sfreq, d.nElementos, d.nSimbolos);
        representa(pt, '1', simbNaoNulos - d.nElementos);
        encode(pt, simbNaoNulos - d.nElementos, totalSimbolos - d.nSimbolos);
    }
}

char **listaAscii(AsciiFreq sfreq)
{
    char **ordenada;
    int i;
    ordenada = malloc(255 * sizeof(char *));

    for (i = 0; i < 255; i++) // inicializar a matriz a null
        ordenada[i] = '\0';

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
        fprintf(fp, "@%d@", totalFrequencias);
        for (i = 0; i < 255; i++)
        {
            ordenada[i] ? fprintf(fp, "%s%c", ordenada[i], ';')
                        : fprintf(fp, "%c", ';');
        }
        r = 1;
        fclose(fp);
    }
    return r;
}

int initFile(char *fileName, int rle, int nBlocos)
{
    FILE *fp = fopen(fileName, "w");
    int r = 0;
    if (fp)
    {
        fprintf(fp, "@%c@%d", rle ? 'R' : 'N', nBlocos);
        fclose(fp);
        r = 1;
    }
    return r;
}

void endFile(char *filename)
{
    FILE *fp = fopen(filename, "a");

    if (fp)
    {
        fprintf(fp, "@0");
        fclose(fp);
    }
}

int moduloT(char *fileName)
{
    double timeSpent = 0.0;
    clock_t begin = clock();
    int i, erro;
    int symbolFreq;
    char t;
    int rle, nBlocos, nBlocosInit, size;
    int simbolosNaoNulos, totalFrequencias;
    AsciiFreq tabFreq;
    rle = size = erro = 0;
    FILE *fp = fopen(fileName, "r");
    fseek(fp, 1, 1); //avança um byte no ficheiro ('@)
    rle = seekFromFile(fp, '@')[0] == 'R' ? 1 : 0;
    nBlocos = nBlocosInit = atoi(seekFromFile(fp, '@'));
    fseek(fp, 1, -1);

    // Criar o nome de output de forma dinâmica, com base no nome do ficheiro de origem
    // armazena na mesma pasta que o ficheiro de origem
    char *outputFileName = strdup(fileName);
    strcpy(outputFileName + strlen(fileName) - 4, "cod");

    printAuthor(nBlocos);

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
            }
        }
        if (size != totalFrequencias) erro = 1;

        seekFromFile(fp, '@');
        encode(tabFreq, simbolosNaoNulos, totalFrequencias);

        if (nBlocosInit == nBlocos)
            initFile(outputFileName, rle, nBlocosInit);

        escreveFicheiro(tabFreq, totalFrequencias, outputFileName);
        printf("%d%s", totalFrequencias, nBlocos == 1 ? " bytes\n" : "/");
        nBlocos--;
    }
    endFile(outputFileName);
    clock_t end = clock();
    timeSpent += (double) (end-begin) / CLOCKS_PER_SEC;

    printf("Tempo de execução: %f s\n", timeSpent);

    printf("Path para o ficheiro gerado: %s\n", outputFileName);

    if(erro){
        printf("\n:::::: Atenção! :::::: \n");
        printf("Existe pelo menos um bloco cujo tamanho difere do total de frequências dos símbolos!\n\n");
    }

    fclose(fp);

    return 0;
}