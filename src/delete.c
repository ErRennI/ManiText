#include "editor.h"

// 1. Görev: Ekrandaki imleç koordinatından dizideki satır indeksini bulur
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