#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fsize.h"

// <RLE Stuff>

int checkCompression(int rleSize, int originalSize){
    double compression = originalSize - rleSize;
    compression = compression/originalSize;
    compression = compression*100;

    if (compression >= 5.00)
        return 1;
    else
        return 0;
}

int rle (unsigned char *filename, char *fileStr, unsigned long blockSize, int forceRLE){
    int i = 0;
    int firstBlockSize;
    int sCount = 0;
    int special = 0;
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
        if (fileStr[i] == '{' && fileStr[i+1] == '0' && fileStr[i+2] == '}'){
            if (sCount > 3) {
                sprintf(amount, "%d", sCount);

                fwrite("{0}", sizeof(unsigned char), strlen("{0}"), rle);
                fwrite(&sym, sizeof(char), 1, rle);
                fwrite("{", sizeof(unsigned char), strlen("{"), rle);
                fwrite(amount, sizeof(char), strlen(amount), rle);
                fwrite("}", sizeof(unsigned char), strlen("}"), rle);
                special = 1;
                sCount = 0;
            } else if (sCount < 4) {
                for (int j = 0; j < sCount; j++)
                    fwrite(&sym, sizeof(char), 1, rle);

                special = 1;
                sCount = 0;
            }

            while (fileStr[i] == '{' && fileStr[i+1] == '0' && fileStr[i+2] == '}') {
                sCount++;
                i += 3;
            }
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

        if (i+1 == blockSize && forceRLE != 1) {
            firstBlockSize = ftell(rle);

            if (checkCompression(firstBlockSize, blockSize) != 1) {
                fclose(rle);
                remove(newFilename);
                return 0;
            }
        }
        i++;
    }

    if (sCount > 3) {
        sprintf(amount, "%d", sCount);

        fwrite("{0}", sizeof(unsigned char), strlen("{0}"), rle);
        fwrite(&sym, sizeof(char), 1, rle);
        fwrite("{", sizeof(unsigned char), strlen("{"), rle);
        fwrite(amount, sizeof(char), strlen(amount), rle);
        fwrite("}", sizeof(unsigned char), strlen("}"), rle);

        sym = fileStr[i];
    } else if (sCount < 4) {
        for (int j = 0; j < sCount; j++)
            fwrite(&sym, sizeof(char), 1, rle);

        sym = fileStr[i];
    }

    fclose(rle);
    return firstBlockSize;
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
        if (bNum == blockNum)
            blockSize = lastBlock;

        int symbs[255] = {0};

        fprintf(freq, "@%ld@", blockSize);

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
    int bNum = 1, amount;
    unsigned char current, forwards1, forwards2;
    char *newFileName = (char *) malloc(strlen(filename)+9);
    strcpy(newFileName, filename);
    strcat(newFileName, ".rle");
    FILE *RLE = fopen(newFileName, "rb");
    strcat(newFileName, ".freq");
    FILE *file = fopen(newFileName, "w");

    if (lastBlock < 1024 && blockNum > 1) {
        blockNum -= 1;
        lastBlock += blockSize;
    }

    fprintf(file, "@R@%lld", blockNum);

    while (bNum <= blockNum) {
        if (bNum == blockNum)
            blockSize = lastBlock;

        int symbCounter = 0;
        int symb[255] = {0};

        fprintf(file, "@%ld@", blockSize);

        while (symbCounter < blockSize){
            fread(&current, sizeof(unsigned char), 1, RLE);
            fread(&forwards1, sizeof(unsigned char), 1, RLE);
            fread(&forwards2, sizeof(unsigned char), 1, RLE);

            if (current == '{' && forwards1 == '0' && forwards2 == '}'){
                char number[13];

                fread(&current, sizeof(unsigned char), 1, RLE);
                fread(&forwards1, sizeof(unsigned char), 1, RLE);
                fread(&forwards2, sizeof(unsigned char), 1, RLE);

                if (current == '{' && forwards1 == '0' && forwards2 == '}') {
                    int tmp = 0;

                    fseek(RLE, 1, SEEK_CUR);
                    fread(&current, sizeof(unsigned char), 1, RLE);

                    while (current != '}') {
                        number[tmp++] = current;
                        fread(&current, sizeof(unsigned char), 1, RLE);
                    } number[tmp] = '\0';

                    amount = atoi(number);
                    symb[(unsigned char) '{'] += amount;
                    symb[(unsigned char) '0'] += amount;
                    symb[(unsigned char) '}'] += amount;
                    symbCounter += amount*3;
                } else {
                    int tmp = 0;

                    while (forwards2 != '}') {
                        number[tmp++] = forwards2;
                        fread(&forwards2, sizeof(unsigned char), 1, RLE);
                    } number[tmp] = '\0';

                    amount = atoi(number);
                    symb[current] += amount;
                    symbCounter += amount;
                }
            } else {
                if (symbCounter == blockSize-2)
                    fseek(RLE, -1, SEEK_CUR);
                else if (symbCounter == blockSize-1 || symbCounter == blockSize)
                    fseek(RLE, 0, SEEK_CUR);
                else
                    fseek(RLE, -2, SEEK_CUR);
                symb[current]++;
                symbCounter++;
            }
        }

        for (int i = 0; i < 255; i++) {
            if (i == 0 || symb[i] != symb[i-1]) {
                fprintf(file, "%d;", symb[i]);
            } else {
                fprintf(file, ";");
            }
        }

        bNum++;
    }

    fprintf(file, "@0");

    fclose(file);
    fclose(RLE);
}

