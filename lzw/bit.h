#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 16
#define MAX 65536

typedef struct Bits {
	unsigned int extra;
	unsigned int count;
	unsigned int bits;
	FILE *fd;
} Bits;

Bits *CreateBit(FILE *fd);
void Clear(Bits *bits);
int ReadBit(Bits *bits, unsigned int nBits, unsigned int *des);
int WriteBit(Bits *bits, unsigned int nBits, unsigned int src);
int Pad(Bits *bits);
