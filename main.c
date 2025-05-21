#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "file.h"
#include "stringarray_list.h"
#define COLOR_TOPARAM(color) color.r, color.g, color.b, color.a

int find_maxcolumn_width(TTF_Font* font, stringarray_list_t stringarray_list,
    size_t column_index) {
    int max_width = 0, column_w = 0;
    for(size_t i=0;i<stringarray_list.count;i++) {
        char* column_name = stringarray_list.lists[i].strings[column_index];
        TTF_SizeText(font, column_name, &column_w, NULL);
        if(column_w > max_width) {
            max_width = column_w;
        }
    }
    return max_width;
}

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    bool quit = false;
    SDL_Event e;
    SDL_Color window_color = {0x12, 0x12, 0x12, 0xff};
    int width = 900, height = 600;
    SDL_Window* window = SDL_CreateWindow("CSV Viewer", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("Roboto-Black.ttf", 20);
    file_t file = file_open("tests/organization.csv", "r");
    stringlist_t lines = file_readlines(&file);
    stringarray_list_t stringarray_list = {0};
    file_close(&file);
    for(size_t i=0;i<lines.count;i++) {
        stringlist_t list = string_split(lines.strings[i], ",");
        for(size_t j=0;j<list.count;j++) {
            char* stripped_string = string_strip(list.strings[j], " \"\r\n");
            free(list.strings[j]);
            list.strings[j] = stripped_string;
        }
        stringarray_list_append(&stringarray_list, list);
        //stringlist_print(list);
    }
    stringarray_list_print(stringarray_list);
    stringlist_t header = stringarray_list.lists[0];
    int column_startx = 0, column_starty = 10; 
    int totalcolumns_width = 0;
    SDL_Rect column_canvas = {
        .x = column_startx, .y = column_starty,
        .w = 0, .h = 0
    },
    column_namecanvas = {0},
    **table_canvases = calloc(stringarray_list.count, sizeof(SDL_Rect*));
    SDL_Color column_color = {0xff, 0xff, 0xff, 0xff};
    for(size_t j=0;j<header.count;j++) {
        char* column_name = header.strings[j];
        TTF_SizeText(font, column_name, &column_namecanvas.w, &column_namecanvas.h);
        int max_columnw = find_maxcolumn_width(font, stringarray_list, j);
        column_canvas.w = max_columnw + 20,
        column_canvas.h = column_namecanvas.h + 20;
        totalcolumns_width += column_canvas.w;
    }
    column_startx = (width - totalcolumns_width) / 2;
    column_canvas.x = column_startx;
    for(size_t i=0;i<stringarray_list.count;i++) {
        stringlist_t list = stringarray_list.lists[i];
        table_canvases[i] = calloc(list.count, sizeof(SDL_Rect));
        for(size_t j=0;j<list.count;j++) {
            char* column_name = list.strings[j];
            TTF_SizeText(font, column_name, &column_namecanvas.w, &column_namecanvas.h);
            int max_columnw = find_maxcolumn_width(font, stringarray_list, j);
            column_canvas.w = max_columnw + 20,
            column_canvas.h = column_namecanvas.h + 20;
            table_canvases[i][j] = column_canvas;
            column_canvas.x += column_canvas.w;
        }
        column_startx = (width - totalcolumns_width) / 2;
        column_canvas.x = column_startx;
        column_canvas.y += column_canvas.h;
    }
    while(!quit) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = true;
                break;
            }
        }
        SDL_GetWindowSize(window, &width, &height);
        SDL_SetRenderDrawColor(renderer, COLOR_TOPARAM(window_color));
        SDL_RenderClear(renderer);
        totalcolumns_width = 0;
        for(size_t i=0;i<header.count;i++) {
            char* column_name = header.strings[i];
            TTF_SizeText(font, column_name, &column_namecanvas.w, &column_namecanvas.h);
            int max_columnw = find_maxcolumn_width(font, stringarray_list, i);
            column_canvas.w = max_columnw + 20,
            column_canvas.h = column_namecanvas.h + 20;
            totalcolumns_width += column_canvas.w;
        }
        column_startx = (width - totalcolumns_width) / 2;
        column_canvas.x = column_startx;
        column_canvas.y = column_starty;
        for(size_t i=0;i<stringarray_list.count;i++) {
            stringlist_t list = stringarray_list.lists[i];
            for(size_t j=0;j<list.count;j++) {
                char* column_name = list.strings[j];
                TTF_SizeText(font, column_name, &column_namecanvas.w, &column_namecanvas.h);
                int max_columnw = find_maxcolumn_width(font, stringarray_list, j);
                column_canvas.w = max_columnw + 20,
                column_canvas.h = column_namecanvas.h + 20;
                table_canvases[i][j] = column_canvas;
                column_canvas.x += column_canvas.w;
            }
            column_startx = (width - totalcolumns_width) / 2;
            column_canvas.x = column_startx;
            column_canvas.y += column_canvas.h;
        }
        for(size_t i=0;i<stringarray_list.count;i++) {
            stringlist_t list = stringarray_list.lists[i];
            for(size_t j=0;j<list.count;j++) {
                char* column_name = list.strings[j];
                SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
                SDL_RenderDrawRect(renderer, &table_canvases[i][j]);
                TTF_SizeText(font, column_name, &column_namecanvas.w, &column_namecanvas.h);
                column_namecanvas.x = table_canvases[i][j].x + (table_canvases[i][j].w -
                    column_namecanvas.w) / 2;
                column_namecanvas.y = table_canvases[i][j].y + (table_canvases[i][j].h -
                    column_namecanvas.h) / 2;
                SDL_Surface* column_surface = TTF_RenderText_Blended(font, column_name,
                    column_color);
                SDL_Texture* column_texture = SDL_CreateTextureFromSurface(renderer, column_surface);
                SDL_FreeSurface(column_surface);
                SDL_RenderCopy(renderer, column_texture, NULL, &column_namecanvas);
                SDL_DestroyTexture(column_texture);
            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    for(size_t i=0;i<stringarray_list.count;i++) {
        free(table_canvases[i]);
    }
    free(table_canvases);
    stringarray_list_destroy(&stringarray_list);
    file_destroy(&file);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}