#ifndef _OTS_RBTREE_H_
#define _OTS_RNTREE_H_

/**
 * @headerfile rbtree.h
 * @brief 这是红黑树的C语言实现，是kernel/set.h和kernel/map.h的底层实现
 */

#include <stddef.h>

#include "../../defines.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OTS_RBTree;
struct OTS_RBTreeNode;
struct OTS_RBTreeIterator;

struct OTS_RBTree 
{
    size_t size;
    size_t keylen, valuelen;
    struct OTS_RBTreeNode *root;
    int (*nodecmp)(void *, void *);
};

/**
 * @param cmp   这是一个比较函数，如果第一个元素被定义成大于第二个元素，返回大于0的值，
 *              如果第一个元素被定义成小于第二个元素，返回小于0的值，如果相等，返回0。
 * @param keylen key类型的长度，如果key是int类型，那么长度为4字节(仅在x64机器上)
 *
 *  
*/
extern struct OTS_RBTree *OTS_RBTree_Initialize(size_t keylen, size_t valuelen, int (*cmp)(void *, void *));
extern int OTS_RBTree_Insert(struct OTS_RBTree *tree, void *key, void *value);
extern void *OTS_RBTree_Erase1(struct OTS_RBTree *tree, void *key);
extern void *OTS_RBTree_Erase2(struct OTS_RBTree *tree, struct OTS_RBTreeIterator *begin, struct OTS_RBTreeIterator *end);
extern void *OTS_RBTree_Find(struct OTS_RBTree *tree, void *key);
extern int OTS_RBTree_FindIF(struct OTS_RBTree *tree, void *key);
extern OTS_RBTree_Size(struct OTS_RBTree *tree);

// 和迭代器相关函数

/**
 * \code
 * OTS_RBTree *tree=OTS_RBTree_Initialize(sizeof(int), sizeof(char *), OTS_RBTree_int_cmp);
 * OTS_RBTreeIterator *itor = OTS_RBTree_Begin(tree);
 * for (;!OTS_RBTreeItor_Equal(itor, OTS_RBTree_End(tree));OTS_RBTreeItor_Increase(itor)) {
 *  // something to do
 *  
 * }
 * \brief
 * 总体来说就是仿照C++中迭代器的作用
*/

extern struct OTS_RBTreeIterator *OTS_RBTree_End(struct OTS_RBTree *tree);
extern struct OTS_RBTreeIterator *OTS_RBTree_Begin(struct OTS_RBTree *tree);
extern void OTS_RBTreeItor_Increase(struct OTS_RBTreeIterator *itor);
extern void OTS_RBTreeItor_Decrease(struct OTS_RBTreeIterator *itor);
extern void *OTS_RBTreeItor_GetKey(struct OTS_RBTreeIterator *itor);
extern void *OTS_RBTreeItor_GetValue(struct OTS_RBTreeIterator *itor);
extern int OTS_RBTreeItor_Equal(struct OTS_RBTreeIterator *itor1, struct OTS_RBTreeIterator *itor2);

// 提供了基本类型的比较函数

extern int OTS_RBTree_str_cmp(void *, void *);
extern int OTS_RBTree_int_cmp(void *, void *);
extern int OTS_RBTree_float_cmp(void *, void *);
extern int OTS_RBTree_double_cmp(void *, void *);

#ifdef __cplusplus
}
#endif

#endif