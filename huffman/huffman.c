#include "huffman.h"

/*
 * Calculates the frequency of each ascii character.
 * Counts number of appeared characters.
 */
void Frequency(FILE *f) {
	int ch;
	if(f == NULL) {
		printf("Cannot Open Input File.\n");
		exit(0);
	}
	while (1) {
		ch = fgetc(f);
		if(ch == EOF)
			break;
		frequency[ch]++;
		size++;
	}
	for(ch = 0; ch < character; ch++)
		if (frequency[ch] > 0)
			active++;
}
/*
 * Allocate space for frequencies and leaf-node index look up table.
 */
void Initiate() {
	frequency = (int *)calloc(512, sizeof(int));
	leaf = frequency + character - 1;
}

/*
 *	Allocate space for the coding tree nodes and parent index
 *	look-up table making full binary tree.
 *	i.e every internal node has two children.
 *	For n leaves there are 2n − 1 nodes; n − 1 internal nodes. 
 */
void InitTree() {
	node = (huffnode *)calloc(active * 2, sizeof(huffnode));
	parent = (int *)calloc(active, sizeof(int));
}

/*
 * Only consider the active characters to store. Builds the tree.
 */
void AddTree() {
	int i, n;
	for (i = 0; i < character; i++) {
		n = frequency[i];
		/* If active */
		if (n > 0) {
			Store(-(i + 1), n);
		}
	}
}

/*
 * Traverses each node that has been moved up and update their index
 * in the corresponding look-up table with the new location.
 * Leaf node indices requires index negation.
 */
int Store(int index, int count) {
	int i = nodecount;
	nodecount++;
	while(i > 0 && node[i].count > count) {
		memcpy(&node[i + 1], &node[i], sizeof(huffnode));
		/* Character Identified. Thus leaf Node. */
		if (node[i].index < 0)
			++leaf[-node[i].index];
		/* No Character Identified. Thus Parent Node. */
		else
			++parent[node[i].index];
		i--;
	}
	i++;
	node[i].index = index;
	node[i].count = count;
	if(index < 0)
		leaf[-index] = i;
	else
		parent[index] = i;

	return i;
}

/*
 * Adds new created node to tree with node pair as children.
 * Store the same parent index in the node, so that node
 * indices can be updated as the tree structure changes.
 */
void Generate() {
	int odd, even, index, freeindex = 1;
	while(freeindex < nodecount) {
		/*Odd Index*/
		odd = freeindex;
		freeindex++;
		/*Even Index*/
		even = freeindex;
		freeindex++;
		index = Store(even/2, node[odd].count + node[even].count);
		parent[even/2] = index;
	}
}

/*
 * Determine the frequencies, write the header
 * data with these frequencies for decoding.
 */
int Encode(const char* in, const char *out) {
	FILE *fin, *fout;
	if((fin = fopen(in, "rb")) == NULL) {
		perror("Input File cannot be opened.\n");
		return EINVAL;
	}
	if ((fout = fopen(out, "wb")) == NULL) {
		perror("Output File cannot be opened.\n");
		fclose(fin);
		return EINVAL;
	}
	Frequency(fin);
	arr = (int *) calloc(active - 1, sizeof(int));
	InitTree();
	AddTree();
	WriteInit(fout);
	Generate();
	fseek(fin, 0, SEEK_SET);

	int ch;
	while ((ch = fgetc(fin)) != EOF) {
		EncodeChar(fout, ch);
	}
	Flush(fout);
	free(arr);
	fclose(fin);
	fclose(fout);
	return 0;
}

/*
 * For each byte from the input data stream, we emit the
 * bits encoding the alphabet. This bit is written to fout .
 * Use a stack to reverse the order of the bits.
 * Get the node index assigned to the alphabet.
 * Use the parent index table to move up the tree, retrieving
 * the new parent using the index stored in the internal nodes using stack.
 * Left children have odd indices and right children have even indices. 
 * If left child, we emit a 1 ; or a 0, otherwise.
 */
void EncodeChar(FILE *fout, int character) {
	int key;
	top = 0;
	key = leaf[character + 1];
	while(key < nodecount) {
		arr[top] = key % 2;
		top++;
		key = parent[(key + 1) / 2];
	}
	top--;
	for(top; top > -1; top--) {
		WriteBit(fout, arr[top]);
	}
}

/*
 * Determine the character frequency from header of the input file. 
 * Read the header file, use the frequencies to build the binary coding tree.
 */
