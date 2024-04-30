#include "./rbtree.h"

#include <stdlib.h>
#include <string.h>

enum OTS_RBTreeNodeColor {
    BLACK, RED,
};

struct OTS_RBTreeNode {
    void *key, *value;
    enum OTS_RBTreeNodeColor color;
    struct OTS_RBTreeNode *rchild, *lchild, *parent;
};

struct OTS_RBTreeIterator 
{
    void *key, *value, *pointer;
    struct OTS_RBTreeIterator *prev, *next;
};

// 左旋
static void leftRotate(struct OTS_RBTree *tree, struct OTS_RBTreeNode *node);
// 右旋
static void rightRotate(struct OTS_RBTree *tree, struct OTS_RBTreeNode *node);
// 更换节点颜色
static void exchangeNodeColor(struct OTS_RBTreeNode *node);
// 查找当前节点的叔叔节点
static struct OTS_RBTreeNode *findUncleNode(struct OTS_RBTreeNode *node);
// 这个函数用于插入，不需要考虑红黑树的平衡条件，插入操作和二叉查找树相同
static struct OTS_RBTreeNode *_M_OTS_RBTree_Insert(struct OTS_RBTree *tree, struct OTS_RBTreeNode *root, void *key, void *value);

#define NODE_PARENT(node)   ((node)->parent)
#define NODE_IS_RED(node)   ((node)->color==RED)
#define NODE_IS_BLACK(node) ((node)->color==BLACK)

#define CREATE_RED_NODE(node, key, value) \
    struct OTS_RBTreeNode *node=NULL;\
    do {\
        node = (struct OTS_RBTreeNode *)malloc(sizeof(struct OTS_RBTreeNode)); \
        node->color = RED; node->key = key; node->value = value; \
        node->lchild=node->rchild=node->parent=NULL;  \
    } while(0)


struct OTS_RBTree *OTS_RBTree_Initialize(size_t keylen, size_t valuelen, int (*cmp)(void *, void *)) {
    struct OTS_RBTree *tree = (struct OTS_RBTree *)malloc(sizeof(struct OTS_RBTree));
    tree->nodecmp = cmp; tree->keylen = keylen; tree->valuelen = valuelen; 
    tree->size = 0; tree->root = NULL;
    return tree;
}

int OTS_RBTree_Insert(struct OTS_RBTree *tree, void *key, void *value) {
    struct OTS_RBTreeNode *node = _M_OTS_RBTree_Insert(tree, tree->root, key, value);
    if (!node) return 0;
    struct OTS_RBTreeNode *parent, *gparent;
    while ((parent=NODE_PARENT(node))&&NODE_IS_RED(parent)) {
        gparent = NODE_PARENT(parent);
        if (parent==gparent->lchild) {
            
        }
    }
}

/**
 *    px                                px
 *    /                                 /
 *   x                                 y
 *  / \         ---(左旋)-->          / \
 * lx  y                             x  ry
 *    / \                           / \
 *   ly ry                         lx ly
*/
void leftRotate(struct OTS_RBTree *tree, struct OTS_RBTreeNode *x) {
    struct OTS_RBTreeNode *y = x->rchild;
    if (!y) return; // 如果右子节点为空，无法进行左旋操作

    // 更新子节点的连接关系
    x->rchild = y->lchild;
    if (y->lchild)
        y->lchild->parent = x;

    // 更新y的父节点和x的父节点的连接关系
    y->parent = x->parent;
    if (!x->parent)
        tree->root = y;
    else if (x == x->parent->lchild)
        x->parent->lchild = y;
    else
        x->parent->rchild = y;

    // 更新x和y之间的连接关系
    y->lchild = x;
    x->parent = y;
}

/**
 *      py                          py
 *      /                           /
 *     y                           x
 *    / \       --(右旋)-->       / \
 *   x  ry                       lx  y 
 *  / \                             / \   
 * lx rx                           rx ry 
*/
void rightRotate(struct OTS_RBTree *tree, struct OTS_RBTreeNode *y) {
    struct OTS_RBTreeNode *x = y->lchild;
    if (!x) return;

    // 更新子节点的连接关系
    y->lchild = x->rchild;
    if (x->rchild)
        x->rchild->parent = y;

    // 更新x的父节点和y的父节点的连接关系
    x->parent = y->parent;
    if (!y->parent)
        tree->root = x;
    else if (y == y->parent->lchild)
        y->parent->lchild = x;
    else
        y->parent->rchild = x;

    // 更新y和x之间的连接关系
    x->rchild = y;
    y->parent = x;
}

struct OTS_RBTreeNode *findUncleNode(struct OTS_RBTreeNode *node) {
    if (!node->parent) return NULL;
    struct OTS_RBTreeNode *parent = node->parent;
    if (parent->parent->lchild==parent) {
        return parent->parent->rchild;
    } else {
        return parent->parent->lchild;
    }
}

void exchangeNodeColor(struct OTS_RBTreeNode *node) {
    if (!node) return;
    if (node->color==RED) node->color = BLACK;
    else node->color = RED;
}

struct OTS_RBTreeNode *_M_OTS_RBTree_Insert(struct OTS_RBTree *tree, struct OTS_RBTreeNode *root, void *key, void *value) {
    int (*cmp)(void *, void *) = tree->nodecmp;
    if (!root) {
        CREATE_RED_NODE(node, key, value);
        root = node;
        return root;
    }
    if (cmp(root->key, key) > 0) {
        return _M_OTS_RBTree_Insert(tree, root->rchild, key, value);
    } else if (cmp(root->key, key) < 0) {
        return _M_OTS_RBTree_Insert(tree, root->lchild, key, value);
    } else {
        return NULL;
    }
}

////    几个基本类型的比较函数    ////

int OTS_RBTree_str_cmp(void *elem1, void *elem2) {
    return strcmp((const char *)elem1, (const char *)elem2);
}

int OTS_RBTree_int_cmp(void *elem1, void *elem2) {
    if (*(int *)elem1>*(int *)elem2) {
        return 1;
    } else if (*(int *)elem1<*(int *)elem2) {
        return -1;
    } else
        return 0;
}

int OTS_RBTree_float_cmp(void *elem1, void *elem2) {
    if (*(float *)elem1>*(float *)elem2) 
        return 1;
    else if (*(float *)elem1<*(float *)elem2)
        return -1;
    else 
        return 0;
}

int OTS_RBTree_double_cmp(void *elem1, void *elem2) {
    if (*(double *)elem1>*(double *)elem2) 
        return 1;
    else if (*(double *)elem1<*(double *)elem2)
        return -1;
    else 
        return 0;
}