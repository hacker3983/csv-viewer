#ifndef _STRINGARRAY_LIST_H
#define _STRINGARRAY_LIST_H
#include "stringlist.h"
typedef struct stringarray_list {
    stringlist_t* lists;
    size_t count;
} stringarray_list_t;

void stringarray_list_init(stringarray_list_t* array_list);
void stringarray_list_append(stringarray_list_t* array_list, stringlist_t list);
void stringarray_list_print(stringarray_list_t array_list);
void stringarray_list_destroy(stringarray_list_t* array_list);
#endif