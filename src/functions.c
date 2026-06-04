#include "functions.h"
#include "datastruct.h"
#include <ncurses.h>
#include <stdio.h>

int cursorChar() {
  int x, y;

  getyx(stdscr, y, x);
  (void)y;

  if (x > CHAR_ARRAY_SIZE - 1) {
    return CHAR_ARRAY_SIZE - 1;
  }

  return x;
}

void replace(int index) {
  int line_index = cursorLine();
  if ((line_index == -1) || (index < 0) || (index >= CHAR_ARRAY_SIZE)) {
    return;
  }

  move(LINES - 1, 0);
  clrtoeol();
  mvprintw(LINES - 1, 0, "Replace character, new character: ");

  echo();
  int new_char = getch();
  noecho();

  textbuffer[line_index].statement[index] = (char)new_char;

  move(LINES - 1, 0);
  clrtoeol();
}

void print() {
  int current = head;
  int screen_line = 0;

  if (current == -1) {
    mvprintw(0, 0, "Buffer is empty. Please press E to open file.");
    return;
  }

  while (current != -1) {
    mvprintw(screen_line, 0, "%s", textbuffer[current].statement);
    screen_line++;
    current = textbuffer[current].next;
  }
}

void insert(int index) {
  if (free_idx >= LINE_ARRAY_SIZE || free_idx == -1) {
    garbageCollection();
    if (free_idx == -1 || free_idx >= LINE_ARRAY_SIZE)
      return;

    index = cursorLine();
  }

  int new_node_index = free_idx;

  move(LINES - 1, 0);
  clrtoeol();
  mvprintw(LINES - 1, 0, "New line: ");

  echo();
  getnstr(textbuffer[new_node_index].statement, CHAR_ARRAY_SIZE - 1);
  noecho();

  if (head == -1) {
    head = new_node_index;
    tail = new_node_index;
    textbuffer[new_node_index].next = -1;
    textbuffer[new_node_index].prev = -1;
  }

  else if (index == -1) {
    textbuffer[new_node_index].prev = -1;
    textbuffer[new_node_index].next = head;
    textbuffer[head].prev = new_node_index;
    head = new_node_index;
  }

  else {
    int next_node_index = textbuffer[index].next;

    textbuffer[new_node_index].prev = index;
    textbuffer[new_node_index].next = next_node_index;
    textbuffer[index].next = new_node_index;

    if (next_node_index != -1) {
      textbuffer[next_node_index].prev = new_node_index;
    } else {
      tail = new_node_index;
    }
  }

  free_idx++;

  move(LINES - 1, 0);
  clrtoeol();

  print();
  refresh();
}

int cursorLine() {
  int y, x;
  getyx(stdscr, y, x);

  int cur = head;
  int count = 0;

  while (cur != -1 && count < y) {
    cur = textbuffer[cur].next;
    count++;
  }

  return cur;
}

void delete_line(int index) {
  if (index == -1)
    return;

  int p_prev = textbuffer[index].prev;
  int p_next = textbuffer[index].next;

  if (index == head) {
    head = p_next;
  } else {
    textbuffer[p_prev].next = p_next;
  }

  if (index == tail) {
    tail = p_prev;
  } else {
    textbuffer[p_next].prev = p_prev;
  }

  print();
  refresh();
}

void edit(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    head = -1;
    tail = -1;
    free_idx = 0;
    return;
  }

  char line[256];
  int current_idx = 0;

  head = 0;

  while (fgets(line, sizeof(line), file) &&
         current_idx < (LINE_ARRAY_SIZE - 1)) {
    line[strcspn(line, "\n")] = 0;

    strncpy(textbuffer[current_idx].statement, line, CHAR_ARRAY_SIZE - 1);
    textbuffer[current_idx].statement[CHAR_ARRAY_SIZE - 1] = '\0';

    textbuffer[current_idx].prev = current_idx - 1;
    textbuffer[current_idx].next = current_idx + 1;

    current_idx++;
  }

  fclose(file);

  if (current_idx == 0) {
    head = -1;
    tail = -1;
    free_idx = 0;
  } else {
    tail = current_idx - 1;
    textbuffer[head].prev = -1;
    textbuffer[tail].next = -1;
    free_idx = current_idx;
  }
}

void save() {
  char filename[50];
  move(LINES - 1, 0);
  clrtoeol();
  mvprintw(LINES - 1, 0, "Enter filename to save: ");
  echo();
  scanw("%49s", filename);
  noecho();

  FILE *fptr = fopen(filename, "w");
  if (fptr == NULL) {
    snprintf(status_message, sizeof(status_message),
             "Error: Could not open file!");
    return;
  }

  int current = head;

  while (current != -1) {
    fprintf(fptr, "%s\n", textbuffer[current].statement);

    current = textbuffer[current].next;
  }

  fclose(fptr);
  snprintf(status_message, sizeof(status_message), "File saved to %s.",
           filename);
}
int garbageCollection() {

  if (head == -1) {
    free_idx = 0;
    return 0;
  }

  struct node temp_buffer[LINE_ARRAY_SIZE];
  int current = head;
  int new_idx = 0;

  while (current != -1 && new_idx < LINE_ARRAY_SIZE) {
    strcpy(temp_buffer[new_idx].statement, textbuffer[current].statement);

    current = textbuffer[current].next;
    new_idx++;
  }

  for (int i = 0; i < new_idx; i++) {
    temp_buffer[i].prev = i - 1;
    temp_buffer[i].next = i + 1;
  }

  if (new_idx > 0) {
    temp_buffer[new_idx - 1].next = -1;
  }

  for (int i = 0; i < new_idx; i++) {
    textbuffer[i] = temp_buffer[i];
  }

  head = 0;
  tail = new_idx - 1;
  free_idx = new_idx;

  return 1;
}
