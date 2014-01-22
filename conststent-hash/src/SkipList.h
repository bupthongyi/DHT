#ifndef _SKIPLIST_H__
#define _SKIPLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZSKIPLIST_MAXLEVEL  12   
#define ZSKIPLIST_BRANCHING 4   

typedef int  (*CompareFunc)(const void *a, const void *b);
typedef void (*ForEachFunc)(void *value, void *data);

/* skiplist node definition */
typedef struct _skiplist_node_t {
	unsigned int key;
	void      *value;
	struct _skiplist_node_t **forward;
}T_SkipListNode;

/* skiplist definition */
typedef struct _skiplist_t {
	struct _skiplist_node_t *header;
	unsigned int length;
	int level;
	CompareFunc compare_func;
}T_SkipList;


T_SkipListNode	*skiplist_create_node(int level, unsigned int key, void *value);
void			skiplist_free_node(T_SkipListNode *node);

T_SkipList		*skiplist_create(CompareFunc compare_func);
void			skiplist_free(T_SkipList *krsl) ;

T_SkipListNode	*skiplist_insert(T_SkipList *krsl, unsigned int key, void *value);
int				skiplist_delete(T_SkipList *krsl, unsigned int key, void *value);
void			*skiplist_lookup(T_SkipList *krsl, unsigned int key, void *value);
void			skiplist_foreach(T_SkipList *krsl, ForEachFunc foreach_func, void *data);

/* cycle look up for consistent hash locating */
void			*skiplist_lookup_cycle(T_SkipList *krsl, unsigned int key);

#endif /* _SKIPLIST_H__ */