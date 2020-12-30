

#include <stdio.h>
#include <string.h>
// example used on the read code of module c

typedef struct sfc{
    char** codes;
    int nBytes;
    int nBite_compressed;
} *SFCodes


int main(){
return 0;
}




int readCodFile(SFCodes** blocosSF, char* fileName){

int nBlocos,symbol=0,blocoAtual=0;
File *fp = fopen(fileName,"r");
char codeSF[9],c;
int lenCodeSF=0;

if(!fp) return 0;

codeSF[0]='\0';

fseek(fp,3,SEEK_SET);
fscanf(fp,"%d",&nBlocos);

*blocosSF = (SFCodes*) malloc(sizeof(SFCodes)*nBlocos);
fseek(fp,1,SEEK_CUR);  //consome @ de bytes bloco

    while(blocoAtual<nBlocos){

        (*blocosSF)[blocoAtual] = initializeSFCodes();

        symbol = 0;

        fscanf(fp,"%D",$sizeBloco);
        setBytes((*blocoSF)[blocoAtual], sizeBloco);

        fseek(fp,1,SEEK_CUR);//consome @ de inicio bloco
        while(symbol<256){ // leitura bloco individual
                c=fgetc(fp);

                if((c==';' || c=='@')&& lenCodeSF==0){symbol++;}
                else if(c==';' || c=='@'){
                    setCodeSF((*blocoSF)[blocoAtual],symbol,codeSF);
                    lenCodeSF=0;
                    codeSF[0]='\0';
                    symbol++;
                }
                else{
                    codeSF[lenCodeSF++]=c;
                    codeSF[lenCodeSF]='\0';
                }
        }blocoAtual++;
    }
    fclose(fp);
    return nBlocos;
}



#include <stdint.h>
#include <stdlib.h>

#define MAX_TREE_NODES 511

//prototipe to decompress the file.shafa

typedef struct {
	uint8_t* BytePointer;
	uint32_t BitPosition;
} BitStream;

typedef struct {
	uint32_t Symbol;
	uint32_t Count;
	uint32_t Code;
	uint32_t Bits;
} Symbol;

typedef struct treeNode {
	struct treeNode* ChildA;
	struct treeNode* ChildB;
	int Symbol;
} TreeNode;

void initBitStream(BitStream* stream, uint8_t* buffer)
{
	stream->BytePointer = buffer;
	stream->BitPosition = 0;
}

uint32_t readBit(BitStream* stream)
{
	uint8_t* buffer = stream->BytePointer;
	uint32_t bit = stream->BitPosition;
	uint32_t x = (*buffer & (1 << (7 - bit))) ? 1 : 0;
	bit = (bit + 1) & 7;

	if (!bit)
	{
		++buffer;
	}

	stream->BitPosition = bit;
	stream->BytePointer = buffer;

	return x;
}

uint32_t read8Bits(BitStream* stream)
{
	uint8_t* buffer = stream->BytePointer;
	uint32_t bit = stream->BitPosition;
	uint32_t x = (*buffer << bit) | (buffer[1] >> (8 - bit));
	++buffer;
	stream->BytePointer = buffer;

	return x;
}

TreeNode* recoverTree(TreeNode* nodes, BitStream* stream, uint32_t* nodeNumber)
{
	TreeNode* thisNode;

	thisNode = &nodes[*nodeNumber];
	*nodeNumber = *nodeNumber + 1;

	thisNode->Symbol = -1;
	thisNode->ChildA = (TreeNode*)0;
	thisNode->ChildB = (TreeNode*)0;

	if (readBit(stream))
	{
		thisNode->Symbol = read8Bits(stream);
		return thisNode;
	}

	if (readBit(stream))
	{
		thisNode->ChildA = recoverTree(nodes, stream, nodeNumber);
	}

	if (readBit(stream))
	{
		thisNode->ChildB = recoverTree(nodes, stream, nodeNumber);
	}

	return thisNode;
}

void Decompress(uint8_t* input, uint8_t* output, uint32_t inputSize, uint32_t outputSize)
{
	TreeNode nodes[MAX_TREE_NODES], *root, *node;
	BitStream stream;
	uint32_t i, nodeCount;
	uint8_t* buffer;

	if (inputSize < 1) return;

	initBitStream(&stream, input);

	nodeCount = 0;
	root = recoverTree(nodes, &stream, &nodeCount);
	buffer = output;

	for (i = 0; i < outputSize; ++i)
	{
		node = root;

		while (node->Symbol < 0)
		{
			if (readBit(&stream))
				node = node->ChildB;
			else
				node = node->ChildA;
		}

		*buffer++ = (uint8_t)node->Symbol;
	}
}


// some tries
uint8_t* decompressedData = (uint8_t*)malloc(originalDataSize);

Decompress(compressedData, decompressedData, compressedDataSize, originalDataSize);



