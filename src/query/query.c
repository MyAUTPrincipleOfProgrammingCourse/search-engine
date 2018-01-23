#include <string.h>
#include <stdio.h>
#include "query.h"
#include "../parse/parse.h"
#include "../util/llstack/llstack.h"
#include "../collection/word.h"
#include <limits.h>

//#define CHECK_IF_QUERY_IS_CORRECT

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
#ifdef CHECK_IF_QUERY_IS_CORRECT
            if (is_correct)
#endif
            {
                printf("Query is correct.\nEvaluating the result of query:\n");
                LLList result = query_evaluate(tokens_list);
                printf("File path\t\t\t\t\tRepeat count\n");
                if (!lllist_is_empty(result))
                {
                    lllist_go_first(result);
                    do
                    {
                        WordFileRepeatStat s = lllist_get_current(result);
                        printf("%s\t\t\t%d\n",s->file_name, s->repeat);
                    }
                    while (lllist_step_forward(result));
                }
            }
#ifdef CHECK_IF_QUERY_IS_CORRECT
            else
                printf("Query is not correct\n");
#endif
        }
    }
}

LLList query_tokenize(char *query) {
    LLList tokens_list;             // List of tokens of query

    lllist_init(&tokens_list);      // Init the list of tokens

    char *buffer;                   // Buffer for tokenizing the query
    int buffer_size = DEFAULT_BUFFER_SIZE;  // Size of the buffer is the default for the start of tokenizing
    int buffer_used = 0;            // Variable to indicate the usage of buffer

    buffer = ALLOC_SRT(buffer_size);    // Allocate the memory for buffer

    query_char_type last_char_type = QUERY_TOKEN_TYPE_WHITE_SPACE;

    size_t query_len = strlen(query);

    for (size_t i = 0; i < query_len; i++) {
        char c = query[i];

        query_char_type char_type = get_query_char_type(c);

        if ((last_char_type == char_type) && (char_type == QUERY_TOKEN_TYPE_IDENTIFIER)) {
            buffer[buffer_used++] = ((c >= 'a') && (c <= 'z'))? c : (c - 'A' + 'a');

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
                buffer[buffer_used] = '\0';
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
                buffer[buffer_used] = '\0';
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
            buffer[buffer_used++] = ((c >= 'a') && (c <= 'z'))? c : (c - 'A' + 'a');
            if (buffer_used == buffer_size - 2) {
                buffer = new_buffer(buffer, buffer_size += 2);
            }
        }

        last_char_type = char_type;
    }

    if (buffer_used != 0) {
        buffer[buffer_used] = '\0';
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

    QueryToken t;
    while (current_state != -1)
    {
        t = lllist_get_current(tokens_list);

        state_functions[current_state](t->type);
        current_state = table[current_state][t->type];

        if (!lllist_step_forward(tokens_list))
            break;
    }

    if (current_state != -1)
        state_functions[current_state](t->type);

    if (parenthesis != 0)
    {
        printf("Close & Open parenthesis are not the same.\n");
        return false;
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
    printf("Query lexeme check in state 0\n");
}

void s1(query_char_type type)
{
    parenthesis++;
    printf("Query lexeme check in state 1\n");
}

void s2(query_char_type type)
{
    printf("Query lexeme check in state 2\n");
}

void s3(query_char_type type)
{
    printf("Query lexeme check in state 3\n");
}

void s4(query_char_type type)
{
    printf("Query lexeme check in state 4\n");
}

void s5(query_char_type type)
{
    parenthesis--;
    printf("Query lexeme check in state 5\n");
}

void load_error_check_automata()
{
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

/**
 * Function of the operator priority
 * @param operator
 * @return
 */
int op_pri(char *operator)
{
    if      (strcmp(operator, "(") == 0) return INT_MIN;
    else if (strcmp(operator, "&") == 0) return 5;
    else if (strcmp(operator, "|") == 0) return 3;
    else if (strcmp(operator, "^") == 0) return 4;
    else if (strcmp(operator, ")") == 0) return INT_MAX;
}

LLList evaluate_exp(LLList operand1, LLList operand2, char *operator)
{
    if      (strcmp(operator, "&") == 0) return lllist_intersect(operand1, operand2, &file_repeat_cmp);
    else if (strcmp(operator, "|") == 0) return lllist_union(operand1, operand2, &file_repeat_cmp);
    else if (strcmp(operator, "^") == 0) return lllist_delta(operand1, operand2, &file_repeat_cmp);
}

LLList query_evaluate(LLList tokens)
{
    typedef struct Operator
    {
        char *oper;
        int priority;
        bool _is_not;
    } *Operator, Operator_t;


    LLStack operand_stack;
    LLStack operator_stack;

    llstack_init(&operand_stack);           // Init the operands stack
    llstack_init(&operator_stack);          // Init the operator stack

    lllist_go_first(tokens);                // Set the pointer of the tokens list to the first

    bool is_not_seen = false;



    int cmp(WordFileRepeatStat s1, WordFileRepeatStat s2) { return strcmp(s1->file_name, s2->file_name); }

    extern LLList universe_file_list;

    do
    {
        QueryToken t = lllist_get_current(tokens);

        if (t->type == QUERY_TOKEN_TYPE_UNARY_OPERATOR)
            is_not_seen = !is_not_seen;
        else if (t->type == QUERY_TOKEN_TYPE_IDENTIFIER)
        {
            if (is_not_seen == true)
            {
                llstack_push(operand_stack,
                             lllist_complement(get_word_list(t->value), universe_file_list, &cmp)
                );
                is_not_seen = false;
            }
            else
            {
                LLList list1 = get_word_list(t->value);

//                lllist_go_first(list1);
//                do
//                {
//                    WordFileRepeatStat s = lllist_get_current(list1);
//                    printf("%s\t\t\t%d",s->file_name, s->repeat);
//                }
//                while (lllist_step_forward(list1));

                llstack_push(operand_stack, get_word_list(t->value));
            }
        }
        else if (t->type == QUERY_TOKEN_TYPE_BINARY_OPERATOR)
        {
            if (llstack_is_empty(operator_stack))
            {
                Operator o = malloc(sizeof(Operator_t));
                o->oper = t->value;
                o->priority = op_pri(t->value);

                llstack_push(operator_stack, o);
            }
            else
            {
                while (!llstack_is_empty(operand_stack))
                {
                    if (((Operator)llstack_top(operator_stack))->priority < op_pri(t->value))
                        break;

                    LLList operand1, operand2, result;
                    operand2 = llstack_top_pop(operand_stack);
                    operand1 = llstack_top_pop(operand_stack);
                    Operator operator = llstack_top_pop(operator_stack);
                    result = evaluate_exp(operand1, operand2, operator->oper);
                    llstack_push(operand_stack, result);
                }

                Operator o = malloc(sizeof(Operator_t));
                o->oper = t->value;
                o->priority = op_pri(t->value);

                llstack_push(operator_stack, o);
            }
        }
        else if (t->type == QUERY_TOKEN_TYPE_OPEN_PARENTHESIS)
        {
            Operator o = malloc(sizeof(Operator_t));
            o->oper = malloc((strlen(t->value) + 1) * sizeof(char));
            strcpy(o->oper, t->value);
            o->priority = op_pri(t->value);
            o->_is_not = is_not_seen;
            llstack_push(operator_stack, o);
        }
        else if (t->type == QUERY_TOKEN_TYPE_CLOSE_PARENTHESIS)
        {
            while (strcmp(((Operator)llstack_top(operator_stack))->oper, "(") != 0)
            {
                LLList operand1, operand2, result;
                operand2 = llstack_top_pop(operand_stack);
                operand1 = llstack_top_pop(operand_stack);
                Operator operator = llstack_top_pop(operator_stack);
                result = evaluate_exp(operand1, operand2, operator->oper);
                llstack_push(operand_stack, result);
            }

            if (((Operator)llstack_top(operator_stack))->_is_not)
                llstack_push(operand_stack, lllist_complement(llstack_top_pop(operand_stack), universe_file_list, &cmp));
            llstack_pop(operator_stack);
        }
    }
    while (lllist_step_forward(tokens));

    while (!llstack_is_empty(operator_stack))
    {
        LLList operand1, operand2, result;
        operand2 = llstack_top_pop(operand_stack);
        operand1 = llstack_top_pop(operand_stack);
        Operator operator = llstack_top_pop(operator_stack);
        result = evaluate_exp(operand1, operand2, operator->oper);
        llstack_push(operand_stack, result);
    }

//    if (!lllist_is_empty(operand_stack))
//    {
//        lllist_go_first(llstack_top(operand_stack));
//        do
//        {
//            WordFileRepeatStat s = lllist_get_current(llstack_top(operand_stack));
//            printf("%s\t\t\t%d",s->file_name, s->repeat);
//        }
//        while (lllist_step_forward(llstack_top(operand_stack)));
//    }

    return llstack_top_pop(operand_stack);
}
