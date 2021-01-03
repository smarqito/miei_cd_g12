#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fsize.h"

// <RLE Stuff>

void rle (unsigned char *filename, char *fileStr, unsigned long blockSize){  //TODO Comments
    int i = 1;
    int sCount = 1;
    int special = 0;

    int check = 0;

    char sym = fileStr[0];
    char amount[12];
    char *newFilename = (char *) malloc(strlen(filename)+4);
    strcpy(newFilename, filename);
    strcat(newFilename, ".rle");
    FILE *rle = fopen(newFilename, "wb");  //Creating RLE File

    if (sym == '{' && fileStr[1] == '0' && fileStr[2] == '}'){
        i = 3;
        special = 1;
    }

    while (i < strlen(fileStr)) {
        if (sym == '{' && fileStr[i-1] == '{' && fileStr[i] == '0' && fileStr[i+1] == '}'){
            if (special != 1) {
                special = 1;
                sCount = 0;
            }

            while (fileStr[i-1] == '{' && fileStr[i] == '0' && fileStr[i+1] == '}') {
                sCount++;
                i += 3;
            }

            i -= 1;
        }

        if (special == 1){
            sprintf(amount, "%d", sCount);

            fwrite("{0}{0}", sizeof(unsigned char), strlen("{0}{0}"), rle);
            fwrite("{", sizeof(unsigned char), strlen("{"), rle);
            fwrite(amount, sizeof(char), strlen(amount), rle);
            fwrite("}", sizeof(unsigned char), strlen("}"), rle);

            sym = fileStr[i];
            sCount = 1;
            special = 0;
        } else if (fileStr[i] != sym && sCount > 3) {
            sprintf(amount, "%d", sCount);

            fwrite("{0}", sizeof(unsigned char), strlen("{0}"), rle);
            fwrite(&sym, sizeof(char), 1, rle);
            fwrite("{", sizeof(unsigned char), strlen("{"), rle);
            fwrite(amount, sizeof(char), strlen(amount), rle);
            fwrite("}", sizeof(unsigned char), strlen("}"), rle);

            sym = fileStr[i];
            sCount = 1;
        } else if (fileStr[i] != sym && sCount < 4) {
            for (int j = 0; j < sCount; j++)
                fwrite(&sym, sizeof(char), 1, rle);

            sym = fileStr[i];
            sCount = 1;
        } else
            sCount++;

        i++;
    }

    if (special == 1){
        sprintf(amount, "%d", sCount);

        fwrite("{0}{0}", sizeof(unsigned char), strlen("{0}{0}"), rle);
        fwrite("{", sizeof(unsigned char), strlen("{"), rle);
        fwrite(amount, sizeof(char), strlen(amount), rle);
        fwrite("}", sizeof(unsigned char), strlen("}"), rle);

        sym = fileStr[i];
        sCount = 1;
        special = 0;
    } else if (fileStr[i] != sym && sCount > 3) {
        sprintf(amount, "%d", sCount);

        fwrite("{0}", sizeof(unsigned char), strlen("{0}"), rle);
        fwrite(&sym, sizeof(char), 1, rle);
        fwrite("{", sizeof(unsigned char), strlen("{"), rle);
        fwrite(amount, sizeof(char), strlen(amount), rle);
        fwrite("}", sizeof(unsigned char), strlen("}"), rle);

        sym = fileStr[i];
        sCount = 1;
    } else if (fileStr[i] != sym && sCount < 4) {
        for (int j = 0; j < sCount; j++)
            fwrite(&sym, sizeof(char), 1, rle);

        sym = fileStr[i];
        sCount = 1;
    }

    fclose(rle);
}

// </RLE Stuff>

// <Freq Stuff>

void freqN (long long blockNum, unsigned long blockSize, long lastBlock, char* filename){
    int bNum = 1;
    unsigned char current;
    char *newFilename = (char *) malloc(strlen(filename)+5);
    strcpy(newFilename, filename);
    strcat(newFilename, ".freq");
    FILE *ogFile = fopen(filename, "rb");
    FILE *freq = fopen(newFilename, "w");

    if (lastBlock < 1024 && blockNum > 1) {
        blockNum -= 1;
        lastBlock += blockSize;
    }

    fprintf(freq, "@N@%lld", blockNum);

    while (bNum <= blockNum) {
        int symbs[255] = {0};

        if (bNum == blockNum)
            fprintf(freq, "@%ld@", lastBlock);
        else
            fprintf(freq, "@%ld@", blockSize);

        if (bNum == blockNum)
            for (int i = 1; i <= lastBlock; i++) {
                fread(&current, sizeof(unsigned char), 1, ogFile);
                symbs[current]++;
            }
        else
            for (int i = 1; i <= blockSize; i++) {
                fread(&current, sizeof(unsigned char), 1, ogFile);
                symbs[current]++;
            }

        for (int i = 0; i < 255; i++) {
            if (i == 0 || symbs[i] != symbs[i-1]) {
                fprintf(freq, "%d;", symbs[i]);
            } else {
                fprintf(freq, ";");
            }
        }

        bNum++;
    }

    fprintf(freq, "@0");

    fclose(ogFile);
    fclose(freq);
}

