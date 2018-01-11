cmake_minimum_required(VERSION 3.9)
project(search_engine C)

set(CMAKE_C_STANDARD 11)

add_executable(search_engine src/main.c src/util/lllist/lllist.h src/util/lllist/lllist.c src/util/llstack/llstack.h src/util/llstack/llstack.c src/parse/parse.h src/parse/parse.c src/bst/bst.h src/bst/bst.c src/query/query.h src/query/query.c)