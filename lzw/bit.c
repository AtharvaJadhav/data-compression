#include "bit.h"

/*
 * Construct Bits.
 */
Bits* CreateBit(FILE *fd) {
	Bits *thing = (Bits *)malloc(sizeof(Bits));
	thing->fd = fd;
	thing->extra = 0;
	thing->count = 0;
	return thing;
}

/*
 * Reads the entry code per byte.
 * TO-DO: re-impliment for nBits bit width.
 */
int ReadBit(Bits *bits, unsigned int nBits, unsigned int *des) {
		unsigned char msb = fgetc(bits->fd);
		if(feof(bits->fd))
			return 0;
		unsigned char lsb = fgetc(bits->fd);
		(*des) = (msb << 8);
		(*des) = (*des) + lsb;
		return 1;
}

/*
 *  Writes the entry code per byte.
 *  TO-DO: re-impliment for nBits bit width.
 */
int WriteBit(Bits *bits, unsigned int nBits, unsigned int src) {
	fputc(((src >> 8) & 0xFF), bits->fd);
	fputc((src & 0xFF), bits->fd);
	return 1;
}

/* 
 * To Do
 * For n-Bits width.
 */
int Pad(Bits *bits) {
	return 0;
}
/*
 * Clear the bits in Bits stream.
 */
void Clear(Bits *bits) {
	free(bits);
}
