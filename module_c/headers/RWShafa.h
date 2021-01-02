#ifndef __RWSHAFAFILE__
#define __RWSHAFAFILE__

#include"./RWCod.h"

int getRleBuffer(char*,char**);
int isRleCompression(char*,int,int);
int encodeSymbol(SFCodes,char,char*,int*);
int encodeRle(SFCodes,char*,int*,char*,int*,int*);
int writeShafaFile(char*,char*);


#endif
