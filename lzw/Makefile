lzw: bit.o sequence.o dictionary.o lzw.o main.o
	cc bit.o sequence.o dictionary.o lzw.o main.o -o lzw
bit.o: bit.h bit.c
	cc -Wall -c bit.c
sequence.o: sequence.h sequence.c
	cc -Wall -c sequence.c
dictionary.o: dictionary.h dictionary.c
	cc -Wall -c dictionary.c
lzw.o: lzw.h lzw.c
	cc -Wall -c lzw.c
main.o: main.c lzw.h
	cc -Wall -c main.c