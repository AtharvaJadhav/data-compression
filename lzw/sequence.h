#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Seq {
	unsigned int length;
	unsigned char *str;
}Seq;

Seq *CreateSeq(unsigned char ch);
Seq *Size(unsigned int size);
void Append(Seq *seq, unsigned char);
void Duplicate(Seq *orig, Seq **dup);
void Print(Seq *, FILE *fd);
unsigned char First(Seq *);
int Equal(Seq *, Seq *);
void DestroySeq(Seq *);
unsigned int Hash(Seq *seq, unsigned int mod);
