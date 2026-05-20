//düzenleme yapılacaktır
//****************************************************

void print() {
    int current = head; // Okumaya listenin başından (head) başlıyoruz
    int screen_line = 0; // NCURSES ekranında hangi satıra yazacağımızı tutar

    // Liste boşsa veya head geçersizse ekrana uyarı verip çıkabiliriz
    if (current == -1) {
        mvprintw(0, 0, "Buffer bos. E ile dosya acin.");
        return;
    }

    // Bağlı listeyi sonuna kadar takip et (-1 listenin bittiğini gösterir)
    while (current != -1) {
        // textbuffer[current].statement içeriğini ekrandaki screen_line satırına bas
        mvprintw(screen_line, 0, "%s", textbuffer[current].statement);
        
        screen_line++;
        current = textbuffer[current].next; // Bir sonraki düğümün indeksine zıpla
    }
}
