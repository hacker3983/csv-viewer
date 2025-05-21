#!/usr/bin/bash
echo "Building CSV Viewer..."
gcc main.c file.c stringlist.c stringarray_list.c --lSDL2 -lSDL2_ttf -o csv_viewer
echo Running CSV Viewer...
./csv_viewer