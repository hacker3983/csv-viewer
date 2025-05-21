#ifndef _FILE_H
#define _FILE_H
#include <stdio.h>
#include "stringlist.h"

typedef struct file {
    FILE* stream;
    char* content;
    size_t content_size;
    stringlist_t lines;
} file_t;

file_t file_open(const char* filename, const char* mode);
char* file_read(file_t* file);
char* file_readline(file_t* file);
void file_rewind(file_t* file);
void file_seek(file_t* file, long offset, int whence);
stringlist_t file_readlines(file_t* file);
void file_close(file_t* file);
void file_destroy(file_t* file);
#endif