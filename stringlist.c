#include "stringlist.h"

char* string_dupstr(const char* string) {
    char* new_string = calloc(strlen(string)+1, sizeof(char));
    sprintf(new_string, "%s", string);
    return new_string;
}

stringlist_t string_split(const char* string, const char* delim) {
    stringlist_t list = {0};
    char* new_string = string_dupstr(string);
    char* ptr = strtok(new_string, delim);
    stringlist_append(&list, ptr);
    while((ptr = strtok(NULL, delim))) {
        stringlist_append(&list, ptr);
    }
    return list;
}

void string_shiftchar_toindex(char* string, size_t char_index, size_t target_index) {
    if(!string) {
        return;
    }
    size_t string_len = strlen(string);
    if(char_index > string_len || target_index >= string_len) {
        return;
    }
    for(size_t i=char_index;i<target_index;i++) {
        char current = string[i];
        string[i] = string[i+1];
        string[i+1] = current;
    }
}

char* string_stripfront(const char* string, const char* chars) {
    char* new_string = string_dupstr(string);
    size_t string_size = strlen(string),
           match_count = 0;
    for(size_t i=0;i<string_size;i++) {
        // check if any characters exist at the front of the string
        for(size_t j=0;j<strlen(chars);j++) {
            size_t current_matchcount = 0, dec = (match_count) ? match_count : 0;
            for(size_t k=0;k<string_size-dec;k++) {
                if(new_string[k] != chars[j]) {
                    break;
                }
                // shift any found characters to the end of the string to prevent
                // oversearching
                string_shiftchar_toindex(new_string, k, string_size-dec-1);
                new_string[string_size-dec-1] = '\0';
                current_matchcount++;
            }
            match_count += current_matchcount;
        }
    }
    if(!match_count) {
        return new_string;
    }
    size_t new_size = string_size - match_count + 1;
    new_string = realloc(new_string, new_size);
    new_string[new_size-1] = '\0';
    return new_string;
}

char* string_stripback(const char* string, const char* chars) {
    size_t string_size = strlen(string);
    char* new_string = string_dupstr(string);
    size_t match_count = 0;
    for(size_t i=0;i<string_size;i++) {
        for(size_t j=0;j<strlen(chars);j++) {
            size_t current_matchcount = 0, dec = (match_count > 0) ? match_count+1 : 1;
            for(size_t k=string_size-dec;k>=0;) {
                if(new_string[k] != chars[j]) {
                    break;
                }
                string_shiftchar_toindex(new_string, k, string_size-dec);
                new_string[string_size-dec] = '\0';
                current_matchcount++;
                if(!k) {
                    break;
                }
                k--;
            }
            match_count += current_matchcount;
        }
    }
    size_t new_size = string_size - match_count + 1;
    new_string = realloc(new_string, new_size);
    new_string[new_size-1] = '\0';
    return new_string;
}

char* string_strip(const char* string, const char* chars) {
    char *stripped_front = string_stripfront(string, chars),
         *stripped_string = string_stripback(stripped_front, chars);
    free(stripped_front);
    return stripped_string;
}

void stringlist_init(stringlist_t* stringlist) {
    stringlist->strings = NULL;
    stringlist->count = 0;
}

bool stringlist_grow(stringlist_t* stringlist, size_t amount) {
    char** new_strings = stringlist->strings;
    size_t new_count = stringlist->count + amount;
    if(!amount) {
        return false;
    }
    new_strings = realloc(stringlist->strings,  new_count * sizeof(char*));
    if(!new_strings) {
        return false;
    }
    stringlist->strings = new_strings;
    stringlist->count = new_count;
    return true;
}

void stringlist_append(stringlist_t* stringlist, const char* string) {
    char* new_string = string_dupstr(string);
    if(!new_string) {
        return;
    }
    if(!stringlist_grow(stringlist, 1)) {
        return;
    }
    stringlist->strings[stringlist->count-1] = new_string;
}

void stringlist_appendptr(stringlist_t* stringlist, char* string) {
    if(!stringlist_grow(stringlist, 1)) {
        return;
    }
    stringlist->strings[stringlist->count-1] = string;
}

char* stringlist_tostring(stringlist_t stringlist) {
    char* string_buff = calloc(3, sizeof(char));
    size_t string_bufflen = 1;
    strcpy(string_buff, "[");
    for(size_t i=0;i<stringlist.count;i++) {
        char* string = stringlist.strings[i];
        size_t extra_count = (i != stringlist.count) ? 4 : 2;
        size_t string_len = strlen(string), new_buffsize = string_bufflen + string_len
               + extra_count + 2;
        char* new_ptr = realloc(string_buff, new_buffsize);
        if(!new_ptr) {
            break;
        }
        sprintf(new_ptr + string_bufflen, "\"%s\"", string);
        if(i != stringlist.count-1) {
            new_ptr[new_buffsize-4] = ',';
            new_ptr[new_buffsize-3] = ' ';
        }
        new_ptr[new_buffsize-1] = '\0';
        string_buff = new_ptr;
        string_bufflen += string_len + extra_count;
    }
    strcat(string_buff, "]");
    return string_buff;
}

void stringlist_print(stringlist_t stringlist) {
    stringlist_printf(stringlist, ", ", "\n");
}

void stringlist_printf(stringlist_t stringlist, const char* sep, const char* end) {
        printf("[");
    for(size_t i=0;i<stringlist.count;i++) {
        printf("\"");
        for(size_t j=0;j<strlen(stringlist.strings[i]);j++) {
            if(stringlist.strings[i][j] == '\n') {
                printf("\\n");
                continue;
            } else if(stringlist.strings[i][j] == '\r') {
                printf("\\r");
                continue;
            }
            putchar(stringlist.strings[i][j]);
        }
        printf("\"");
        if(i != stringlist.count-1) {
            printf("%s", sep);
        }
    }
    printf("]%s", end);
}

void stringlist_destroy(stringlist_t* stringlist) {
    for(size_t i=0;i<stringlist->count;i++) {
        free(stringlist->strings[i]);
    }
    stringlist_init(stringlist);
}