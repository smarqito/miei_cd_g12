#include <stdio.h>
#include <stdlib.h>
#include "module_f.h"
#include "fsize.h"
#include "SFCodes.h"

void rle (unsigned char *filename, char *fileStr){ 
    int i = 1;
    int sCount = 1;
    int special = 0;
    char sym = fileStr[0];
    char amount[12];
    char *newFilename = (char *) malloc(strlen(filename)+4);
    strcpy(newFilename, filename);
    strcat(newFilename, ".rle");
    FILE *rle = fopen(newFilename, "wb");

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

void freqR (FILE *file, FILE *RLE, int force, long long blockNum, unsigned long blockSize, unsigned long lastBlock){  //TODO THE CHECK IF RLE IS NOT FORCED
    unsigned char current;
    char check[2];
    int count = 1, amount = 0;
    int symbs[255] = {0};

    if (force == 1) {
        fprintf(file, "@R@%lld", blockNum);
    }
    else {  //TODO CHECK IF RLE GETS USED
        fprintf(file, "@N@%lld", blockNum);
    }

    fprintf(file, "@%ld@", blockSize);

    fseek(RLE, 0, SEEK_SET);

    for (int i = 1; i <= blockNum; ++i) {
        //TODO SQUISH LAST BLOCK IF SMALLER THEN 1Kb

        while (count <= blockSize) {
            fread(&current, sizeof(char), 1, RLE);

            if (current != '{'){
                symbs[current]++;
                count++;
            } else {
                fread(&check, sizeof(char), 2, RLE);
                if (check[0] == '0' && check[1] == '}'){
                    fseek(RLE, 1, SEEK_CUR);
                    fread(&current, sizeof(char), 1, RLE);
                    fseek(RLE, 2, SEEK_CUR);
                    fscanf(RLE, "%d", &amount);
                    fseek(RLE, 1, SEEK_CUR);

                    symbs[current] += amount;
                    if (current == '0') count += amount*3;
                    else count += amount;
                } else {
                    fwrite("{", sizeof(char), 1, RLE);
                    fseek(RLE, -2, SEEK_CUR);
                    count++;
                }
            }
        }

        for (int i = 0; i < 255; i++)
            if (i == 0 || symbs[i] != symbs[i-1]) {
                fprintf(file, "%d;", symbs[i]);
            } else {
                fprintf(file, ";");
            }

        memset(symbs, 0, sizeof(symbs));
    }

    fprintf(file, "@0");
}

// </Freq Stuff>

void *moduleF(char bSize, int forceRLE, unsigned char *filename){
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
        rle(filename, fileStr);
        freqR(filename, n_blocks, blockSize, last_Block_Size);

    } else
        printf("File not found");
}
