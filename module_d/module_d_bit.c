#include "module_d_bit.h"


void initBitStream(BitStream *stream, uint8_t *buffer)
{
	stream->BytePointer = buffer;
	stream->BitPosition = 0;
}

uint32_t readBit(BitStream *stream)
{
	uint8_t *buffer = stream->BytePointer;
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

uint32_t read8Bits(BitStream *stream)
{
	uint8_t *buffer = stream->BytePointer;
	uint32_t bit = stream->BitPosition;
	uint32_t x = (*buffer << bit) | (buffer[1] >> (8 - bit));
	++buffer;
	stream->BytePointer = buffer;

	return x;
}

TreeNode *recoverTree(TreeNode *nodes, BitStream *stream, uint32_t *nodeNumber)
{
	TreeNode *thisNode;

	thisNode = &nodes[*nodeNumber];
	*nodeNumber = *nodeNumber + 1;

	thisNode->Symbol = -1;
	thisNode->ChildA = (TreeNode *)0;
	thisNode->ChildB = (TreeNode *)0;

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

void Decompress(uint8_t *input, uint8_t *output, uint32_t inputSize, uint32_t outputSize)
{
	TreeNode nodes[MAX_TREE_NODES], *root, *node;
	BitStream stream;
	uint32_t i, nodeCount;
	uint8_t *buffer;

	if (inputSize < 1)
		return;

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
