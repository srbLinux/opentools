#include "./rbtree.h"

#include <stdlib.h>
#include <string.h>

enum OTS_RBTreeNodeColor {
    BLACK, RED,
};

// 红黑树的节点
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
// 这个函数用于删除单个元素
static int _M_OTS_RBTree_Delete(struct OTS_RBTree *tree, void *key);
// 这个函数用于查找
static struct OTS_RBTreeNode *_M_OTS_RBTree_Find(struct OTS_RBTreeNode *node, void *key, int (*cmp)(void *, void *));
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

int OTS_RBTree_FindIF(struct OTS_RBTree *tree, void *key) {
    return (_M_OTS_RBTree_Find(tree->root, key, tree->nodecmp)!=NULL);
}

void *OTS_RBTree_Find(struct OTS_RBTree *tree, void *key) {
    struct OTS_RBTreeNode *node = _M_OTS_RBTree_Find(tree->root, key, tree->nodecmp);
    if (node) return node->value;
    else return NULL;
}

int OTS_RBTree_Insert(struct OTS_RBTree *tree, void *key, void *value) {
    struct OTS_RBTreeNode *node = _M_OTS_RBTree_Insert(tree, tree->root, key, value);
    // 如果插入失败返回0
    if (!node) return 0;
    tree->size ++ ;
    if (node==tree->root) {
        tree->root->color = BLACK;
        return 1;
    }

    return 1;
}

/**
 * 左旋
 *              parent                                      node
 *              /    \                                      /  \      
 *          node    p_right         --(右旋)-->         n_left parent
 *          /  \                                               /   \  
 *     n_left  n_right                                    n_right  p_right
*/
void leftRotate(struct OTS_RBTree *tree, struct OTS_RBTreeNode *node) {
    struct OTS_RBTreeNode *parent = node->parent;
    if (!parent) return;
    struct OTS_RBTreeNode *n_right = node->rchild;
    struct OTS_RBTreeNode *gparent = parent->parent;
    parent->lchild = n_right; n_right->parent = parent;
    if (gparent) { // 代表parent是红黑树的根节点，node将会旋转到树的根节点
        if (parent==gparent->lchild) 
            gparent->lchild = node;
        else 
            gparent->rchild = node;
    } else {
        node->parent = NULL; tree->root = node;
    }
    node->rchild = parent; parent->parent = node;
}

/**
 * 右旋
 *        parent                                      node
 *        /    \                                      /  \
 *   p_left    node         --(左旋)-->          parent   n_right
 *             /  \                              /    \
 *        n_left  n_right                   p_left    n_left
*/
void rightRotate(struct OTS_RBTree *tree, struct OTS_RBTreeNode *node) {
    struct OTS_RBTreeNode *parent = node->parent;
    // 如果没有父节点，无法进行左旋
    if (!parent) return;
    struct OTS_RBTreeNode *n_left = node->lchild;
    struct OTS_RBTreeNode *gparent = parent->parent;
    // step1: 将n_left节点挂载在parent节点的右孩子上
    n_left->parent = parent; parent->rchild = n_left;
    // step2: 将parent节点挂载在node节点的左孩子上，同时将node作为gparent节点的孩子
    if (gparent) {
        if (gparent->lchild==parent) {
            gparent->lchild = node;
        } else 
            gparent->rchild = node;
    } else { // 如果原来的parent就是root节点
        node->parent = NULL; tree->root = node;
    }
    node->lchild = parent; parent->parent = node;
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

struct OTS_RBTreeNode *_M_OTS_RBTree_Find(struct OTS_RBTreeNode *node, void *key, int (*cmp)(void *, void *)) {
    if (!node) return NULL;
    if (node&&node->key==key) return node;
    if (cmp(node->key, key)>0) return _M_OTS_RBTree_Find(node->lchild, key, cmp);
    else if (cmp(node->key, key)<0) return _M_OTS_RBTree_Find(node->rchild, key, cmp);
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