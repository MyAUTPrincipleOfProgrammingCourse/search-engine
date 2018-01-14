#ifndef SEARCH_ENGINE_BST_H
#define SEARCH_ENGINE_BST_H

#include <stdlib.h>
#include <stdbool.h>

typedef void *BSTDataType;

typedef struct _BSTNode{
    BSTDataType __data;
    struct _BSTNode *__lc;
    struct _BSTNode *__rc;
} *BSTNode, BSTNode_t;


typedef struct {
    size_t __size;
    BSTNode __root;
    int (*__cmp_function)(BSTDataType, BSTDataType);
} *BST, BST_t;

void bst_init(BST *, int (*compare_function)(BSTDataType, BSTDataType));
bool bst_insert(BST, BSTDataType);
BSTDataType bst_search(BST, BSTDataType);
void bst_inorder(BST, void (*cb_function)(BSTDataType));
void bst_preorder(BST, void (*cb_function)(BSTDataType));
void bst_postorder(BST, void (*cb_function)(BSTDataType));

#endif //SEARCH_ENGINE_BST_H
