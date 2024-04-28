#ifndef _OTS_RBTREE_H_
#define _OTS_RNTREE_H_

#include <stddef.h>

#include "../../defines.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OTS_RBTree;
struct OTS_RBTreeNode;
struct OTS_RBTreeIterator;

enum OTS_RBTreeNodeColor {
    BLACK, RED,
};

struct OTS_RBTree 
{
    size_t size;
    size_t keylen, valuelen;
    struct OTS_RBTreeNode *root;
    int (*nodecmp)(void *, void *);
};

struct OTS_RBTreeIterator 
{
    void *key;
    struct OTS_RBTreeIterator *prev, *next;
};

/**
 * @param cmp   这是一个比较函数，如果第一个元素被定义成大于第二个元素，返回大于0的值，
 *              如果第一个元素被定义成小于第二个元素，返回小于0的值，如果相等，返回0。
 * @param keylen key类型的长度，如果key是int类型，那么长度为4字节(仅在x64机器上)
 *
 *  
*/
struct OTS_RBTree *OTS_RBTree_Initialize(size_t keylen, size_t valuelen, int (*cmp)(void *, void *));
int OTS_RBTree_Insert(struct OTS_RBTree *tree, void *key, void *value);
void *OTS_RBTree_Find(struct OTS_RBTree *tree, void *key);
int OTS_RBTree_FindIF(struct OTS_RBTree *tree, void *key);

extern int OTS_RBTree_str_cmp(void *, void *);
extern int OTS_RBTree_int_cmp(void *, void *);
extern int OTS_RBTree_float_cmp(void *, void *);
extern int OTS_RBTree_double_cmp(void *, void *);

#ifdef __cplusplus
}
#endif

#endif