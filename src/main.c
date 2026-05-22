#include "functions.h"
#include "datastruct.h"

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
    int current_line = cursorLine();
    int current_char = cursorChar();

    switch (ch) {
    case KEY_UP:
      if (row > 0) {
        row--;
        move(row, col);
      }
      break;

    case KEY_DOWN:
      // Eğer liste boşsa aşağı inmeyi engelle, çökme riskini azaltır
      if (head != -1 && row < 29 && row < free_idx - 1) {
        row++;
        move(row, col);
      }
      break;

    case KEY_LEFT:
      if (col > 0) {
        col--;
        move(row, col);
      }
      break;

    case KEY_RIGHT:
      if (col < 39) {
        col++;
        move(row, col);
      }
      break;

    case 'I':
    case 'i':
      if (free_idx >= 100) {
        garbageCollection();
        op_counter = 0;
      }

      // IDE Uyarısı 5 Çözümü: Eğer liste boşsa Miray'ın fonksiyonuna güvenli
      // bir işaret (-1) yolluyoruz Miray'ın insert() içinde 'if (index == -1)'
      // kontrolü yapması şart!
      insert(current_line);
      op_counter++;
      break;

    case 'D':
    case 'd':
      if (head != -1 &&
          current_line != -1) { // Boş listeden eleman silinmesini engelle
        delete_line(current_line);
        op_counter++;
      }
      break;

    case 'R':
    case 'r':
      if (head != -1) {
        replace(current_char);
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

    if (op_counter >= 10) {
      garbageCollection();
      op_counter = 0;
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