#ifndef FUNCTIONS_H
#define FUNCTIONS_H
//Function signatures
void edit(char *filename);
void insert(int index);
void delete(int index);
void replace(int index);
void save();
void print();
int cursorLine();
int cursorChar();
int garbageCollection();

#endif
