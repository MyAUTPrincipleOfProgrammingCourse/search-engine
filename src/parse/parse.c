#include "parse.h"
#include "../util/lllist/lllist.h"
#include "../util/llstack/llstack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#define ALLOC_SRT(N) malloc(N * sizeof(char))

void scan_dir(char * path)
{
    LLList file_list = list_of_content_files(path);
    lllist_go_first(file_list);
    do
    {
        printf("file %s: \n", lllist_get_current(file_list));
        scan_file(lllist_get_current(file_list));
    }
    while (lllist_step_forward(file_list));

}

LLList list_of_content_files(char *path)
{
    // List for storing the path of all content files
    LLList files_list;
    lllist_init(&files_list);

    // Stack for
    LLStack dir_stack;
    llstack_init(&dir_stack);

#if defined(__unix__) || defined(__unix) || \
        (defined(__APPLE__) && defined(__MACH__))   // To check the os
#include <dirent.h>


    llstack_push(dir_stack, path);

    char *current_path;

    while ((current_path = llstack_top_pop(dir_stack)))
    {
        DIR * d = opendir(current_path);                                // open the path

        struct dirent *dir;                                             // The directory entry

        if (d == NULL) return files_list;                               // if was not able return empty list


        while ((dir = readdir(d)) != NULL)                              // if we were able to read somehting from the directory
        {
            if(dir-> d_type != DT_DIR)                                  // if the type is not directory it is a file
            {
                char *f_path = ALLOC_SRT(strlen(current_path) + strlen(dir->d_name) + 2);
                sprintf(f_path, "%s/%s", current_path, dir->d_name);
                lllist_push_front(files_list, f_path);
            }
            else
            if((dir -> d_type == DT_DIR) &&                             // Check if the type is not directory
               (strcmp(dir->d_name, ".") != 0) &&                  // Check if the item is not myself
               (strcmp(dir->d_name, "..") != 0) )                  // Check if the item is not my parent
            {
                char *d_path = ALLOC_SRT(strlen(current_path) + strlen(dir->d_name) + 2);
                sprintf(d_path, "%s/%s", current_path, dir->d_name);
                llstack_push(dir_stack, d_path);
            }
        }
    }
#else
    // TODO implement for other OSs
#endif

    llstack_release(dir_stack);

    return files_list;
}

void scan_file(char *path)
{
    FILE *file = fopen(path, "r");
    char c;

    char *buffer;
    int buffer_size = DEFAULT_BUFFER_SIZE;
    int buffer_used = 0;

    buffer = ALLOC_SRT(buffer_size);

    while ((c = fgetc(file)) != EOF)
    {

        c = valid_char(c);
        if (c != '\0')
        {
            buffer[buffer_used++] = c;
//            printf("%c", c);
            if (buffer_used == buffer_size - 2)
            {
                buffer = new_buffer(buffer, buffer_size += 2);
            }
        }
        else
        {
            if (buffer_used != 0)
            {
                buffer[buffer_used] = '\0';
                printf("%s\n", buffer);
                buffer_size = DEFAULT_BUFFER_SIZE;
                buffer_used = 0;
                buffer = ALLOC_SRT(buffer_size);
            }
        }
    }
    fclose(file);
}

char valid_char(char c)
{
    if ((c >= 'a' && c <= 'z') || ((c >= 'A' && c <= 'Z')))
        return c;
    else
        return '\0';
}

char *new_buffer(char *buffer, int new_buffer_size)
{
    char *new_buff = ALLOC_SRT(new_buffer_size);
    strcpy(new_buff, buffer);
    free(buffer);
    return new_buff;
}
