#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
    if (argc == 3 && strcmp(argv[1],"-m") == 0 ) {
        if (strcmp(argv[2],"f") == 0 ) {
            // call module f
            printf("I'm on \"-m f\"\n");
            // must output .rle file
            // @rle
        } else if (strcmp(argv[2],"t") == 0) {
            // call module t
            printf("I'm on \"-m t\"\n");  
            // must output .cod file
            // @cod
        } else if (strcmp(argv[2],"d") == 0 ) {
            // call module d
            printf("I'm on \"-m d\"\n");
            // must output .shaf file
            // @shaf
        } else {
            // wtf is module c ???
        }
        return 0;
    }
}