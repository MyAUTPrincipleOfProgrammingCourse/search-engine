#include "bst.h"

void bst_init(BST *bst, int (*compare_function)(BSTDataType, BSTDataType))
{
    (*bst) = malloc(sizeof(BST_t));
    (*bst)->__size = 0;
    (*bst)->__root = NULL;
    (*bst)->__cmp_function = compare_function;
}

bool bst_insert(BST bst, BSTDataType data)
{
    bst-> __size ++;

    BSTNode new_node = malloc(sizeof(BSTNode_t));
    new_node->__lc = NULL;
    new_node->__rc = NULL;
    new_node->__data = data;

    if (bst->__root == NULL)
        bst->__root = new_node;
    else
    {
        BSTNode parent = NULL;
        BSTNode current = bst->__root;
        while (current)
        {
            parent = current;
            if (bst->__cmp_function(current->__data, data) > 0)
            {
                current = current->__lc;
            }
            else if (bst->__cmp_function(current->__data, data) < 0)
            {
                current = current->__rc;
            }
            else
            {
                current = NULL;
            }

        }

        if (bst->__cmp_function(parent->__data, data) > 0)
        {
            parent->__lc = new_node;
            return true;
        }
        else if (bst->__cmp_function(parent->__data, data) < 0)
        {
            parent->__rc = new_node;
            return true;
        }
        else    // Same node found
            return false;
    }
}

void _bst_inorder(BSTNode root, void (*cb_function)(BSTDataType))
{
    if (root)
    {
        _bst_inorder(root->__lc, cb_function);
        cb_function(root->__data);
        _bst_inorder(root->__rc, cb_function);
    }
}
void bst_inorder(BST bst, void (*cb_function)(BSTDataType))
{
    _bst_inorder(bst->__root, cb_function);
}

BSTDataType bst_search(BST bst, BSTDataType data)
{
    BSTNode current = bst->__root;

    while (current)
    {
        if (bst->__cmp_function(current->__data, data) > 0)
        {
            current = current->__lc;
        }
        else if (bst->__cmp_function(current->__data, data) < 0)
        {
            current = current->__rc;
        }
        else
        {
            return current->__data;
        }

    }

    return NULL;
}
