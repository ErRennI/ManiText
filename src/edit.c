#include "editor.h"

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
    // Eğer dosya tamamen boş çıktıysa listeyi boşalt
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