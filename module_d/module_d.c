#include <stdio.h>
#include <stdlib.h>
#include "module_d.h"
#include "../module_t/module_t.h"

int decompressRLE(char *RLEFile)
{

	FILE *outputFileName = fopen(OG, "w");
	int nBlocos2, nBlocosInit2, size2;
	FILE *fpRLE = fopen(RLEFile, "r");
	fseek(fpRLE, 0, SEEK_SET);
	for (int q = 0; q < strlen(RLEFile); q++)
	{
		if (strcmp(RLEFile[q], '{0}'))
		{
			int aux = 0;
			aux = RLEFile[q + 2];
			while (aux > 0)
			{
				fprintf(outputFileName, "%c", RLEFile[q + 1]);
				aux--;
			}
			q = q + 2;
		}
		else
			fprintf(outputFileName, "%c", RLEFile[q]);
	}
	fclose(outputFileName);
	return 0;
}

int decompress(char *shafaFile, char *codFile)
{
	//verificar o n� de blocos(nBlocos)
	if (!codFile || !shafaFile)
		return 0;

	int i;
	int symbolFreq;
	char t;
	int nBlocos, nBlocosInit, size;
	int simbolosNaoNulos;
	char tc;
	//verificar N ou R
	FILE *fpcod = fopen(codFile, "r");
	fseek(fpcod, 1, SEEK_SET);
	fscanf(fpcod, "%c", &tc);
	if ((tc != 'N') || (tc != 'R'))
		return 0;

	nBlocos = nBlocosInit = atoi(seekFromFile(fpcod, '@'));
	if (!nBlocos)
		return 0;

	//criar uma matriz (nBlocos) linhas
	//cadalinha 255strings

	char *matriz[nBlocos];

	for (int k = 0; k < nBlocos; k++)
		matriz[k] = malloc(255 * sizeof(char *));

	fseek(fpcod, 1, -1);
	//ler cod
	// Criar o nome de output de forma din�mica, com base no nome do ficheiro de origem
	// armazena na mesma pasta que o ficheiro de origem
	char *outputFileName = strdup(shafaFile);
	strcpy(outputFileName + strlen(shafaFile) - 4, "cod");

	printAuthor(nBlocos);

	while (nBlocos > 0)
	{
		size = atoi(seekFromFile(fpcod, '@'));
		for (i = 0; i < 255; i++)
		{
			matriz[nBlocos][i] = seekFromFile(fpcod, ';');
		}

		seekFromFile(fp, '@');

		nBlocos--;
	}

	fclose(fpcod);

	//ler shafa
	char ts;
	int nBlocos1, nBlocosInit1, size1;
	FILE *fpshafa = fopen(shafaFile, "r");
	fseek(fpshafa, 1, SEEK_SET);
	fscanf(fpshafa, "%c", &ts);
	if ((ts != 'N') || (ts != 'R'))
		return 0;
	nBlocos1 = nBlocosInit1 = atoi(seekFromFile(fpshafa, '@'));
	if (!nBlocos1)
		return 0;
	seekFromFile(fpshafa, '@');
	for (int q = 0; q < strlen(shafaFile); q++)
	{
		if (seekFromFile(fpshafa, '@'))
		{

			while (nBlocosInit > 0)
			{
				size = atoi(seekFromFile(fpcod, '@'));
				for (int i = 0; i < 255; i++)
				{
					if (strcmp(matriz[nBlocos][i], shafaFile[q]))
					{
						fprintf(outputFileName, "%c", i);
					}
				}

				nBlocosInit--;
			}
		}
	}

	return 0;
}
