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
