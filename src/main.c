#include "parse/parse.h"
#include "query/query.h"
#include <stdio.h>
#include "collection/word.h"

int main(int argc, char **argv)
{
    load_error_check_automata();        // Load the automata table from the file "resource/automata-table"

    char *files_path = "../resource/input/content"; // Path of the directory of the files
    char *stop_words_path = "../resource/input/stopwords.txt";                           // Path of the stop words file

    init_stop_word_collection(stop_words_path);                                          // Init the bst of stop words
    init_word_collection(files_path);                                                    // Init the bst of words

    // Printing the collection
    printf("Stop words:\n");
    print_stop_word_collection();                                                        // Printing the stop words

    printf("Stop words:\n");
    print_word_collection();                                                             // Printing the words

    start_query_get();                                                                   // Start getting the query from the user

    return EXIT_SUCCESS;
}