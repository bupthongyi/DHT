#ifndef __CONHASH_H__
#define __CONHASH_H__

#include "SkipList.h"

typedef unsigned int (*HashFunc)(const void *key);

/* forward declaretion */
typedef struct _virtual_node_t T_VirtualNode;

typedef enum {
    KR_NODE_STATUS_CREATED = '0',
    KR_NODE_STATUS_INSERTED,
    KR_NODE_STATUS_DELETED
}E_NodeStatus;

/* actual node structure */
typedef struct _kr_actual_node_t
{
	unsigned int    hash;        /*node hash, hash(id)*/
	char            id[100];     /*node identifier, should be unique!*/
	unsigned int    weights;     /*node weights, the replicas of this node*/
	unsigned int    kicks;       /*node kicks, the kick times of this node*/
	T_VirtualNode *vnodes;     /*vitual nodes of this actual node*/
	E_NodeStatus  status;      /*node status*/
}T_ActualNode;

/* virtual node structure */
struct _virtual_node_t
{
	unsigned int    hash;        /* vnode hash, hash(vid)*/
	char            vid[128];    /* vnode identifier, come from nodeid!*/
	T_ActualNode *node;        /* pointer to the actual node */
};

/* consistent hashing */
typedef struct _conhash_t
{
	HashFunc      hash_func;   /* hash function */
	T_SkipList   *vnode_list;  /* skiplist of virtual nodes */
	T_SkipList   *node_list;   /* skiplist of actual nodes */
}T_ConHash;



T_ConHash	*conhash_construct(HashFunc hash_func);
void		conhash_destruct(T_ConHash *krch);

T_ActualNode	*conhash_lookup(T_ConHash *krch, char *id);
int				conhash_add(T_ConHash *krch, char *id, unsigned int weights);
int				conhash_remove(T_ConHash *krch, char *id);
int				conhash_adjust_weights(T_ConHash *krch, char *id, unsigned int weights);

T_ActualNode	*conhash_locate(T_ConHash *krch, char *object);

#endif /* __CONHASH_H__ */
