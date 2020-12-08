#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Call this function:
 * ./shafa.exe file_name -m f/t/d
 */

int main (int argc, char *argv[]) {
    if (argc == 4 && strcmp(argv[2],"-m") == 0 ) {
        printf("File to read: \"%s\"\n",argv[1]);
        if (strcmp(argv[3],"f") == 0 ) {
            // call module f
            printf("I'm on module: \"-m f\"\n");
            // must output .rle file
            // @rle
        } else if (strcmp(argv[3],"t") == 0) {
            // call module t
            printf("I'm on module: \"-m t\"\n");  
            // must output .cod file
            // @cod
        } else if (strcmp(argv[3],"d") == 0 ) {
            // call module d
            printf("I'm on module: \"-m d\"\n");
            // must output .shaf file
            // @shaf
        } else {
            // wtf is module c ???
        }
        return 0;
    }
}