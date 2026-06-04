#ifndef FUNCTIONS_H
#define FUNCTIONS_H
void edit(char *filename);
void insert(int index);
void delete_line(int index);
void replace(int index);
void save();
void print();
int cursorLine();
int cursorChar();
int garbageCollection();

#endif
