#ifndef _OPENTOOLS_SET_H_
#define _OPENTOOLS_SET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "./internal/rbtree.h"

struct OTS_RBTree;
struct OTS_RBTreeIterator;

typedef struct OTS_RBTree           OTS_Set;
typedef struct OTS_RBTreeIterator   OTS_SetIterator;

/**
 * @brief 由于Set使用RBTree实现，直接使用宏重新定义RBTree的底层函数即可
*/


#define OTS_Set_Initialize(elemlen, cmp)    OTS_RBTree_Initialize(elemlen, elemlen, cmp)
#define OTS_Set_Insert(set, key)    OTS_RBTree_Insert(set, key, key)

#ifdef __cplusplus
}
#endif

#endif