#include <stdio.h>
#include "../file.h"
int main(int argc, char** argv) {
    file_t file = file_open("file-test.c", "r");
    char* contents = file_read(&file);
    printf("%s\n", contents);
    file_rewind(&file);
    stringlist_t lines = file_readlines(&file);
    printf("Lines within file are:\n");
    stringlist_print(lines);
    FILE* f = fopen("output.txt", "w");
    char* output_data = stringlist_tostring(lines);
    fprintf(f, "%s", output_data);
    fclose(f);
    file_destroy(&file);
    return 0;
}