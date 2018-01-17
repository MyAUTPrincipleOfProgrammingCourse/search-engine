#ifndef SEARCH_ENGINE_WORD_H
#define SEARCH_ENGINE_WORD_H

#include "../util/lllist/lllist.h"

typedef struct {
    char *word;
    LLList files_list;
} *Word, Word_t;

typedef struct {
    char *file_name;
    int repeat;
} *WordFileRepeatStat, WordFileRepeatStat_t;

void init_word_collection(char *);
void init_stop_word_collection(char *);

void print_word_collection();
void print_stop_word_collection();

LLList get_word_list(char *);

void make_universe_set_file_list();

int word_cmp(Word, Word);

int file_repeat_cmp(WordFileRepeatStat, WordFileRepeatStat);

#endif //SEARCH_ENGINE_WORD_H
