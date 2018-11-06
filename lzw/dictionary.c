#include "dictionary.h"

/*
 * Initiates Dictionary.
 */
dict *Init() {
	int i;
	dict *d = (dict *)malloc(sizeof(dict));
	d->len = MAXSIZE;
	d->entries = (node **)calloc(d->len, sizeof(node *));

	for(i = 0; i < CHARAC; i++) {
		Seq *seq = CreateSeq(i);
		Insert(d, CreateNode(seq, i), i);
	}
	return d;
}

/*
 *  Determines if the sequence s exists in the dictionary dict.
 *  Returns 1 if found, and sets entry to the appropriate sequence.
 *  Returns 0 if not found, entry is then set to NULL.
 */
int Search(dict *d, Seq *s, node **entry) {

	unsigned int index = Hash(s, d->len);
	node *tmp = d->entries[index];

	while(tmp != NULL) {
		if(Equal(tmp->key, s)) {
			(*entry) = tmp;
			return 1;
		}
		tmp = tmp->next;
	}
	(*entry) = NULL;
	return 0;
}

/*
 *  Inserts a Sequence pair entry into the dictionary at a specified index.
 */
void Insert(dict *d, node *pair, unsigned int index) {
	assert(d != NULL);
	if(d->entries[index] == NULL)
		d->entries[index] = pair;
	else {
		node *tmp = d->entries[index];
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = pair;
	}
}

/*
 * Initiates Nodes and stores value and key.
 */
node *CreateNode(Seq *key, unsigned int value) {
	node *x = (node *)malloc(sizeof(node));
	x->code = value;
	x->next = NULL;
	x->key = key;
	return x;
}

/*
 * Frees Dictionary.
 */
void DestroyDict(dict *d) {
	int i;
	for(i = 0; i < d->len; i++) {
		node *tmp = d->entries[i];
		DestroyNode(tmp);
	}
	free(d->entries);
	free(d);
}

/*
 * Frees Nodes.
 */
void DestroyNode(node *x) {
	if(x != NULL) {
		DestroyNode(x->next);
		DestroySeq(x->key);
		free(x);
	}
}