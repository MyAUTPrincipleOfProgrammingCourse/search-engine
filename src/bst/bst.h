#ifndef SEARCH_ENGINE_BST_H
#define SEARCH_ENGINE_BST_H

#include "../util/lllist/lllist.h"

typedef struct _BSTNode{
    char *word;
    LLList file_repeat_stat;
    struct _BSTNode *__lc;
    struct _BSTNode *__rc;
} *BSTNode, BSTNode_t;

typedef struct {
    char *file_name;
    int repeat;
} *WordFileRepeatStat, WordFileRepeatStat_t;

BSTNode root;

bst_add(char *, char *);

#endif //SEARCH_ENGINE_BST_H
