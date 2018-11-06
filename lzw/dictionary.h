#include "sequence.h"
#include <assert.h>

#define MAXSIZE 100000
#define CHARAC 256
typedef struct node {
	Seq *key;
	unsigned int code;
	struct node *next;
}node;

typedef struct dict {
	unsigned int len;
	node **entries;
}dict;

dict *Init();
int Search(dict *d, Seq *s, node **entry);
void Insert(dict *d, node *pair, unsigned int index);
node *CreateNode(Seq *, unsigned int);
void DestroyDict(dict *d);
void DestroyNode(node *);
