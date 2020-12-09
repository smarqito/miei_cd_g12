#include <stdio.h>
#include <stdlib.h>
#include "module_f.h"
#include "fsize.h"

// Start of Block stuff

struct blockList {
    int blockNum;
    char symbol;
    struct blockList* next;
};

void insertList (int block, char symbol, struct blockList* head) {
    struct blockList* new =  (struct blockList*)malloc(sizeof(struct blockList));
    struct blockList* temp;
    temp = head;

    new->blockNum = block;
    new->symbol = symbol;
    new->next = NULL;

    while(temp->next != NULL && temp != NULL)
        temp = temp->next;

    temp->next = new;
}

void trim (struct blockList* head){
    struct blockList *current, *prev;

    current = head;
    prev = head;

    while(current->next != NULL) {  // Find End file symbol
        prev = current;
        current = current->next;
    }

    if(current == head) {
        head = NULL;
    } else {
        prev->next = NULL;  // Delete end file symbol
    }

    free(current);  // Free Last symbol memory
}

//End of block stuff

/*void printList(struct blockList* n)
{
    while (n != NULL) {
        printf(" %c ", n->symbol);
        n = n->next;
    }
} Testing stuff*/

void *moduleF(char bSize, int forceRLE, char *filename){
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

    // Main Vars
    unsigned long last_Block_Size = 0;
    FILE *file = fopen(filename, "r");
    long long n_blocks = fsize(NULL, filename, &blockSize, &last_Block_Size);
    struct blockList* head =  (struct blockList*)malloc(sizeof(struct blockList));
    char c = fgetc(file);
    int bCount = 1;
    int sCount = 1;

    
    if (c != feof(file)){
        head->blockNum = bCount;
        head->symbol = c;
        head->next = NULL;
    }

    while (!(feof(file))){
        c = fgetc(file);

        if(bCount <= n_blocks && sCount < blockSize){
            insertList(bCount, c, head);
            sCount++;
        } else {
            insertList(++bCount, c, head);
            sCount = 1;
        }
    }

    trim(head);

    // printList(head); Testing
}
