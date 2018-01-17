#include <string.h>
#include <stdio.h>
#include "word.h"
#include "../util/bst/bst.h"
#include "../parse/parse.h"

BST words_bst;
BST stop_words_bst;

LLList universe_file_list;

int word_cmp(Word w1, Word w2)
{
    return strcmp(w1->word, w2->word);
}

void init_word_collection(char *path)
{
    // Init a bst for words
    bst_init(&words_bst, &word_cmp);

    void new_word(char *word, char *file)
    {
        if (!bst_search(stop_words_bst, word))              // Check if the word is not a stop word
        {
            Word temp = malloc(sizeof(Word_t));
            temp->word = malloc((strlen(word) + 1 ) * sizeof(char));
            strcpy(temp->word, word);

            Word w = bst_search(words_bst, temp);           // Search for the word if it is existed in the bst
            if (w == NULL)                                  // Check if the word is not found
            {
                Word new_word = malloc(sizeof(Word_t));     // Make the word structure
                new_word->word = malloc((strlen(word) + 1 ) * sizeof(char)); // Allocate the space for word string
                strcpy(new_word->word, word);               // Copy to the word
                lllist_init(&(new_word->files_list));       // Init the list of files occurred
                WordFileRepeatStat repeat = malloc(sizeof(WordFileRepeatStat_t));
                repeat->file_name = malloc((strlen(file) + 1 ) * sizeof(char));
                strcpy(repeat->file_name, file);
                repeat->repeat = 1;
                lllist_push_front(new_word->files_list, repeat);
                bst_insert(words_bst, new_word);
            }
            else
            {
                // Function for check if this file is in list
                int compare_file_name(WordFileRepeatStat repeat) { return strcmp(repeat->file_name, file); }


                WordFileRepeatStat repeat_stat = lllist_search(w->files_list, &compare_file_name);

                if (repeat_stat == NULL)    // This word is not repeated in this file
                {
                    repeat_stat = malloc(sizeof(WordFileRepeatStat_t));
                    repeat_stat->file_name = malloc((strlen(file) + 1 ) * sizeof(char));
                    strcpy(repeat_stat->file_name, file);
                    repeat_stat->repeat = 1;
                    lllist_push_front(w->files_list, repeat_stat);
                }
                else                        // This word was founded in this file before
                {
                    repeat_stat->repeat ++;
                }
            }
        }
    }

    // Scan the dir path and when a new word found, "new_word" function is called
    scan_dir(path, &new_word);

    make_universe_set_file_list();
}

void init_stop_word_collection(char *path)
{
    // Init a bst for stop words
    bst_init(&stop_words_bst, &strcmp);

    void new_stop_words(char *stop_word)
    {
        bst_insert(stop_words_bst, stop_word);
    }

    scan_file(path, &new_stop_words);
}

void print_word_collection()
{
    void print_function(Word word)
    {
        printf("%s ", word->word);
        lllist_go_first(word->files_list);
        do
        {
            WordFileRepeatStat repeat_stat = lllist_get_current(word->files_list);
            printf("%s(%d) ", repeat_stat->file_name, repeat_stat->repeat);
        }
        while (lllist_step_forward(word->files_list));
        printf("\n");
    }

    bst_inorder(words_bst, &print_function);
}

void print_stop_word_collection()
{
    void print_function(char *word)
    {
        printf("%s\n", word);
    }

    bst_inorder(stop_words_bst, &print_function);
}

LLList get_word_list(char *word)
{
    Word t = malloc(sizeof(Word_t));
    t->word = malloc((strlen(word) + 1) * sizeof(char));
    strcpy(t->word, word);
    Word  w = bst_search(words_bst, t);

    if (w)
    {
        printf("Word %s is founded\n", word);
        return w->files_list;
    }
    else
    {
        printf("Word %s is NOT founded\n", word);
        LLList empty_list;
        lllist_init(&empty_list);
        return empty_list;
    }
}


void make_universe_set_file_list()
{
    /**
     * This function will make the universe set of files list for the not operator in query evaluation
     */
    extern LLList file_list;

    lllist_init(&universe_file_list);

    lllist_go_first(file_list);
    do
    {
        char *s = lllist_get_current(file_list);
        WordFileRepeatStat r = malloc(sizeof(WordFileRepeatStat_t));
        r->file_name = malloc((strlen(s) + 1) * sizeof(char));
        strcpy(r->file_name, s);
        r->repeat = 0;
        lllist_push_front(universe_file_list, r);
    }
    while (lllist_step_forward(file_list));
}

int file_repaet_cmp(WordFileRepeatStat s1, WordFileRepeatStat s2)
{
    return strcmp(s1->file_name, s2->file_name);
}
