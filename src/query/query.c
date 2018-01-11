#include <string.h>
#include <stdio.h>
#include "query.h"
#include "../parse/parse.h"

char *read_line()
{
    char c;

    char *buffer;
    int buffer_size = DEFAULT_BUFFER_SIZE;
    int buffer_used = 0;

    buffer = ALLOC_SRT(buffer_size);

    while ((c = fgetc(stdin)) != '\n')
    {
        buffer[buffer_used++] = c;
        if (buffer_used == buffer_size - 2)
        {
            buffer = new_buffer(buffer, buffer_size += 2);
        }
    }

    buffer[buffer_used] = '\0';
    return buffer;
}

void start_query_get()
{
    char *query;

    do
    {
        query = read_line();
        printf("query is : %s\n", query);
    }while (strcmp(query, QUERY_INPUT_TERMINATE_CMD) != 0);
}