void freqR (unsigned char *filename, long long blockNum, unsigned long blockSize, unsigned long lastBlock){
    int  amount = 0;
    int symbs[255] = {0};

    char *newFileName = (char *) malloc(strlen(filename)+9);
    strcpy(newFileName, filename);
    strcat(newFileName, ".rle");
    FILE *RLE = fopen(newFileName, "rb");
    fseek(RLE, 0, SEEK_END);
    long fileSize = ftell(RLE);
    rewind(RLE);
    unsigned char *fileStr = malloc(fileSize+1);
    fileStr[fileSize] = '\0';
    fread(fileStr, 1, fileSize, RLE);
    fclose(RLE);

    strcat(newFileName, ".freq");
    FILE *file = fopen(newFileName, "w");

    if (lastBlock < 1024 && blockNum > 1) {
        blockNum -= 1;
        lastBlock += blockSize;
    }

    fprintf(file, "@R@%lld@", blockNum);

    for (int i = 1; i <= blockNum; i++){
        if (i == blockNum)
            blockSize = lastBlock;

        fprintf(file, "%ld@", blockSize);

        for (int j = 0; j < blockSize; ++j) {
            if (fileStr[j] == '{' && fileStr[j+1] == '0' && fileStr[j+2] == '}') {
                if (fileStr[j+3] == '{' && fileStr[j+4] == '0' && fileStr[j+5] == '}'){
                    char *num = malloc(13);
                    char *ptr;
                    int tmp = 0;

                    j += 7;

                    while (fileStr[j] != '}'){
                        num[tmp] = (char) fileStr[j];
                        tmp++;
                        j++;
                    } num[tmp] = '\0';

                    amount = strtol(num, &ptr, 10);
                    symbs[(unsigned char) '{'] += amount;
                    symbs[(unsigned char) '0'] += amount;
                    symbs[(unsigned char) '}'] += amount;

                    free(num);
                } else {
                    char *num = malloc(13);
                    char *ptr;
                    int tmp = 0;
                    unsigned char symbol = fileStr[j+3];

                    j += 5;

                    while (fileStr[j] != '}'){
                        num[tmp] = (char) fileStr[j];
                        tmp++;
                        j++;
                    } num[tmp] = '\0';

                    amount = strtol(num, &ptr, 10);
                    symbs[symbol] += amount;

                    free(num);
                }
            } else
                if (fileStr [j] != '\0')
                    symbs[fileStr[j]]++;
        }

        for (int y = 0; y < 255; y++)
            if (y == 0 || symbs[y] != symbs[y-1]) {
                fprintf(file, "%d;", symbs[y]);
            } else {
                fprintf(file, ";");
            }

        memset(symbs, 0, sizeof(symbs));
    }

    fprintf(file, "@0");

    fclose(file);
}

// </Freq Stuff>

void *moduleF(char bSize, int forceRLE, unsigned char *filename){
    clock_t begin = clock();

    unsigned long blockSize;

    switch(bSize){  //Handle block size input
        case 'K':
            blockSize = 655360;
            break;
        case 'm':
            blockSize = 8388608;
            break;
        case 'M':
            blockSize = 67108864;
            break;
        default:
            blockSize = 65536;
            break;
    }

    FILE *file = fopen(filename, "rb");

    if (file != NULL) {
        // Main Vars
        long last_Block_Size = 0;
        long long n_blocks = fsize(NULL, filename, &blockSize, &last_Block_Size);
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);
        char *fileStr = malloc(fileSize+1);
        fileStr[fileSize] = '\0';

        fread(fileStr, 1, fileSize, file);

        fclose(file);

        freqN(n_blocks, blockSize, last_Block_Size, filename);
        rle(filename, fileStr, blockSize);
        freqR(filename, n_blocks, blockSize, last_Block_Size);

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

        printf("[Tomás Dias, Mariana Marques, MIEI/CD, 03/01/2021]\n");
        printf("[Módulo: F]\n");
        printf("[Número de Blocos: %lld]\n", n_blocks);
        printf("[Tamanho dos blocos analisados no ficheiro original: %ld/%ld]\n", blockSize, last_Block_Size);
        printf("[Ficheiro RLE: %s.rle (?%% compressão)]\n", filename);
        printf("[Tempo de Execução: %f ms]\n", time_spent);
        if (forceRLE == 1) {
            printf("[tamanhos de todos os blocos processados no ficheiro RLE: ?/?]\n");
            printf("[Ficheiros Gerados: %s.rle, %s.freq, %s.rle.freq]\n", filename);
        }

    } else
        printf("File not found");
}
