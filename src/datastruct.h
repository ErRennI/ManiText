#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_ARRAY_SIZE 40
#define LINE_ARRAY_SIZE 100

struct node {
    char statement[CHAR_ARRAY_SIZE];
    int next;
    int prev;
};

extern struct node textbuffer[LINE_ARRAY_SIZE];
extern int head;
extern int tail;
extern int free_idx;
extern char status_message[100];

#endif