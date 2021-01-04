#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./module_t/module_t.h"
#include "./module_f/module_f.h"
#include "./module_d/module_d.h"
#include "./module_c/module_c.h"

/*
 * Call this function:
 * ./shafa.exe file_name -m f/t/d
 */

int main(int argc, char *argv[])
{
    if (argc >= 4 && argc <= 8)
    {
        int forceRLE = 0;
        char byteDimension = 'K';
        char *module = NULL;
        int moduleType;
        char *fileToOpen = argv[1];
        int optimizeSF_opt = 0;

        for (int i = 2; i < argc; i = i + 2)
        {

            if (strcmp(argv[i], "-m") == 0)
            {
                if (i + 1 < argc)
                {
                    module = argv[i];            // get module -m
                    moduleType = argv[i + 1][0]; // get module type [d, t, f]
                }
            }
            else if (strcmp(argv[i], "-c") == 0)
            {
                if (i + 1 < argc && argv[i + 1][0] == 'r')
                {
                    forceRLE = 1; //force RLE compression
                }
            }
            else if (strcmp(argv[i], "-b") == 0)
            {
                if (i + 1 < argc && (argv[i + 1][0] == 'K' || argv[i + 1][0] == 'm' || argv[i + 1][0] == 'M'))
                {
                    byteDimension = argv[i + 1][0]; // [K, m, M]
                }
            }
            else if (strcmp(argv[i], "-d") == 0)
            {
                if (i + 1 < argc)
                {
                    optimizeSF_opt = (argv[i + 1][0] == 's') ? 1 : (argv[i + 1][0] == 'r') ? 2 : 0;
                }
            }
        }

        if (module && fileToOpen)
        {
            char *freqFile = NULL;
            printf("File to read: \"%s\"\n", fileToOpen);
            switch (moduleType)
            {
            case 'c':
                //call module c
                freqFile = malloc(strlen(fileToOpen) * sizeof(char) + 6);
                strcpy(freqFile, fileToOpen);
                moduleC(moduloT(strcat(freqFile, ".freq")), fileToOpen);
                break;
            case 'd':
                // call module d
                printf("I'm on module: \"-m d\"\n");
                printf("Options: %c\n", optimizeSF_opt == 1 ? 's' : optimizeSF_opt == 2 ? 'r' : '-');
                decompress("./doc_teste/teste.txt.shaf", fileToOpen);
                // must output .shaf file
                // @shaf
                break;
            case 'f':
                // call module f
                // must output .rle file
                // @rle
                // TODO: módulo f em falta.
                moduleF(byteDimension, forceRLE, fileToOpen);
                
                printf("I'm on module: \"-m f\"\n");
                printf("ForceRLE?: %s\n", forceRLE ? "Sim" : "Não");
                printf("Tamanho: %cbytes\n", byteDimension);
                
                break;
            case 't':
                // call module t
                moduloT(fileToOpen);
                // must output .cod file
                // @cod
                break;
            default:
                printf("Reveja o comando inserido.\n");
                break;
            }
        } else if (fileToOpen) {
            // int fileNameLen = strlen(fileToOpen);
            // char *codFile;
            // char *shafaFile;
            // char *rleFile[fileNameLen+4];
            // char *freqFile;
            // freqFile = moduleF(byteDimension, forceRLE, fileToOpen);
            // codFile = moduleT(freqFile);
            // shafaFile = moduleC(codFile, strcat(rleFile, ".rle") );
            // moduleD(shafaFile, codFile);
        }
    }
    return 0;
}
