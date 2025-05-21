#include <stdio.h>
#include "../stringlist.h"

int main(int argc, char** argv) {
    stringlist_t list = {0};
    for(size_t i=0;i<100;i++) {
        stringlist_append(&list, "Hello,");
        stringlist_append(&list, "World!");
        stringlist_append(&list, "How ");
        stringlist_append(&list, "are ");
        stringlist_append(&list, "you?");
    }
    stringlist_print(list);
    FILE* f = fopen("output.txt", "w");
    char* string = stringlist_tostring(list);
    fprintf(f, "%s\n", string);
    printf("List size is %zu\n", list.count);
    fprintf(f, "List size is %zu\n", list.count);
    free(string);
    fclose(f);
    stringlist_destroy(&list);
    return 0;
}