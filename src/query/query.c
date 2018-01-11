#include <string.h>
#include <stdio.h>
#include "query.h"
#include "../parse/parse.h"

int **table;
void (*state_functions[6])(query_char_type);

bool parenthesis;

char *read_line() {
    char c;

    char *buffer;
    int buffer_size = DEFAULT_BUFFER_SIZE;
    int buffer_used = 0;

    buffer = ALLOC_SRT(buffer_size);

    while ((c = fgetc(stdin)) != '\n') {
        buffer[buffer_used++] = c;
        if (buffer_used == buffer_size - 2) {
            buffer = new_buffer(buffer, buffer_size += 2);
        }
    }

    buffer[buffer_used] = '\0';
    return buffer;
}

void start_query_get() {
    char *query;
    bool is_continue = true;

    while (is_continue)
    {

        query = read_line();
        if (strcmp(query, QUERY_INPUT_TERMINATE_CMD) == 0)
        {
            is_continue = false;
        }
        else
        {
            printf("query is : %s\n", query);
            LLList tokens_list = query_tokenize(query);
            lllist_go_first(tokens_list);
            do {
                QueryToken token = lllist_get_current(tokens_list);
                printf("%s type %d\n", token->value, token->type);
            } while (lllist_step_forward(tokens_list));
            bool is_correct = query_error_check(tokens_list);
            if (is_correct)
                printf("Query is correct\n");
            else
                printf("Query is not correct\n");
        }
    }
}

LLList query_tokenize(char *query) {
    LLList tokens_list;

    lllist_init(&tokens_list);

    char *buffer;
    int buffer_size = DEFAULT_BUFFER_SIZE;
    int buffer_used = 0;

    buffer = ALLOC_SRT(buffer_size);

    query_char_type last_char_type = QUERY_TOKEN_TYPE_WHITE_SPACE;

    size_t query_len = strlen(query);

    for (size_t i = 0; i < query_len; i++) {
        char c = query[i];
        query_char_type char_type = get_query_char_type(c);

        if ((last_char_type == char_type) && (char_type == QUERY_TOKEN_TYPE_IDENTIFIER)) {
            buffer[buffer_used++] = c;
            if (buffer_used == buffer_size - 2) {
                buffer = new_buffer(buffer, buffer_size += 2);
            }
        } else if (
                char_type == QUERY_TOKEN_TYPE_UNARY_OPERATOR ||
                char_type == QUERY_TOKEN_TYPE_CLOSE_PARENTHESIS ||
                char_type == QUERY_TOKEN_TYPE_OPEN_PARENTHESIS ||
                char_type == QUERY_TOKEN_TYPE_BINARY_OPERATOR
                ) {
            if (buffer_used != 0) {
                buffer_used = 0;
                buffer_size = DEFAULT_BUFFER_SIZE;
                QueryToken ltoken = malloc(sizeof(QueryToken_t));
                ltoken->value = ALLOC_SRT(strlen(buffer) + 1);
                strcpy(ltoken->value, buffer);
                ltoken->type = QUERY_TOKEN_TYPE_IDENTIFIER;
                lllist_push_front(tokens_list, ltoken);
                buffer = ALLOC_SRT(buffer_size);
            }

            QueryToken token = malloc(sizeof(QueryToken_t));
            token->value = ALLOC_SRT(2);
            sprintf(token->value, "%c", c);
            token->type = char_type;
            lllist_push_front(tokens_list, token);
        } else if (char_type == QUERY_TOKEN_TYPE_WHITE_SPACE) {
            if (buffer_used != 0) {
                buffer_used = 0;
                buffer_size = DEFAULT_BUFFER_SIZE;
                QueryToken token = malloc(sizeof(QueryToken_t));
                token->value = ALLOC_SRT(strlen(buffer) + 1);
                strcpy(token->value, buffer);
                token->type = QUERY_TOKEN_TYPE_IDENTIFIER;
                lllist_push_front(tokens_list, token);
                buffer = ALLOC_SRT(buffer_size);
            }
        }

        if ((last_char_type != QUERY_TOKEN_TYPE_IDENTIFIER) && char_type == QUERY_TOKEN_TYPE_IDENTIFIER) {
            buffer[buffer_used++] = c;
            if (buffer_used == buffer_size - 2) {
                buffer = new_buffer(buffer, buffer_size += 2);
            }
        }

        last_char_type = char_type;
    }

    if (buffer_used != 0) {
        QueryToken token = malloc(sizeof(QueryToken_t));
        token->value = ALLOC_SRT(strlen(buffer) + 1);
        strcpy(token->value, buffer);
        token->type = QUERY_TOKEN_TYPE_IDENTIFIER;
        lllist_push_front(tokens_list, token);
    }
    return tokens_list;
}

query_char_type get_query_char_type(char c) {

    if (c >= 'a' && c <= 'z')
        return QUERY_TOKEN_TYPE_IDENTIFIER;
    else if (c >= 'A' && c <= 'Z')
        return QUERY_TOKEN_TYPE_IDENTIFIER;
    else if (c == '&' || c == '|' || c == '^')
        return QUERY_TOKEN_TYPE_BINARY_OPERATOR;
    else if (c == '(')
        return QUERY_TOKEN_TYPE_OPEN_PARENTHESIS;
    else if (c == ')')
        return QUERY_TOKEN_TYPE_CLOSE_PARENTHESIS;
    else if (c == '!')
        return QUERY_TOKEN_TYPE_UNARY_OPERATOR;
    else
        return QUERY_TOKEN_TYPE_WHITE_SPACE;
}

bool query_error_check(LLList tokens_list)
{
    lllist_go_first(tokens_list);

    int current_state = 0;

    while (current_state != -1)
    {
        QueryToken t = lllist_get_current(tokens_list);

        state_functions[current_state](t->type);
        current_state = table[current_state][t->type];

        if (!lllist_step_forward(tokens_list))
            break;
    }

    if ((current_state != -1)
//        &&
//            (parenthesis == 0)
            )
        return true;
    else
        return false;
}

void s0(query_char_type type)
{
    parenthesis = 0;
}

void s1(query_char_type type)
{
    parenthesis++;
}

void s2(query_char_type type)
{
    printf("in state 2\n");
}

void s3(query_char_type type)
{
    printf("in state 3\n");
}

void s4(query_char_type type)
{
    printf("in state 4\n");
}

void s5(query_char_type type)
{
    parenthesis--;
}

void load_error_check_automata() {
    FILE *automata_file = fopen(ERROR_CHECK_AUTOMATA_FILE_PATH, "r");
    if (automata_file == NULL)
    {
        printf("File %s not found\n", ERROR_CHECK_AUTOMATA_FILE_PATH);
        exit(-1);
    }

    int symbol_type_count;
    int state_count;
    fscanf(automata_file, "%d", &symbol_type_count);
    fscanf(automata_file, "%d", &state_count);

    table = malloc(state_count * sizeof(int *));
    for (int i = 0; i < state_count; i++)
        table[i] = malloc(symbol_type_count * sizeof(int));


    for (int i = 0; i < state_count; i ++)
    {
        for(int j = 0; j < symbol_type_count; j ++)
        {
            fscanf(automata_file, "%d", &table[i][j]);
        }
    }


    state_functions[0] = &s0;
    state_functions[1] = &s1;
    state_functions[2] = &s2;
    state_functions[3] = &s3;
    state_functions[4] = &s4;
    state_functions[5] = &s5;
}
