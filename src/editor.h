#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char statement[40]; // En fazla 40 karakterlik satır 
    int next;           // Bir sonraki satırın dizi indeksi 
    int prev;           // Bir önceki satırın dizi indeksi 
};

extern struct node textbuffer[100]; // En fazla 100 satır 
extern int head; 
extern int tail; 
extern int free_idx; 

void edit(char *filename);
int cursorLine();
int cursorChar();
void delete_line(int index);
void insert(int index);
void replace(int index);
void print();
void save();
int garbageCollection();

#endif