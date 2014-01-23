#include <windows.h>
#include "ConHash.h"
#include <stdint.h>
#include <assert.h>

static char *machines[5] = {"tiger-laptop", 
                            "rabbit-laptop", 
                            "dragon-laptop", 
                            "snake-laptop", 
                            "horse-laptop"};


void kr_vnode_print(void *value, void *data) 
{
    T_VirtualNode *vnode = (T_VirtualNode *)value;
    printf("vnode->hash[%u], vnode->vid[%s], node->id[%s]\n", 
           vnode->hash, vnode->vid, vnode->node->id);
}


void kr_node_print(void *value, void *data) 
{
    T_ActualNode *node = (T_ActualNode *)value;
    printf("node->hash[%u], node->id[%s], node->weights[%u], node->kicks[%u]\n", 
           node->hash, node->id, node->weights, node->kicks);
}


int main(int argc, char *argv[])
{
    int i;
    int result = 0;
    T_ConHash *krch = NULL;
    T_ActualNode *kick_node = NULL;
    char kick_value[200] = {0};
    
    krch = conhash_construct(NULL);
    assert(krch);
    result = conhash_add(krch, machines[0], 9);
    assert(result == 0);
    result = conhash_add(krch, machines[1], 3);
    assert(result == 0);
    result = conhash_add(krch, machines[2], 12);
    assert(result == 0);
    result = conhash_add(krch, machines[3], 4);
    assert(result == 0);
    result = conhash_add(krch, machines[4], 8);
    assert(result == 0);
    
	printf("node_num[%u], vnode_num[%u]\n", krch->node_list->length, krch->vnode_list->length);
    
    for (i = 0; i < 20; i++) 
	{
        if (i%3 == 0) 
		{
			wsprintfA(kick_value, "haha-%03d", i);
        } 
		else if (i%3 == 1) 
		{
            wsprintfA(kick_value, "hehe-%03d", i);
        } 
		else 
		{
            wsprintfA(kick_value, "hihi-%03d", i);
        }
        kick_node = conhash_locate(krch, kick_value);
        printf("kick [%s] node is[%s]\n", kick_value, kick_node->id);
    }
    
    skiplist_foreach(krch->vnode_list, kr_vnode_print, NULL);
    skiplist_foreach(krch->node_list, kr_node_print, NULL);
    
    result = conhash_remove(krch, machines[0]);
    assert(result == 0);
    printf("delete machine [%s]\n", machines[0]);

    result = conhash_remove(krch, machines[4]);
    assert(result == 0);
    printf("delete machine [%s]\n", machines[4]);

    result = conhash_adjust_weights(krch, machines[1], 5);
    assert(result == 0);
    printf("adjust machine[%s] weights[%u]\n", machines[1], 5);

	printf("node_num[%u], vnode_num[%u]\n", krch->node_list->length, krch->vnode_list->length);
    
    for (i = 0; i < 20; i++) 
	{
        if (i%3 == 0) 
		{
            wsprintfA(kick_value, "haha-%03d", i);
        }
		else if (i%3 == 1) 
		{
            wsprintfA(kick_value, "hehe-%03d", i);
        } 
		else 
		{
            wsprintfA(kick_value, "hihi-%03d", i);
        }
        kick_node = conhash_locate(krch, kick_value);
        printf("kick [%s] node is[%s]\n", kick_value, kick_node->id);
    }
    
    skiplist_foreach(krch->vnode_list, kr_vnode_print, NULL);
    skiplist_foreach(krch->node_list, kr_node_print, NULL);

    conhash_destruct(krch);
    
    printf("Sucess!\n");
    return 0;
}


