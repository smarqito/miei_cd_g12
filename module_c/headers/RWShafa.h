#ifndef __RWSHAFAFILE__
#define __RWSHAFAFILE__

#include"./RWCod.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

int getRleBuffer(char*,char**);
int writeShafaFile(char*,char*);


#endif
