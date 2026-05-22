#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_ARRAY_SIZE 40
#define LINE_ARRAY_SIZE 100

struct node {
    char statement[CHAR_ARRAY_SIZE]; // En fazla 40 karakterlik satır 
    int next;           // Bir sonraki satırın dizi indeksi 
    int prev;           // Bir önceki satırın dizi indeksi 
};

extern struct node textbuffer[LINE_ARRAY_SIZE]; // En fazla 100 satır 
extern int head;
extern int tail;
extern int free_idx;

#endif