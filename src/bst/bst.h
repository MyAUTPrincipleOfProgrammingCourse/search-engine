#ifndef SEARCH_ENGINE_BST_H
#define SEARCH_ENGINE_BST_H

#include "../util/lllist/lllist.h"

typedef struct _BSTNode{
    char *word;
    int repeated_count;
    LLList file_repeat_stat;
    struct _BSTNode *__lc;
    struct _BSTNode *__rc;
} *BSTNode, BSTNode_t;

typedef struct {
    char *file_name;
    int repeat;
} *WordFileRepeatStat, WordFileRepeatStat_t;

BSTNode bst_root;

void bst_init();
void bst_add(char *, char *);
void bst_inorder();
void _bst_inorder(BSTNode);
void bst_add_repeat_file_to_words_files(LLList, char *);
#endif //SEARCH_ENGINE_BST_H
