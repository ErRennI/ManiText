#ifndef FUNCTIONS_H
#define FUNCTIONS_H
//Function signatures
void edit(char *filename);
void insert(int index);
void delete_line(int index);
void replace(int index);
void print_buffer();
void save();
int cursorLine();
int cursorChar();
int garbageCollection();

#endif