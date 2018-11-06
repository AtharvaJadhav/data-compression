#include "sequence.h"

/*
 * Initiates Bits.
 */
Seq *CreateSeq(unsigned char ch) {
	Seq *seq = (Seq *)malloc(sizeof(Seq));
	seq->length = 1;
	seq->str = (unsigned char *)malloc(sizeof(unsigned char));
	seq->str[0] = ch;
		return seq;
}

/*
 * Initiates sequence with specific size.
 */
Seq *Size(unsigned int size) {
	Seq *seq = (Seq *)malloc(sizeof(Seq));
	seq->length = 0;
	seq->str = (unsigned char *)malloc(sizeof(unsigned char) * size);
	return seq;
}

/*
 * Adds a byte to existing sequence.
 */
void Append(Seq *seq, unsigned char value) {
	seq->str = (unsigned char *)realloc(seq->str, ++seq->length);
	seq->str[seq->length - 1] = value;
}

/*
 * Duplicates the original sequence.
 */
void Duplicate(Seq *orig, Seq **dup) {
	if((*dup) != NULL)
		DestroySeq(*dup);
	(*dup) = Size(orig->length);
	memcpy((*dup)->str, orig->str, orig->length);
	(*dup)->length = orig->length;
}

/*
 * Writes the sequence to file.
 */
void Print(Seq *seq, FILE *fd) {
	int i = 0;
	while(i < seq->length) {
		fputc(seq->str[i], fd);
		i++;
	}
}

/*
 * Returns first byte of sequence.
 */
unsigned char First(Seq *seq) {
	return seq->str[0];
}

/*
 * Checks for equality.
 */
int Equal(Seq *first, Seq *second) {
	int i;
	if(first->length == second->length) {
		for(i = 0; i < first->length; i++) {
			if(first->str[i] != second->str[i])
				return 0;
		}
		return 1;
	}
	else
		return 0;
}

/*
 * Frees a sequence.
 */
void DestroySeq(Seq *seq) {
	if(seq != NULL) {
		free(seq->str);
		free(seq);
	}
}

/*
 * Computes hash for the sequence.
 */
unsigned int Hash(Seq *seq, unsigned int mod) {
	unsigned int hash = 0;
	int i;
	for(i = 0; i < seq->length; i++)
		hash = 33 * hash ^ seq->str[i];
	
	return hash % mod;
}
