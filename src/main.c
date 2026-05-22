#include "functions.h"
#include "datastruct.h"
#include <stdlib.h>

struct node textbuffer[LINE_ARRAY_SIZE];
int head = -1;
int tail = -1;
int free_idx = 0;

int main() {
  int op_counter = 0;
  int ch;
  
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  printw("Enter the name of the file you want to edit: ");
  char filename[40];
  echo();
  getstr(filename);
  noecho();

  edit(filename);

  int row = 0;
  int col = 0;
  move(row, col);

  while ((ch = getch()) != 'Q' && ch != 'q') {

    switch (ch) {
    case KEY_UP:
      if (row > 0) {
        row--;
      }
      break;

    case KEY_DOWN: {
      int curr = head;
      int line_count = 0;

      while (curr != -1) {
        line_count++;
        curr = textbuffer[curr].next;
      }

      // Eğer liste boşsa aşağı inmeyi engelle, çökme riskini azaltır
      if (line_count > 0 && row < line_count - 1) {
        row++;
      }
      break;
    }
    case KEY_LEFT:
      if (col > 0) {
        col--;
      }
      break;

    case KEY_RIGHT:
      if (col < 39) {
        col++;
      }
      break;

    case 'I':
    case 'i':
      if (free_idx >= 100) {
        garbageCollection();
        op_counter = 0;
      }

      move(row, col);
      int current_line_i = cursorLine();

      // IDE Uyarısı 5 Çözümü: Eğer liste boşsa Miray'ın fonksiyonuna güvenli
      // bir işaret (-1) yolluyoruz Miray'ın insert() içinde 'if (index == -1)'
      // kontrolü yapması şart!
      insert(current_line_i);
      op_counter++;
      break;

    case 'D':
    case 'd':
      if (head != -1) { 
        move(row, col);
        int current_line_d = cursorLine();

        if (current_line_d != -1) { // Boş listeden eleman silinmesini engelle
          delete_line(current_line_d);
          op_counter++;
          
          if (row > 0) {
            row--;
          }
        }
      }
      break;

    case 'R':
    case 'r':
      if (head != -1) {
        move(row, col);
        int current_char_r = cursorChar();
        replace(current_char_r);
      }
      break;

    case 'S':
    case 's':
      save();
      break;

    case 'G':
    case 'g':
      garbageCollection();
      op_counter = 0;
      break;
    }

    if (op_counter >= 10 || free_idx >= LINE_ARRAY_SIZE) {
      garbageCollection();
      op_counter = 0;
      
      int line_count = 0;
      int curr = head;
      while (curr != -1) {
        line_count++;
        curr = textbuffer[curr].next;
      }

      if (row >= line_count) {
        row = (line_count > 0) ? line_count - 1 : 0;
      }
    }

    clear();

    if (head == -1) {
      printw("[Empty file - Press 'I' to add line]");
    } else {
      print();
    }

    move(row, col);
    refresh();
  }

  endwin();
  return 0;
}