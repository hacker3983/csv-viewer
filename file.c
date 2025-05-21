#include "file.h"

void file_init(file_t* file) {
    file->stream = NULL;
    stringlist_init(&file->lines);
    file->content = NULL;
    file->content_size = 0;
}

file_t file_open(const char* filename, const char* mode) {
    file_t file = {0};
    file.stream = fopen(filename, mode);
    return file;
}
char* file_read(file_t* file) {
    fseek(file->stream, 0, SEEK_END);
    size_t content_size = ftell(file->stream);
    rewind(file->stream);
    char* content = calloc(content_size+1, sizeof(char));
    fread(content, sizeof(char), content_size, file->stream);
    file->content = content;
    file->content_size = content_size;
    return content;
}

char* file_readline(file_t* file) {
    char* line = NULL, c = '\0';
    size_t line_size = 0;
    while((c = fgetc(file->stream)) != EOF) {
        char* new_ptr = realloc(line, (line_size+2) * sizeof(char));
        if(!new_ptr) {
            break;
        }
        new_ptr[line_size++] = c;
        new_ptr[line_size] = '\0';
        line = new_ptr;
        if(c == '\n') {
            break;
        }
    }
    return line;
}

stringlist_t file_readlines(file_t* file) {
    stringlist_t lines = {0};
    char* line = NULL;
    while((line = file_readline(file))) {
        stringlist_appendptr(&lines, line);
    }
    file->lines = file->lines;
    return lines;
}

void file_rewind(file_t* file) {
    rewind(file->stream);
}

void file_seek(file_t* file, long offset, int whence) {
    fseek(file->stream, offset, whence);
}

void file_close(file_t* file) {
    fclose(file->stream);
}

void file_destroy(file_t* file) {
    file_close(file);
    free(file->content);
    stringlist_destroy(&file->lines);
    file_init(file);
}