// </Freq Stuff>

char *moduleF(char bSize, int forceRLE, unsigned char *filename){
    clock_t begin = clock();

    unsigned long blockSize;
    int rleUsed;

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
        long long originalFileSize = n_blocks * blockSize + last_Block_Size;
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);
        char *fileStr = malloc(fileSize+1);
        fileStr[fileSize] = '\0';
        char *newFilename = (char *) malloc(strlen(filename)+4);
        strcpy(newFilename, filename);
        strcat(newFilename, ".rle");

        fread(fileStr, 1, fileSize, file);

        fclose(file);

        freqN(n_blocks, blockSize, last_Block_Size, filename);

        if (n_blocks != 1)
            rleUsed = rle(filename, fileStr, blockSize, forceRLE);
        else
            rleUsed = rle(filename, fileStr, last_Block_Size, forceRLE);

        if (rleUsed != 0)
            freqR(filename, n_blocks, blockSize, last_Block_Size);

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

        printf("[Tomás Dias, Mariana Marques, MIEI/CD, 03/01/2021]\n");
        printf("[Módulo: F]\n");
        printf("[Número de Blocos: %lld]\n", n_blocks);

        if (n_blocks > 1)
            printf("[Tamanho dos blocos analisados no ficheiro original: %ld/%ld]\n", blockSize, last_Block_Size);
        else
            printf("[Tamanho dos blocos analisados no ficheiro original: %ld]\n", last_Block_Size);

        printf("[Tempo de Execução: %f ms]\n", time_spent);
        if (rleUsed != 0) {
            n_blocks = fsize(NULL, newFilename, &blockSize, &last_Block_Size);
            long long rleFileSize = n_blocks * blockSize + last_Block_Size;

            double compression = originalFileSize - rleFileSize;
            compression = compression/originalFileSize;
            compression = compression*100;

            printf("[Ficheiro RLE: %s.rle (%f%% compressão)]\n", filename, compression);
            printf("[tamanhos de todos os blocos processados no ficheiro RLE: %d]\n", rleUsed);
            printf("[Ficheiros Gerados: %s.rle, %s.freq, %s.rle.freq]\n", filename, filename, filename);
        } else
            printf("[Ficheiros Gerados: %s.freq]\n", filename);

    } else
        printf("File not found");

    char *newFilename = (char *) malloc(strlen(filename)+4);
    strcpy(newFilename, filename);
    strcat(newFilename, ".rle");

    if (rleUsed != 0)
        return newFilename;
    else
        return filename;
}
