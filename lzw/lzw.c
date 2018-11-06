#include "lzw.h"

/*
 * Reads from input stream and writes lzw encoded code to output stream.
 */
void Encode(unsigned int bits, FILE *in, FILE *out) {
	unsigned int next = CHARAC;
	unsigned char byte = fgetc(in);
	int c = fgetc(in);

	Bits *output = CreateBit(out);
	dict *d = Init();
	Seq *w = CreateSeq(byte);
	Seq *x = NULL;
	node *entry = NULL;

	while(c != EOF) {
		Duplicate(w, &x);
		Append(x, c);
		if(Search(d, x, &entry)) {
			Duplicate(x, &w);
		}
		else {
			Search(d, w, &entry);
			WriteBit(output, SIZE, entry->code);
			if(next < MAX) {
				Seq *new = NULL;
				Duplicate(x, &new);
				Insert(d, CreateNode(new, next), Hash(new, MAX));
				next++;
			}
			DestroySeq(w);
			w = CreateSeq(c);
		}
		c = fgetc(in);
	}
	Search(d, w, &entry);
	WriteBit(output, SIZE, entry->code);
	
	DestroyDict(d);
	Clear(output);
	DestroySeq(w);	
	DestroySeq(x);
}


/*
 * Reads lzw encoded code from input stream and writes decoded code to output.
 */
void Decode(unsigned int bits, FILE *in, FILE *out) {
	unsigned int next = CHARAC;
	unsigned int prev = 0;
	int i;	

	Seq **table = (Seq **)calloc(MAX, sizeof(Seq *));
	Bits *input = CreateBit(in);

	for(i = 0; i < CHARAC; i++) {
		table[i] = CreateSeq(i);
	}

	ReadBit(input, bits, &prev);
	assert(prev < CHARAC);
	Print(table[prev], out);

	unsigned char ch;
	unsigned int current = 0;

	while(ReadBit(input, bits, &current)) {
		if(table[current] != NULL)
			ch = First(table[current]);
		else
			ch = First(table[prev]);
		if(next < MAX) {
			Seq *w = NULL;
			Duplicate(table[prev], &w);
			Append(w, ch);
			table[next] = w;
			next++;
		}
		Print(table[current], out);
		prev = current;
	}
	for (int i = 0; i < MAX; ++i) {
		DestroySeq(table[i]);
	}
	free(table);
	Clear(input);
}