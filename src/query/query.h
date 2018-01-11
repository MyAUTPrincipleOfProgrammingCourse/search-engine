#ifndef SEARCH_ENGINE_QUERY_H
#define SEARCH_ENGINE_QUERY_H

#include "../util/lllist/lllist.h"

#define QUERY_INPUT_TERMINATE_CMD "terminate"

#define QUERY_TOKEN_TYPE_UNARY_OPERATOR 10
#define QUERY_TOKEN_TYPE_BINARY_OPERATOR 20
#define QUERY_TOKEN_TYPE_OPEN_PARENTHESIS 30
#define QUERY_TOKEN_TYPE_CLOSE_PARENTHESIS 40
#define QUERY_TOKEN_TYPE_IDENTIFIER 50
#define QUERY_TOKEN_TYPE_WHITE_SPACE 60

typedef unsigned char query_char_type;

typedef struct {
    query_char_type type;
    char *value;
} *QueryToken, QueryToken_t;

char *read_line();
void start_query_get();
LLList query_tokenize(char *);

query_char_type get_query_char_type(char);

#endif //SEARCH_ENGINE_QUERY_H
