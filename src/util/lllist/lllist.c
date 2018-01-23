#include <stdlib.h>
#include "lllist.h"

void lllist_init(LLList *lllist)
{
    (*lllist) = malloc(sizeof(LLListType));
    (*lllist)->size = 0;
    (*lllist)->__last_node =
    (*lllist)->__current_node =
    (*lllist)->__first_node = NULL;
}

bool lllist_is_empty(LLList lllist)
{
    if (lllist == NULL)
        return true;
    else
        return ((lllist->__first_node == NULL) && (lllist->__last_node == NULL)) || (lllist->size == 0);
}

void lllist_push_back(LLList lllist, LLListData lllist_data)
{
    LLListNode node = malloc(sizeof(LLListNodeType));
    node->__data = lllist_data;
    node->__ll_last = NULL;

    if (lllist->__first_node == NULL)        // List is empty
    {
        node->__ll_next = NULL;
        lllist->__last_node = node;
        lllist->__first_node = node;
        lllist->__current_node = node;
    }
    else
    {
        node->__ll_next = lllist->__first_node;
        lllist->__first_node->__ll_last = node;
        lllist->__first_node = node;
    }

    lllist->size ++;
}

void lllist_push_front(LLList lllist, LLListData lllist_data)
{
    LLListNode node = malloc(sizeof(LLListNodeType));
    node->__data = lllist_data;
    node->__ll_next = NULL;

    if (lllist->__last_node == NULL)        // List is empty
    {
        node->__ll_last = NULL;
        lllist->__last_node = node;
        lllist->__first_node = node;
        lllist->__current_node = node;
    }
    else
    {
        node->__ll_last = lllist->__last_node;
        lllist->__last_node->__ll_next = node;
        lllist->__last_node = node;
    }

    lllist->size ++;
}

LLListData lllist_get_current(LLList lllist)
{
    return lllist->__current_node->__data;
}

LLListNode lllist_get_current_p(LLList lllist)
{
    return lllist->__current_node;
}

void lllist_set_current_p(LLList lllist, LLListNode lllist_node)
{
    lllist->__current_node = lllist_node;
}

bool lllist_step_forward(LLList lllist)
{
    if (lllist->__current_node == lllist->__last_node) return false;
    lllist->__current_node = lllist->__current_node->__ll_next;
    return true;
}

bool lllist_step_backward(LLList lllist)
{
    if (lllist->__current_node == lllist->__first_node) return false;
    lllist->__current_node = lllist->__current_node->__ll_last;
    return true;
}

bool lllist_is_at_last(LLList lllist)
{
    return lllist->__current_node == lllist->__last_node;
}

bool lllist_is_at_first(LLList lllist)
{
    return lllist->__current_node == lllist->__first_node;
}

void lllist_go_last(LLList lllist)
{
    lllist->__current_node = lllist->__last_node;
}

void lllist_go_first(LLList lllist)
{
    lllist->__current_node = lllist->__first_node;
}

void lllist_release(LLList *lllist)
{
    // TODO Implement the release function
}

void lllist_concat(LLList list1, LLList list2)
{
    if ((!list1) && (!list2))
        return;
    else if (lllist_is_empty(list1))
    {
        list1->size = list2->size;
        list1->__last_node = list2->__last_node;
        list1->__first_node = list2->__first_node;
        list1->__current_node = list2->__current_node;
    }
    else if (lllist_is_empty(list2))
        return;
    else
    {
        list1->__last_node->__ll_next = list2->__first_node;
        list2->__first_node->__ll_last = list1->__last_node;
        list1->size += list2->size;
        list1->__last_node = list2->__last_node;
    }
}

LLListData lllist_search(LLList lllist, int (*compare_function)(LLListData))
{
    lllist_go_first(lllist);
    if (lllist_is_empty(lllist))
        return NULL;
    do{
        int result = compare_function(lllist_get_current(lllist));
        if (result == 0)
            return lllist_get_current(lllist);
    }while (lllist_step_forward(lllist));

    return NULL;
}

