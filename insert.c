//düzenleme yapılacaktır

void insert(int index) {
    // 1. Hafızada yer kalmış mı kontrol et
    if (free_index >= 100 || free_index == -1) {
        // Yer kalmadıysa önce otomatik Garbage Collection çağrılmalı!
        garbageCollection();
        // GC sonrası hala yer yoksa hata verip çıkılır
        if (free_index == -1) return;
    }

    int new_node_index = free_index; // Yeni veriyi koyacağımız indeks

    // 2. Kullanıcıdan (klavyeden) yeni satırı al (NCURSES modunda girdi alma)
    echo(); // Kullanıcı yazarken ekranda görünsün
    // Ekranın en altına gidip girdiyi alabiliriz
    mvprintw(LINES - 1, 0, "Yeni Satir: ");
    getnstr(textbuffer[new_node_index].statement, 39); // En fazla 39 karakter oku
    noecho(); // Tekrar gizli moda geç

    // 3. Bağları Güncelleme (Linked List Mantığı)
    // Örnek: A -> B arasına YENI ekliyoruz (A = index, YENI = new_node_index)
    
    int next_node_index = textbuffer[index].next; // B'nin indeksi

    // Yeni elemanın bağlarını ayarla
    textbuffer[new_node_index].prev = index;           // YENI'nin arkası -> A
    textbuffer[new_node_index].next = next_node_index;  // YENI'nin önü -> B

    // A'nın önünü yeni elemana bağla
    textbuffer[index].next = new_node_index;           // A'nın önü -> YENI

    // Eğer ekleme listenin en sonuna yapılmıyorsa, B'nin arkasını yeni elemana bağla
    if (next_node_index != -1) {
        textbuffer[next_node_index].prev = new_node_index; // B'nin arkası -> YENI
    } else {
        // Eğer en sona eklendiyse, artık yeni tail bu elemandır
        tail = new_node_index;
    }

    // 4. Global free_index'i bir sonraki boş yere ilerlet
    // (Basitçe 1 artırıyoruz, silinen yerleri GC toplayacak)
    free_index++; 
}
