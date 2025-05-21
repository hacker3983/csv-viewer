#include "stringarray_list.h"

void stringarray_list_init(stringarray_list_t* array_list) {
    array_list->lists = NULL;
    array_list->count = 0;
}

void stringarray_list_append(stringarray_list_t* array_list, stringlist_t list) {
    size_t new_count = array_list->count + 1;
    stringlist_t* new_ptr = realloc(array_list->lists, new_count * sizeof(stringlist_t));
    if(!new_ptr) {
        return;
    }
    new_ptr[array_list->count] = list;
    array_list->lists = new_ptr;
    array_list->count = new_count;
}

void stringarray_list_print(stringarray_list_t array_list) {
    printf("[\n");
    for(size_t i=0;i<array_list.count;i++) {
        printf("\t");
        if(i != array_list.count-1) {
            stringlist_printf(array_list.lists[i], ", ", ",\n");
            continue;
        }
        stringlist_printf(array_list.lists[i], ", ", "\n");
    }
    printf("]\n");
}

void stringarray_list_destroy(stringarray_list_t* array_list) {
    for(size_t i=0;i<array_list->count;i++) {
        stringlist_destroy(&array_list->lists[i]);
    }
    free(array_list->lists);
    stringarray_list_init(array_list);
}