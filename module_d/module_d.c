#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "module_d.h"
#include "../module_t/module_t.h"
#include "../module_c/headers/RWCod.h"

// int decompressRLE(char *RLEFile)
// {

// 	FILE *outputFileName = fopen(OG, "w");
// 	int nBlocos2, nBlocosInit2, size2;
// 	FILE *fpRLE = fopen(RLEFile, "r");
// 	fseek(fpRLE, 0, SEEK_SET);
// 	for (int q = 0; q < strlen(RLEFile); q++)
// 	{
// 		if (strcmp(RLEFile[q], '{0}'))
// 		{
// 			int aux = 0;
// 			aux = RLEFile[q + 2];
// 			while (aux > 0)
// 			{
// 				fprintf(outputFileName, "%c", RLEFile[q + 1]);
// 				aux--;
// 			}
// 			q = q + 2;
// 		}
// 		else
// 			fprintf(outputFileName, "%c", RLEFile[q]);
// 	}
// 	fclose(outputFileName);
// 	return 0;
// }


char getCharFromSF (SFCodes sfCodes, char* sfcode) {
	int i;
	for (i = 0; i< 255 && strcmp(sfCodes->codes[i], sfcode); i++);
	return i < 255 ? i : -1;
}

int decompress(char *shafaFileName, char *codFileName)
{
	//verificar o n� de blocos(nBlocos)
	if (!codFileName || !shafaFileName)
		return 0;
	printf("%s\n", codFileName);
	int i;
	int symbolFreq;
	char t;
	int nBlocos, nBlocosInit, size;
	int simbolosNaoNulos;
	char tc;
	int rle;
	//verificar N ou R

	SFCodes *matriz;

	nBlocos = readCodFile(&matriz, codFileName);


	if (!nBlocos)
		return 0;


	

	//ler shafa
	// char ts;
	// int nBlocos1, nBlocosInit1, size1;
	// FILE *fpshafa = fopen(shafaFileName, "r");
	// fseek(fpshafa, 1, SEEK_SET);
	// fscanf(fpshafa, "%c", &ts);
	// if ((ts != 'N') || (ts != 'R'))
	// 	return 0;
	// nBlocos1 = nBlocosInit1 = atoi(seekFromFile(fpshafa, '@'));
	// if (!nBlocos1)
	// 	return 0;
	// seekFromFile(fpshafa, '@');
	// for (int q = 0; q < strlen(shafaFileName); q++)
	// {
	// 	if (seekFromFile(fpshafa, '@'))
	// 	{

	// 		while (nBlocosInit > 0)
	// 		{
	// 			(00000)
	// 			size = atoi(seekFromFile(fpshafa, '@'));
	// 			for (int i = 0; i < 255; i++)
	// 			{
	// 				if (strcmp(matriz[nBlocos][i], fpshafa))
	// 				{
	// 					fprintf(outputFileName, "%c", i);
	// 				}
	// 			}

	// 			nBlocosInit--;
	// 		}
	// 	}
	// }

	return 0;
}
