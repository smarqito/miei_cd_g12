#ifndef MODULE_D_BIT
#define MODULE_D_BIT


#include <stdint.h>
#include <stdlib.h>
//prototipe to decompress the file.shafa
#define MAX_TREE_NODES 511

typedef struct
{
	uint8_t *BytePointer;
	uint32_t BitPosition;
} BitStream;

typedef struct
{
	uint32_t Symbol;
	uint32_t Count;
	uint32_t Code;
	uint32_t Bits;
} Symbol;

typedef struct treeNode
{
	struct treeNode *ChildA;
	struct treeNode *ChildB;
	int Symbol;
} TreeNode;

#endif