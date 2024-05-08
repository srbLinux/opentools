#ifndef _OPENTOOLS_HASH_H_
#define _OPENTOOLS_HASH_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OTS_Hash;
struct OTS_List;
struct OTS_HashNode;

struct OTS_Hash 
{
    int size;
    size_t elelen;
    struct OTS_List *list_head;
    int (*compute)(void *data);
};

extern struct OTS_Hash *OTS_Hash_Initialize(size_t elelen, int (*compute)(void *));

#ifdef __cplusplus
}
#endif

#endif