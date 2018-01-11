#include "parse.h"
#include "../util/lllist/lllist.h"
#include "../util/llstack/llstack.h"
#include "../bst/bst.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>


void scan_dir(char * path)
{
    LLList file_list = list_of_content_files(path);
    lllist_go_first(file_list);
    do
    {
        char *file_path = lllist_get_current(file_list);
        void add_to_bst(char *word)
        {
            int compare_in_stop_words(LLListData data)
            {
                char *stop_word_item = data;
                return strcmp(word, stop_word_item);
            }

            LLListData r = lllist_search(stop_words, &compare_in_stop_words);
            if (r == NULL) // Not found in stop words so add it to bst
                bst_add(word, file_path);
        }
        printf("file %s: \n", file_path);
        scan_file(file_path, &add_to_bst);
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

void scan_file(char *path, void (*call_back)(char *))
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File %s not found\n", path);
        exit(-1);
    }
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
                call_back(buffer);
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
    if (c >= 'a' && c <= 'z')
        return c;
    else if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    else
        return '\0';
}

char *new_buffer(char *buffer, int new_buffer_size)
{
    char *new_buff = ALLOC_SRT(new_buffer_size);
    strcpy(new_buff, buffer);
    return new_buff;
}

void scan_stop_words(char *path)
{
    lllist_init(&stop_words);
    void new_stop_words(char *stop_word)
    {
        lllist_push_front(stop_words, stop_word);
    }

    scan_file(path, &new_stop_words);
}
