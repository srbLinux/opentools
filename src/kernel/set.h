#ifndef _OPENTOOLS_SET_H_
#define _OPENTOOLS_SET_H_

#ifdef __cplusplus
extern "C" {
#endif

struct OTS_RBTree;
struct OTS_RBTreeNode;

typedef struct _OTS_Set {
    struct OTS_RBTreeNode *node;
} OTS_Set;

typedef struct _OTS_SetIterator {

} OTS_SetIterator;

OTS_SetIterator *OTS_Set_Begin(OTS_Set *set);
OTS_SetIterator *OTS_Set_End(OTS_Set *set);
void OTS_Set_Add(OTS_SetIterator *itor);

#ifdef __cplusplus
}
#endif

#endif