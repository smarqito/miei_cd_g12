#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
    if (argc == 4 && strcmp(argv[2],'-m') == 0 ) {
        if (strcmp(argv[3],'f') == 0 ) {
            // call module f

            // must output .rle file
            // @rle
        } else if (strcmp(argv[3],'t') == 0) {
            // call module t

            // must output .cod file
            // @cod
        } else if (strcmp(argv[3],'d') == 0 ) {
            // call module d

            // must output .shaf file
            // @shaf
        } else {
            // wtf is module c ???
        }
        return 0;
    }
}