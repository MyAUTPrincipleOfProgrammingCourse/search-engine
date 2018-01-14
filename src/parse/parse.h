#ifndef SEARCH_ENGINE_PARSE_H
#define SEARCH_ENGINE_PARSE_H

#include "../util/lllist/lllist.h"

#define ALLOC_SRT(N) malloc(N * sizeof(char))

#define DEFAULT_BUFFER_SIZE 10

LLList stop_words;

void scan_dir(char * path, void (*new_word_found_file_cb)(char *word, char *file_path));
void scan_file(char *, void (*call_back)(char *));

LLList list_of_content_files(char *);
char valid_char(char);

char *new_buffer(char *, int);
#endif //SEARCH_ENGINE_PARSE_H
