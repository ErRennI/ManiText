//düzenleme yapılacaktır
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
