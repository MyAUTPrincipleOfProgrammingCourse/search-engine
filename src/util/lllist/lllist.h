/**
 * Linked list implementation of Two-way List
 * General purpose with void
 * This list is not random accessible and new item just can be added to the
 * back and front of the list.
 */
#include <stdbool.h>
#include <stdlib.h>

#ifndef SEARCH_ENGINE_LLLIST_H
#define SEARCH_ENGINE_LLLIST_H

typedef void *LLListData;

typedef struct LLListNode{
    LLListData __data;
    struct LLListNode *__ll_last;
    struct LLListNode *__ll_next;
} LLListNodeType, *LLListNode;


typedef struct {
    LLListNode __first_node;
    LLListNode __current_node;
    LLListNode __last_node;
    size_t size;
} *LLList, LLListType;

/**
 * Initial the data structure of list container
 * @param lllist The list container
 */
void lllist_init(LLList *);

/**
 * Returns whether the list is empty or not
 * @param lllist The list to be checked if it is empty or not
 * @return The emptiness of the list
 */
bool lllist_is_empty(LLList);

/**
 * Returns whether the current of list is at last or not
 * @param lllist The list to be checked if it is empty or not
 * @return The emptiness of the list
 */
bool lllist_is_at_last(LLList);

/**
 * Returns whether the current of list is at first or not
 * @param lllist The list to be checked if it is empty or not
 * @return The emptiness of the list
 */
bool lllist_is_at_first(LLList);

/**
 *
 * @param lllist
 * @param lllist_data
 */
void lllist_push_back(LLList, LLListData);

/**
 * Push to the front of the list
 * @param lllist The list
 * @param llList_data The data to be pushed
 */
void lllist_push_front(LLList, LLListData);

/**
 * Get the data from the current node
 * @param lllist The list
 * @return The data of the current node
 */
LLListData lllist_get_current(LLList);

/**
 * Returns the pointer to the current node
 * @param lllist The list
 * @return The pointer of current
 */
LLListNode lllist_get_current_p(LLList);

/**
 * Set the current pointer
 * @param lllist The list
 * @param lllist_node The node to be assigned to current
 * @return
 */
void lllist_set_current_p(LLList, LLListNode);

/**
 * Move the current to the forward and returns false
 * if current is in the last of list
 * @param lllist
 * @return If it can step returns true
 */
bool lllist_step_forward(LLList);

/**
 * Move the current to the backward and returns false
 * if current is in the first of list
 * @param lllist
 * @return If it can step returns true
 */
bool lllist_step_backward(LLList);

/**
 *
 * @param lllist
 */
void lllist_go_last(LLList);

/**
 *
 * @param lllist
 */
void lllist_go_first(LLList lllist);

/**
 *
 * @param lllist1
 * @param lllist2
 */
void lllist_concat(LLList, LLList);

/**
 *
 * @param list1
 * @param list2
 * @param compare_function
 * @return
 */
LLList lllist_union(LLList, LLList, int (*compare_function)(LLListData, LLListData));

/**
 *
 * @param list1
 * @param list2
 * @param compare_function
 * @return
 */
LLListData lllist_search(LLList, LLList, int (*compare_function)(LLListData, LLListData));

/**
 * Release the resources allocated to the lllist
 * @param lllist The lllist that should be un allocate from the memory
 */
void lllist_release(LLList *);

#endif //SEARCH_ENGINE_LLLIST_H