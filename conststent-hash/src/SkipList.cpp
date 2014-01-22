#include "SkipList.h"
#include <time.h>

T_SkipListNode *skiplist_create_node(int level, unsigned int key, void *value) 
{
	T_SkipListNode *node = (T_SkipListNode *)malloc(sizeof(T_SkipListNode));
	if (node == NULL) 
	{
		fprintf(stderr, "malloc node failed!\n");
		return NULL;
	}

	node->key = key;
	node->value = value;
	node->forward = (_skiplist_node_t **)calloc(level, sizeof(T_SkipListNode));
	if (node->forward == NULL) 
	{
		fprintf(stderr, "malloc node->forward failed!\n");
		free(node);
		return NULL;
	}

	return node;
}

void skiplist_free_node(T_SkipListNode *node) 
{
	free(node->forward);
	free(node);
}

T_SkipList *skiplist_create(CompareFunc compare_func) 
{
	T_SkipList *krsl;
	int i;

	krsl = (T_SkipList *)malloc(sizeof(T_SkipList));
	if (krsl == NULL) 
	{
		fprintf(stderr, "malloc krsl failed!\n");
		return NULL;
	}
	krsl->header = skiplist_create_node(ZSKIPLIST_MAXLEVEL, 0, NULL);
	for (i = 0; i < ZSKIPLIST_MAXLEVEL; i++) 
	{
		krsl->header->forward[i] = NULL;
	}
	krsl->length = 0;
	krsl->level = 1;
	krsl->compare_func = compare_func;

	return krsl;
}


void skiplist_free(T_SkipList *krsl) 
{
	T_SkipListNode *node = krsl->header->forward[0], *next;

	free(krsl->header);
	while(node) 
	{
		next = node->forward[0];
		skiplist_free_node(node);
		node = next;
	}
	free(krsl);
}


static int skiplist_random_level(unsigned int key) 
{
	int level = 1;
	srand((unsigned int)(time(NULL)+key));
	while (level<ZSKIPLIST_MAXLEVEL && (rand()%ZSKIPLIST_BRANCHING) == 0) 
	{
		level += 1;
	}
	return level;
}


static T_SkipListNode *
	skiplist_lookup_internal(T_SkipList *krsl, T_SkipListNode **update, 
	unsigned int key, void *value)
{
	T_SkipListNode *x;
	int i;

	x = krsl->header;
	for (i = krsl->level-1; i >= 0; i--) {
		while (x->forward[i] &&
			(x->forward[i]->key < key ||
			(x->forward[i]->key == key &&
			krsl->compare_func(x->forward[i]->value, value) < 0))) {
				x = x->forward[i];
		}
		update[i] = x;
	}

	return x->forward[0];
}


T_SkipListNode *skiplist_insert(T_SkipList *krsl, unsigned int key, void *value) 
{
	T_SkipListNode *update[ZSKIPLIST_MAXLEVEL], *x;
	int i, level;

	x = skiplist_lookup_internal(krsl, update, key, value);
	if (x && key == x->key && !krsl->compare_func(x->value, value)) {
		x->value = value;
		return x;
	}

	level = skiplist_random_level(key);
	if (level > krsl->level) {
		for (i = krsl->level; i < level; i++) {
			update[i] = krsl->header;
		}
		krsl->level = level;
	}
	x = skiplist_create_node(level, key, value);
	for (i = 0; i < level; i++) {
		x->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = x;
	}

	krsl->length++;
	return x;
}

int skiplist_delete(T_SkipList *krsl, unsigned int key, void *value) 
{
	T_SkipListNode *update[ZSKIPLIST_MAXLEVEL], *x;
	int i;

	x = skiplist_lookup_internal(krsl, update, key, value);
	if (x && key == x->key && !krsl->compare_func(x->value, value)) {
		for (i = 0; i < krsl->level; i++) {
			if (update[i]->forward[i] == x) {
				update[i]->forward[i] = x->forward[i];
			}
		}

		skiplist_free_node(x);

		while(krsl->level>1 && krsl->header->forward[krsl->level-1] == NULL)
			krsl->level--;

		krsl->length--;
		return 0;
	}

	return -1;
}


void *skiplist_lookup(T_SkipList *krsl, unsigned int key, void *value)
{
	T_SkipListNode *update[ZSKIPLIST_MAXLEVEL], *x;

	x = skiplist_lookup_internal(krsl, update, key, value);
	if (x && key == x->key && !krsl->compare_func(x->value, value)) {
		return x->value;
	}

	return NULL;
}


void skiplist_foreach(T_SkipList *krsl, ForEachFunc foreach_func, void *data)
{
	T_SkipListNode *node = krsl->header->forward[0], *next;

	while(node) {
		next = node->forward[0];
		if (foreach_func) {
			foreach_func(node->value, data);
		}
		node = next;
	}
}


void *skiplist_lookup_cycle(T_SkipList *krsl, unsigned int key)
{
	T_SkipListNode *x;
	int i;

	x = krsl->header;
	for (i = krsl->level-1; i >= 0; i--) {
		while (x->forward[i] && (x->forward[i]->key < key )) {
			x = x->forward[i];
		}
	}

	x = x->forward[0];
	if (x) {
		return x->value;
	} else {
		return krsl->header->forward[0]->value;
	}
}