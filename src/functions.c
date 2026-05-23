#include "functions.h"
#include "datastruct.h"
#include <stdio.h>
#include <ncurses.h>

int cursorChar(){
    int x,y;

    getyx(stdscr, y, x);

    if(x > CHAR_ARRAY_SIZE-1){
        return CHAR_ARRAY_SIZE-1;
    }

    return x;
}

void replace(int index){
    int line_index = cursorLine();
    if((line_index == -1) || (index < 0) || (index >= CHAR_ARRAY_SIZE)){
        return;
    }

    mvprintw(LINES - 1, 0, "Replace character, new character: ");
    echo();
    int new_char = getch();
    noecho();

    textbuffer[line_index].statement[index] = (char)new_char;

    mvprintw(LINES-1, 0, "                                  ");
}

// 20 Mayıs 2026
//********************
void print() {
    int current = head; 
    int screen_line = 0; 

    if (current == -1) {
        // check if the text buffer is empty
        mvprintw(0, 0, "Buffer is empty. Please press E to open file.");
        return;
    }

    while (current != -1) {
        // Loop through the buffer following the links until the end
        mvprintw(screen_line, 0, "%s", textbuffer[current].statement);
        screen_line++;
        current = textbuffer[current].next; 
    }
}

// 20 Mayıs 2026
//********************
//garbageCollection()
//new_node_index
//free_index
//next_node_index
//noecho araştır
void insert(int index) {
    if (free_idx >= LINE_ARRAY_SIZE || free_idx == -1) {
        //otomatik GarbageCollection
        garbageCollection();
        if (free_idx == -1 || free_idx >= LINE_ARRAY_SIZE) return;

        index = cursorLine();
    }

    int new_node_index = free_idx; // yeni veriyi koyacağımız indeks

    echo(); 
    mvprintw(LINES - 1, 0, "New line: ");
    clrtoeol();
    getnstr(textbuffer[new_node_index].statement, CHAR_ARRAY_SIZE - 1); // max 39 char
    noecho(); // tekrar gizli moda geç 

    if(head == -1){
        head = new_node_index;
        tail = new_node_index;
        textbuffer[new_node_index].next = -1;
        textbuffer[new_node_index].prev = -1;
    }

    else if(index == -1){
        textbuffer[new_node_index].prev = -1;
        textbuffer[new_node_index].next = head;
        textbuffer[head].prev = new_node_index;
        head = new_node_index;
    }

    else{
        int next_node_index = textbuffer[index].next;
        
        textbuffer[new_node_index].prev = index;
        textbuffer[new_node_index].next = next_node_index;
        textbuffer[index].next = new_node_index;

        if(next_node_index != -1){
            textbuffer[next_node_index].prev = new_node_index;
        }
        else{
            tail = new_node_index;
        }
    }
    
    free_idx++;
    mvprintw(LINES - 1, 0, "                                             ");
}

int cursorLine() {
    int y, x;
    getyx(stdscr, y, x); // Ncurses imleç koordinatlarını aldık

    int curr = head; 
    int count = 0;

    // ekrandaki satır sayısına (y) ulaşana kadar listeyi takip et
    while (curr != -1 && count < y) {
        curr = textbuffer[curr].next;
        count++;
    }

    return curr; // bulduğu dizi indexi
}

void delete_line(int index) {
    if (index == -1) return; 

    int p_prev = textbuffer[index].prev;
    int p_next = textbuffer[index].next;

    // silinen eleman head ise
    if (index == head) { 
        head = p_next; // yeni head sonraki eleman
    } else {
        // başta değilse, arkasındaki elemanın next bağını bir ileri atlatırız
        textbuffer[p_prev].next = p_next;
    }

    // listenin sonundaysa tail
    if (index == tail) { 
        tail = p_prev; // yeni tail öncekli eleman olur
    } else {
        // son değilse previ geriye bağlarız
        textbuffer[p_next].prev = p_prev;
    }
    
}

void edit(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    head = -1;
    tail = -1;
    free_idx = 0;
    return;
  }

  // IDE Uyarısı 6 Çözümü: Satır tamponunu büyük tutuyoruz ki bölünme olmasın
  char line[256];
  int current_idx = 0;

  head = 0;

  while (fgets(line, sizeof(line), file) && current_idx < (LINE_ARRAY_SIZE - 1) ) {
    line[strcspn(line, "\n")] = 0;

    // Hocanın kuralı: En fazla 40 karakter (39 + null)
    strncpy(textbuffer[current_idx].statement, line, CHAR_ARRAY_SIZE - 1);
    textbuffer[current_idx].statement[CHAR_ARRAY_SIZE - 1] = '\0';

    textbuffer[current_idx].prev = current_idx - 1;
    textbuffer[current_idx].next = current_idx + 1;

    current_idx++;
  }

  fclose(file);

  if (current_idx == 0) {
    // eğer dosya tamamen boş çıktıysa listeyi boşalt
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

//21 Mayıs 2026 Samet
void save() {
    char filename[50];
    //kaydedilecek dosya adı 
    mvprintw(LINES - 1, 0,"\nEnter filename to save");
    echo();
    scanw("%49s", filename);
    noecho();

    FILE* fptr = fopen(filename, "w"); //write mode, varsa dosyayı baştan yeniden oluşturur yoksa yeni dosya oluşturur
    if (fptr == NULL) {
        mvprintw(LINES - 1, 0, "Error: Could not open file!                 ");
        return;
    }

    int current = head;

    while (current != -1) {
        fprintf(fptr, "%s\n", textbuffer[current].statement); //dosyanın içine geçiren kısım

        current = textbuffer[current].next;
    }

    fclose(fptr);
    mvprintw(LINES - 1, 0, "File saved to %s.                           ", filename);
}
//22 Mayıs Samet
int garbageCollection() {

    if (head == -1) {
        free_idx = 0;
        return 0;
    }

    struct node temp_buffer[LINE_ARRAY_SIZE]; //temizler konuyo
    int current = head;
    int new_idx = 0;

    while (current != -1 && new_idx < LINE_ARRAY_SIZE) {
        strcpy(temp_buffer[new_idx].statement, textbuffer[current].statement);//erişebildiği nextleri kopyalıyor, erişemedikleri kalıyor.

        current = textbuffer[current].next;
        new_idx++;

    }

    for(int i = 0; i < new_idx; i++){
        temp_buffer[i].prev = i - 1;
        temp_buffer[i].next = i + 1;
    }

    //Son elemandan sonrasını kapatır
    if (new_idx > 0) {
        temp_buffer[new_idx - 1].next = -1;
    }

    for (int i = 0; i < new_idx; i++) { //temp temiz olan, temiz olanı orijinale koyuyor, collection tamamlanıyor.
        textbuffer[i] = temp_buffer[i];
    }

    head = 0;
    tail = new_idx - 1;
    free_idx = new_idx;

    return 1;
}
