#ifndef _STRINGLIST_H
#define _STRINGLIST_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct stringlist {
    char** strings;
    size_t count;
} stringlist_t;

char* string_dupstr(const char* string);
char* string_stripfront(const char* string, const char* chars);
char* string_stripback(const char* string, const char* chars);
char* string_strip(const char* string, const char* chars);
stringlist_t string_split(const char* string, const char* delim);
void stringlist_init(stringlist_t* stringlist);
bool stringlist_grow(stringlist_t* stringlist, size_t amount);
void stringlist_append(stringlist_t* stringlist, const char* string);
void stringlist_appendptr(stringlist_t* stringlist, char* string);
char* stringlist_tostring(stringlist_t stringlist);
void stringlist_print(stringlist_t stringlist);
void stringlist_printf(stringlist_t stringlist, const char* sep, const char* end);
void stringlist_destroy(stringlist_t* stringlist);
#endif