int Decode(const char* in, const char *out) {
	FILE *fin, *fout;
	if((fin = fopen(in, "rb")) == NULL) {
		perror("Input File cannot be opened.\n");
		return EINVAL;
	}
	if((fout = fopen(out, "wb")) == NULL) {
		perror("Output File cannot be opened.\n");
		fclose(fin);
		return EINVAL;
	}

	if(ReadInit(fin) == 0) {
		Generate();
		DecodeChar(fin, fout);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}

/*
 * Retrieve one bit at a time and start at the root. 
 * Depending on the bit value, visit the left subtree(odd index) or the
 * right subtree(even index).
 * For node index look-up, index in each node stores the sequence when
 * the node was inserted.
 */
void DecodeChar(FILE *fin, FILE *fout) {
	int i = 0, bit, key = node[nodecount].index;
	while(1) {
		bit = ReadBit(fin);
		if(bit == -1)
			break;
		key = node[key * 2 - bit].index;
		if(key < 0) {
			char c = -key - 1;
			fwrite(&c, 1, 1, fout);
			if (++i == size)
				break;
			key = node[nodecount].index;
		}
	}
}

/*
 * Writes a bit value bit to file f .
 * If the buffer is full, write the buffer to the file. 
 * Else, the bit is packed to the existing buffer.
 */
int WriteBit(FILE *f, int bit) {
	unsigned int byte, pos, val;
	if(bits == MAX << 3) {
		size_t bytes_written = fwrite(buffer, 1, MAX, f);
		if(bytes_written < MAX && ferror(f))
			return -1;
		bits = 0;
		memset(buffer, 0, MAX);
	}
	if(bit) {
		byte = bits >> 3;
		pos = bits % 8;
		val = (1 << (7 - pos));
		buffer[byte] = buffer[byte] | (val);
	}
	++bits;
	return 0;
}

/*
 * writes the last bits in the buffer, packing them into the nearest bytes.
 * If the bits are not fully packed, the bits are zero-padded.
 */
int Flush(FILE *fp) {
	if(bits) {
		int pad = (bits + 7) >> 3;
		size_t bytes = fwrite(buffer, 1, pad, fp);
		if(bytes < MAX && ferror(fp))
		return -1;
		bits = 0;
	}
	return 0;
}

/*
 * Reads a bit from the file f.
 * Read a full buffer at a time, and return bits from this buffer.
 * Buffer is replenished with more bits from the file when empty.
 */
int ReadBit(FILE *f) {
	unsigned int current, pos;
	int bit, end = 0;
	if(currbit == bits) {
		if(end)
			return -1;
		else {
			size_t bytes = fread(buffer, 1, MAX, f);
			if (bytes < MAX) {
				if (feof(f))
					end = 1;
			}
			bits = bytes << 3;
			currbit = 0;
		}
	}
	if(bits == 0)
		return -1;
	current = currbit >> 3;
	pos = currbit % 8;
	bit = (buffer[current] >> (7 - pos)) & 0x1;
	++currbit;
	return bit;
}

/*
 * Writes the header after inserting leaves and before building the coding tree.
 * size is number of bytes in data stream.
 * active is number of active characters.
 * character freq is alphabet code(1 byte) + Frequency. 
 */
int WriteInit(FILE *f) {
	int i, j, byte = 0, alphabetcode = 1;
	int total = sizeof(unsigned int) + 1 + active * (alphabetcode + sizeof(int));
	unsigned int cnt;
	char *buffer = (char *) calloc(total, 1);
	if (buffer == NULL)
		return -1;

	j = sizeof(int);
	while (j--)
		buffer[byte++] = (size >> (j << 3)) & 0xff;
	buffer[byte++] = (char) active;
	for(i = 1; i <= active; ++i) {
		cnt = node[i].count;
		buffer[byte++] = (char) (-node[i].index - 1);
		j = sizeof(int);
		while (j--)
			buffer[byte++] = (cnt >> (j << 3)) & 0xff;
	}
	fwrite(buffer, 1, total, f);
	free(buffer);
	return 0;
}

/*
 * To decode the encoded file, read back header and retrieve
 * the active characters and corresponding frequencies. 
 * Rebuild the coding tree.
 */
int ReadInit(FILE *f) {
	int i, j, byte = 0, totes;
	size_t bytes;
	unsigned char buff[4];

	bytes = fread(&buff, 1, sizeof(int), f);
	if(bytes < 1)
		return -1;
	byte = 0;
	size = buff[byte++];
	while(byte < sizeof(int))
		size = (size << (1 << 3)) | buff[byte++];

	bytes = fread(&active, 1, 1, f);
	if(bytes < 1)
		return -1;

	InitTree();
	totes = active * (1 + sizeof(int));
	unsigned int cnt;
	char *buffer = (char *) calloc(totes, 1);
	if(buffer == NULL)
		return -1;
	fread(buffer, 1, totes, f);
	byte = 0;
	for(i = 1; i <= active; ++i) {
		node[i].index = -(buffer[byte++] + 1);
		j = 0;
		cnt = (unsigned char) buffer[byte++];
		while (++j < sizeof(int)) {
			cnt = (cnt << (1 << 3)) | (unsigned char) buffer[byte++];
		}
		node[i].count = cnt;
	}
	nodecount = (int) active;
	free(buffer);
	return 0;
}



void Help() {
	fprintf(stderr, "USAGE: ./huffman <encode>/<decode> <input> <output>");
}

void FreePtr() {
	free(parent);
	free(frequency);
	free(node);
}

int main(int argc, char **argv) {
	if(argc != 4) {
		Help();
		return 1;
	}
	Initiate();
	if(strcmp(argv[1], "encode") == 0)
		Encode(argv[2], argv[3]);
	else if (strcmp(argv[1], "decode") == 0)
		Decode(argv[2], argv[3]);
	else
		Help();
	FreePtr();
	return 0;
}


