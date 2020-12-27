#include <stdio.h>
#include <stdlib.h>
#include "module_f.h"
#include "fsize.h"

// <Freq Stuff>

void freqN (unsigned char* block, int blockNum, int blockSize, FILE *file){
    int symbs[255] = {0};

    for (int i = 0; i < strlen(block); ++i)
        symbs[block[i]]++;

    fprintf(file, "@%d@", blockSize);

    for (int i = 0; i < 255; i++) {
        if (i == 0 || symbs[i] != symbs[i-1]) {
            fprintf(file, "%d;", symbs[i]);
        } else {
            fprintf(file, ";");
        }
    }
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
        unsigned char c;
        int bCount = 1;
        int sCount = 1;

        char *newFileName = (char *) malloc(strlen(filename)+9);
        strcpy(newFileName, filename);
        strcat(newFileName, ".rle");
        FILE *rleFile = fopen(newFileName, "wb+");

        strcpy(newFileName, filename);
        strcat(newFileName, ".freq");
        FILE *ogFreq = fopen(newFileName, "w");

        strcpy(newFileName, filename);
        strcat(newFileName, ".rle");
        strcat(newFileName, ".freq");
        FILE *rleFreq = fopen(newFileName, "w");

        fprintf(ogFreq, "@N@%d", n_blocks);

        for (int i = 1; i <= n_blocks; ++i) {
            if (n_blocks == i)
                blockSize = last_Block_Size;

            unsigned char block[blockSize+1];
            fread(&block, sizeof(char), blockSize, file);
            block[blockSize] = '\0';

            rle(rleFile, block);
            freqN(block, i, blockSize, ogFreq);
        }

        freqR(rleFreq, rleFile, forceRLE, n_blocks, blockSize, last_Block_Size);

        fprintf(ogFreq, "@0");

        fclose(ogFreq);
        fclose(rleFile);
        fclose(rleFreq);
        fclose(file);

    } else
        printf("File not found");
}
