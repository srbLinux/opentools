#include "list.h"

#include <stdlib.h>

typedef struct OTS_List List;

static void List_Print(List *list);

typedef struct OTS_ListNode 
{
    void *data;
    struct OTS_ListNode *prev, *next;
} LNode;

static LNode *LNode_Initialize(void *data);
static void LNode_Delete(LNode *node);
static int LNode_Empty(LNode *node);

struct OTS_List *OTS_List_Initialize(size_t elelen) {
    List *list = (List *)malloc(sizeof(List));
    list->elelen = elelen; list->size = 0;
    list->head = (LNode *)malloc(sizeof(LNode));
    list->head->next = list->head; list->head->prev = list->head;
}

int OTS_List_Empty(struct OTS_List *list) {
    return (list->head->next==list->head);
}

int OTS_List_Insert(struct OTS_List *list, int pos, void *data) {
    if (!list) return 0;
    if (pos<0||pos>=list->size) return 0;
    LNode *head = list->head, *insert=NULL, *prev=NULL, *next=NULL;
    prev = head;
    for (int i=0;i<pos;i++) {
        prev = prev->next; next=prev->next;
    }
    insert = LNode_Initialize(data);
    insert->next = prev->next; insert->prev = next->prev;
    prev->next = insert; next->prev = insert;
    return 1;
}

void *OTS_List_Erase1(struct OTS_List *list, int pos) {
    
}

LNode *LNode_Initialize(void *data) {
    LNode *node = (LNode *)malloc(sizeof(LNode));
    node->data = data; node->next=node->prev = NULL;
}

int LNode_Empty(LNode *node) {
    return (node->data==NULL);
}

void LNode_Delete(LNode *node) {
    node->next=node->prev=NULL;
    free(node);
}