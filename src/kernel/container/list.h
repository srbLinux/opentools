#ifndef _OPENTOOLS_LIST_H_
#define _OPENTOOLS_LIST_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OTS_List;
struct OTS_ListNode;
struct OTS_ListIerator;

/**
 * @brief 实现是循环双链表
*/
struct OTS_List 
{
    size_t elelen, size;
    struct OTS_ListNode *head;
};

extern struct OTS_List *OTS_List_Initialize(size_t elelen);
extern size_t OTS_List_Size(struct OTS_List *list);
extern int OTS_List_Empty(struct OTS_List *list);
extern void *OTS_List_Popback(struct OTS_List *list);
extern void *OTS_List_Popfront(struct OTS_List *list);
extern void *OTS_List_Erase1(struct OTS_List *list, int pos);
extern void *OTS_List_Erase2(struct OTS_List *list, void *data);
extern void OTS_List_Pushback(struct OTS_List *list, void *data);
extern void OTS_List_Pushfront(struct OTS_List *list, void *data);
extern int OTS_List_Insert(struct OTS_List *list, int pos, void *data);

#ifdef __cplusplus
}
#endif

#endif