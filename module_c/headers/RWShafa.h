#ifndef __RWSHAFAFILE__
#define __RWSHAFAFILE__

#include"./RWCod.h"

int getRleBuffer(char*,char**);
int isRleCompression(char*,int,int);
int encodeSymbol(SFCodes,char,char*,int*);
int encodeRle(int*,SFCodes,char*,int*,char*,int*,int*,int,int);
int writeShafaFile(SFCodes*,int,char*,int,char*);

#endif