LLList lllist_union(LLList list1, LLList list2, int (*compare_function)(LLListData, LLListData))
{
    LLList result;
    LLList intersect = lllist_intersect(list1, list2, compare_function);    // List of items that are both in list1 and list2

    LLList list1_prime, list2_prime;    // List of the items that are not in list1 or list2

    lllist_init(&result);
    lllist_init(&list1_prime);
    lllist_init(&list2_prime);

    if (!lllist_is_empty(list1))
    {
        lllist_go_first(list1);

        do
        {
            LLListData list1_item = lllist_get_current(list1);

            int search_function(LLListData data)
            {
                return compare_function(list1_item, data);
            }

            if (lllist_search(intersect, &search_function) == NULL)
                lllist_push_front(list1_prime, list1_item);

        }
        while (lllist_step_forward(list1));
    }


    if (!lllist_is_empty(list2))
    {
        lllist_go_first(list2);

        do
        {
            LLListData list2_item = lllist_get_current(list2);
            int search_function(LLListData data)
            {
                return compare_function(list2_item, data);
            }

            if (lllist_search(intersect, &search_function) == NULL)
                lllist_push_front(list2_prime, list2_item);
        }
        while (lllist_step_forward(list2));

    }

    if (!lllist_is_empty(intersect))
    {
        lllist_go_first(intersect);
        do
        {
            lllist_push_front(result, lllist_get_current(intersect));
        }
        while (lllist_step_forward(intersect));
    }

    if (!lllist_is_empty(list1_prime))
    {
        lllist_go_first(list1_prime);
        do
        {
            lllist_push_front(result, lllist_get_current(list1_prime));
        }
        while (lllist_step_forward(list1_prime));
    }

    if (!lllist_is_empty(list2_prime))
    {
        lllist_go_first(list2_prime);
        do
        {
            lllist_push_front(result, lllist_get_current(list2_prime));
        }
        while (lllist_step_forward(list2_prime));
    }

    return result;
}

LLList lllist_sublist(LLListNode start_item, LLListNode end_item)
{
    LLList result_list;
    lllist_init(&result_list);

    return NULL;
}

void lllist_cpy(LLList dest, LLList src)
{
    if (lllist_is_empty(src))
        return;

    lllist_go_first(src);

    do
    {
        lllist_push_front(dest, lllist_get_current(src));
    }
    while (lllist_step_forward(src));
}

LLList lllist_complement(LLList lllist, LLList universe, int (*compare_function)(LLListData, LLListData))
{
    LLList result;
    lllist_init(&result);

    lllist_go_first(universe);

    do
    {
        int cmp(LLListData data)
        {
            return compare_function(data, lllist_get_current(universe));
        }

        LLListData founded = lllist_search(lllist, &cmp);
        if (founded == NULL)        // If not in the list
            lllist_push_front(result, lllist_get_current(universe));
    }
    while(lllist_step_forward(universe));

    return result;
}

LLList lllist_intersect(LLList list1, LLList list2, int (*compare_function)(LLListData, LLListData))
{
    LLList result;
    lllist_init(&result);

    // Check the lists whether non of them is empty
    if (lllist_is_empty(list1) || lllist_is_empty(list2))
        return result;

    lllist_go_first(list1);

    do
    {
        LLListData list1_item = lllist_get_current(list1);
        lllist_go_first(list2);
        do
        {
            LLListData list2_item = lllist_get_current(list2);

            if (compare_function(list1_item, list2_item) == 0)
                lllist_push_front(result, list1_item);
        }
        while (lllist_step_forward(list2));
    }
    while (lllist_step_forward(list1));

    return result;
}

size_t lllist_get_size(LLList lllist)
{
    return lllist->size;
}

LLList lllist_delta(LLList list1, LLList list2, int (*compare_function)(LLListData, LLListData))
{
    LLList result;
    LLList intersect = lllist_intersect(list1, list2, compare_function);    // List of items that are both in list1 and list2

    LLList list1_prime, list2_prime;    // List of the items that are not in list1 or list2

    lllist_init(&result);
    lllist_init(&list1_prime);
    lllist_init(&list2_prime);

    lllist_go_first(list1);

    do
    {
        LLListData list1_item = lllist_get_current(list1);

        int search_function(LLListData data)
        {
            return compare_function(list1_item, data);
        }

        if (lllist_search(intersect, &search_function) == NULL)
            lllist_push_front(list1_prime, list1_item);

    }
    while (lllist_step_forward(list1));

    lllist_go_first(list2);

    do
    {
        LLListData list2_item = lllist_get_current(list2);
        int search_function(LLListData data)
        {
            return compare_function(list2_item, data);
        }

        if (lllist_search(intersect, &search_function) == NULL)
            lllist_push_front(list2_prime, list2_item);
    }
    while (lllist_step_forward(list2));

    if (!lllist_is_empty(list1_prime))
    {
        lllist_go_first(list1_prime);
        do
        {
            lllist_push_front(result, lllist_get_current(list1_prime));
        }
        while (lllist_step_forward(list1_prime));
    }

    if (!lllist_is_empty(list2_prime))
    {
        lllist_go_first(list2_prime);
        do
        {
            lllist_push_front(result, lllist_get_current(list2_prime));
        }
        while (lllist_step_forward(list2_prime));
    }

    return result;
}
