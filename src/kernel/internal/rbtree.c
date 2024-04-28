#include "./rbtree.h"

#include <stdlib.h>
#include <string.h>

struct OTS_RBTreeNode {
    void *key, *value;
    enum OTS_RBTreeNodeColor color;
    struct OTS_RBTreeNode *rchild, *lchild, *parent;
};

// 红黑树节点左旋
static void leftRotate(struct OTS_RBTreeNode *node);
// 红黑树节点右旋
static void rightRotate(struct OTS_RBTreeNode *node);
// 更换节点颜色
static void exchangeNodeColor(struct OTS_RBTreeNode *node);
// 查找当前节点的叔叔节点
static struct OTS_RBTreeNode *findUncleNode(struct OTS_RBTreeNode *node);
// 这个函数用于插入，不需要考虑红黑树的平衡条件，插入操作和二叉查找树相同
static struct OTS_RBTreeNode *_M_OTS_RBTree_Insert(struct OTS_RBTree *tree, struct OTS_RBTreeNode *root, void *key, void *value);

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
    // 如果新插入节点是根节点
    if (tree->size==0) {
        tree->size ++ ;
        node->color = BLACK;
        return 1; 
    }
    // 如果新插入节点的叔叔节点是红色
    struct OTS_RBTreeNode *uncleNode = findUncleNode(node);
    if (uncleNode->color==RED) {
        struct OTS_RBTreeNode *cur = node;
        do {
            exchangeNodeColor(cur->parent);
            exchangeNodeColor(findUncleNode(cur));
            exchangeNodeColor(cur->parent->parent);
        } while (findUncleNode(cur)&&findUncleNode(cur)->color!=RED);
        if (cur==tree->root) cur->color = BLACK;
        return 1;
    }
}

void leftRotate(struct OTS_RBTreeNode *node) {

}

void rightRotate(struct OTS_RBTreeNode *node) {

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

}