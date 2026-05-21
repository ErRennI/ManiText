#include "functions.h"
#include "datastruct.h"
#include <stdio.h>
#include <ncurses.h>

//Fonksiyonlarınızı burdan sonrasına ekleyin insert delete gibi

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
    if (free_index >= 100 || free_index == -1) {
        //otomatik GarbageCollection
        garbageCollection();
        if (free_index == -1) return;
    }

    int new_node_index = free_index; // yeni veriyi koyacağımız indeks

    echo(); 
    mvprintw(LINES - 1, 0, "New line: ");
    getnstr(textbuffer[new_node_index].statement, 39); // max 39 char
    noecho(); // tekrar gizli moda geç 

   
    
    int next_node_index = textbuffer[index].next;
    textbuffer[new_node_index].prev = index;           
    textbuffer[new_node_index].next = next_node_index;  
     // Örnek: A -> B arasına YENI ekliyoruz (A = index, YENI = new_node_index)

    
    textbuffer[index].next = new_node_index;           

    // Eğer ekleme listenin en sonuna yapılmıyorsa, B'nin arkasını yeni elemana bağla
    if (next_node_index != -1) {
        textbuffer[next_node_index].prev = new_node_index; 
    } else {
        tail = new_node_index;    // if it is end then it is tail now
    }
    
    free_index++; //increase
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

  while (fgets(line, sizeof(line), file) && current_idx < 100) {
    line[strcspn(line, "\n")] = 0;

    // Hocanın kuralı: En fazla 40 karakter (39 + null)
    strncpy(textbuffer[current_idx].statement, line, 39);
    textbuffer[current_idx].statement[39] = '\0';

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
    printw("\nEnter filename to save");
    refresh();
    scanw("%49s", filename);

    FILE* fptr = fopen(filename, "w"); //write mode, varsa dosyayı baştan yeniden oluşturur yoksa yeni dosya oluşturur
    if (fptr == NULL) {
        printw("Could not open file");
        refresh();
        return;
    }

    int current = head;

    while (current != -1) {
        fprintf(fptr, "%s\n", textbuffer[current].statement); //dosyanın içine geçiren kısım

        current = textbuffer[current].next;
    }

    fclose(fptr);
    printw("File saved to %s.", filename);
    refresh();
}