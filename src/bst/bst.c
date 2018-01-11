#include <string.h>
#include <stdio.h>
#include "bst.h"

void bst_add(char *word, char *file_path)
{
    if (bst_root == NULL)
    {
        bst_root = malloc(sizeof(BSTNode_t));
        bst_root->word = malloc((strlen(word) + 1) * sizeof(char));
        strcpy(bst_root->word, word);
        bst_root->repeated_count = 1;
        lllist_init(&(bst_root->file_repeat_stat));
        bst_add_repeat_file_to_words_files(bst_root->file_repeat_stat, file_path);
        bst_root->__lc = NULL;
        bst_root->__rc = NULL;
    }
    else
    {
        BSTNode parent = NULL;
        BSTNode current = bst_root;
        while (current)
        {
            parent = current;
            if (strcmp(current->word, word) > 0)
            {
                current = current->__lc;
            }
            else if (strcmp(current->word, word) < 0)
            {
                current = current->__rc;
            }
            else
            {
                current = NULL;
            }

        }

        if (strcmp(parent->word, word) > 0)
        {
            parent->__lc = malloc(sizeof(BSTNode_t));
            parent->__lc->word = malloc((strlen(word) + 1) * sizeof(char));
            strcpy(parent->__lc->word, word);
            lllist_init(&(parent->__lc->file_repeat_stat));
            parent->__lc->__lc = NULL;
            parent->__lc->__rc = NULL;
            parent->__lc->repeated_count = 1;
            bst_add_repeat_file_to_words_files(parent->__lc->file_repeat_stat, file_path);
        }
        else if (strcmp(parent->word, word) < 0)
        {
            parent->__rc = malloc(sizeof(BSTNode_t));
            parent->__rc->word = malloc((strlen(word) + 1) * sizeof(char));
            strcpy(parent->__rc->word, word);
            lllist_init(&(parent->__rc->file_repeat_stat));
            parent->__rc->__lc = NULL;
            parent->__rc->__rc = NULL;
            parent->__rc->repeated_count = 1;
            bst_add_repeat_file_to_words_files(parent->__rc->file_repeat_stat, file_path);
        }
        else
        {
            parent->repeated_count ++;
            bst_add_repeat_file_to_words_files(parent->file_repeat_stat, file_path);
        }
    }
}

void bst_init()
{
    bst_root = NULL;
}



void bst_add_repeat_file_to_words_files(LLList file_repeat_stat, char *file_path)
{
    int compare(LLListData data)
    {
        WordFileRepeatStat s = data;
        return strcmp(s->file_name, file_path);
    }

    WordFileRepeatStat res = lllist_search(file_repeat_stat, &compare);
    if (res == NULL)
    {
        res = malloc(sizeof(WordFileRepeatStat_t));
        res->file_name = malloc((strlen(file_path) + 1) * sizeof(char));
        strcpy(res->file_name, file_path);
        res->repeat = 1;
        lllist_push_front(file_repeat_stat, res);
    }
    else
    {
        res->repeat ++;
    }
}

void bst_inorder()
{
    _bst_inorder(bst_root);
}

void _bst_inorder(BSTNode root)
{
    if (root->__lc)
        _bst_inorder(root->__lc);

    printf("%s repeated(%d) in : ", root->word, root->repeated_count);
    lllist_go_first(root->file_repeat_stat);
    if (lllist_is_empty(root->file_repeat_stat))
        printf("Error is here");
    else
    {
        do
        {
        WordFileRepeatStat s = lllist_get_current(root->file_repeat_stat);
        printf("%s(%d) ", s->file_name, s->repeat);
        }while (lllist_step_forward(root->file_repeat_stat));
    }

    printf("\n");
    if (root->__rc)
        _bst_inorder(root->__rc);
}
