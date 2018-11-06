#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "bit.h"
#include "dictionary.h"

void Encode(unsigned int maxBits, FILE *in, FILE *out);
void Decode(unsigned int maxBits, FILE *in, FILE *out);