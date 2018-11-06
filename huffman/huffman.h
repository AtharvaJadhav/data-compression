#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 256

typedef struct huffnode{
	int index;
	unsigned int count;
} huffnode;

unsigned char buffer[MAX];
int bits = 0;
int currbit = 0;
unsigned int size = 0;
int character = 256;
int active = 0;
huffnode *node = NULL;
int nodecount = 0;
int *leaf = NULL;
int *parent = NULL;
int *arr;
int *frequency = NULL;
int top;

void Initiate();
void Frequency(FILE *f);
void InitTree();
void AddTree();
int WriteInit(FILE *f);
void Generate();
int ReadInit(FILE *f);
int ReadBit(FILE *f);
int WriteBit(FILE *f, int bit);
int Flush(FILE *f);
void DecodeChar(FILE *fin, FILE *fout);
int Decode(const char* in, const char *out);
void EncodeChar(FILE *fout, int character);
int Encode(const char* in, const char *out);
int Store(int index, int count);
void FreePtr();