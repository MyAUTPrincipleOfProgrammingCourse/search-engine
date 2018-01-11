#include "parse/parse.h"
#include "bst/bst.h"
#include "query/query.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    load_error_check_automata();

    bst_init();
    scan_stop_words("/home/ahmad/CLionProjects/search-engine/resource/input/stopwords.txt");
    scan_dir("/home/ahmad/CLionProjects/search-engine/resource/input");
    bst_inorder();
    start_query_get();

    return 0;
}