#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lzw.h"

void Help_Message(char **argv)
{
	fprintf(stderr, "USAGE: ./lzw <encode>/<decode> -input <input> -output <output>\n");
	
}

void Set(FILE **fd, int argc, char **argv, char *mode, int i)
{
	if(i + 1 >= argc) {
		fprintf(stderr, "%s error: missing argument for %s\n", argv[0], argv[i]);
		exit(1);
	}
	
	(*fd) = fopen(argv[i+1], mode);
	
	if((*fd) == NULL) {
		fprintf(stderr, "%s error: %s : %s\n", argv[0], strerror(errno), argv[i+1]);
		exit(1);
	}
	
}

int main(int argc, char *argv[])
{
	FILE *in = stdin;
	FILE *out = stdout;
	int encode = 0;
	int decode = 0;
	unsigned int i;
	for(i = 0; i < argc; i++) {
		if(argc == 1 || (argc > 1 && strcmp(argv[1], "help") == 0)) {
			break;
		}
		else {
			if(strcmp(argv[i], "encode") == 0)
				encode = 1;
			else if(strcmp(argv[i], "decode") == 0)
				decode = 1;
			else if(strcmp(argv[i], "-input") == 0)
				Set(&in, argc, argv, "r", i);
			else if(strcmp(argv[i], "-output") == 0)		
				Set(&out, argc, argv, "w", i);
		}
	}
	
	if(encode != decode) {
		if(encode)
			Encode(SIZE, in, out);
		else
			Decode(SIZE, in, out);
	}
	else {
		if(encode && decode)
			fprintf(stderr, "%s error: cannot encode and decode at the same time!\n\n", argv[0]);
		else
			fprintf(stderr, "%s error: -encode or -decode not specified.\n\n", argv[0]);
			
		Help_Message(argv);
	}

	fclose(in);
	fclose(out);
	
	return 0;
}

