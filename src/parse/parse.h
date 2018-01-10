#ifndef SEARCH_ENGINE_PARSE_H
#define SEARCH_ENGINE_PARSE_H

#include "../util/lllist/lllist.h"

#define DEFAULT_BUFFER_SIZE 10

void scan_dir(char *);
void scan_file(char *);
LLList list_of_content_files(char *);
char valid_char(char);

char *new_buffer(char *, int);
#endif //SEARCH_ENGINE_PARSE_H
