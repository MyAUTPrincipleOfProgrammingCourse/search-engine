#ifndef SEARCH_ENGINE_QUERY_H
#define SEARCH_ENGINE_QUERY_H

#include "../util/lllist/lllist.h"

#define QUERY_INPUT_TERMINATE_CMD "terminate"

#define QUERY_TOKEN_TYPE_UNARY_OPERATOR 2
#define QUERY_TOKEN_TYPE_BINARY_OPERATOR 1
#define QUERY_TOKEN_TYPE_OPEN_PARENTHESIS 3
#define QUERY_TOKEN_TYPE_CLOSE_PARENTHESIS 4
#define QUERY_TOKEN_TYPE_IDENTIFIER 0
#define QUERY_TOKEN_TYPE_WHITE_SPACE 5

#define ERROR_CHECK_AUTOMATA_FILE_PATH "../resource/automata-table"

typedef unsigned char query_char_type;

typedef struct {
    query_char_type type;
    char *value;
} *QueryToken, QueryToken_t;

void load_error_check_automata();
bool query_error_check(LLList);
char *read_line();
void start_query_get();
LLList query_tokenize(char *);

LLList query_evaluate(LLList);

query_char_type get_query_char_type(char);

#endif //SEARCH_ENGINE_QUERY_H
