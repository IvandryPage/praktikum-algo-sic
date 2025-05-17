#include <iomanip>
#include <iostream>

struct Roti {
  std::string jenis;
  int harga;
};

struct Pesanan {
  static int id_count;
  int nomor_antrean;
  std::string nama;
  Roti roti;
  int jumlah;
  int total_harga{0};

  Pesanan() : nomor_antrean(id_count++) {}

  Pesanan(std::string nama, Roti roti, int jumlah)
      : nama(nama),
        roti(roti),
        jumlah(jumlah),
        total_harga(roti.harga * jumlah) {
    nomor_antrean = id_count++;
  }
};

struct Node {
  Pesanan info;
  Node* next;

  Node(Pesanan info) : info(info), next{nullptr} {}
};

struct Stack {
  Node *bottom, *top;

  void createStack() { bottom = top = nullptr; }

  bool emptyStack() { return bottom == nullptr; }

  void push(Pesanan info_baru) {
    Node* new_node = new Node(info_baru);

    if (emptyStack()) {
      bottom = top = new_node;
      return;
    }

    top->next = new_node;
    top = new_node;
  }

  void pop() {
    if (emptyStack()) {
      std::cout << "Stack masih kosong!\n";
      return;
    }

    if (bottom == top) {
      delete top;
      bottom = top = nullptr;
      return;
    }

    Node* target_node = bottom;
    while (target_node->next != top) {
      target_node = target_node->next;
    }

    delete top;
    top = target_node;
    top->next = nullptr;
  }
};

struct Queue {
  Node* front;
  Node* back;

  void create() { front = back = nullptr; }

  bool isEmpty() { return front == nullptr; }

  void enqueue(Pesanan info_baru) {
    Node* new_node = new Node(info_baru);

    if (isEmpty()) {
      front = back = new_node;
      return;
    }

    back->next = new_node;
    back = new_node;
  }

  void dequeue() {
    if (isEmpty()) {
      std::cout << "Antrian masih kosong!\n";
      return;
    }

    Node* target = front;
    front = front->next;
    delete target;
  }
};

Queue antrean;
Stack riwayat;
int Pesanan::id_count = 1;

template <typename T>
void printTable(T* start_point) {
  std::cout << std::left << std::setw(5) << "No" << std::setw(20) << "Nama"
            << std::setw(15) << "Jenis Roti" << std::setw(8) << "Jumlah"
            << std::setw(12) << "Total(Rp)"
            << "\n";
  std::cout << std::setfill('-') << std::setw(60) << "-" << std::setfill(' ')
            << "\n";

  Node* x = start_point;
  while (x != nullptr) {
    Pesanan p = x->info;
    std::cout << std::left << std::setw(5) << p.nomor_antrean << std::setw(20)
              << p.nama << std::setw(15) << p.roti.jenis << std::setw(8)
              << p.jumlah << std::setw(12) << p.total_harga << "\n";
    x = x->next;
  }
}

void ambilAntrean(Roti* data_roti, int size) {
  std::cin.ignore();
  std::cout << "\n=== Ambil Antrean ===\n";

  std::string nama;
  std::cout << "Nama Pelanggan: ";
  getline(std::cin, nama);

  Roti roti;
  std::cout << "Jenis Roti:\n";
  for (int i = 0; i < size; ++i) {
    std::cout << i + 1 << ") " << (data_roti + i)->jenis << " - Rp"
              << (data_roti + i)->harga << "\n";
  }

  int roti_choice;
  std::cout << "Pilih: ";
  std::cin >> roti_choice;

  roti = *(data_roti + roti_choice - 1);

  int jumlah;
  std::cout << "Jumlah: ";
  std::cin >> jumlah;

  Pesanan pesanan_baru(nama, roti, jumlah);
  antrean.enqueue(pesanan_baru);

  std::cout << "Pesanan berhasil ditambahkan dengan nomor antrean: "
            << pesanan_baru.nomor_antrean << std::endl;
}

void layaniPelanggan() {
  if (antrean.isEmpty()) {
    std::cout << "Tidak ada pesanan dalam antrean.\n";
    return;
  }

  riwayat.push(antrean.front->info);
  std::cout << "\nMelayani pelanggan: " << antrean.front->info.nama << "\n";
  antrean.dequeue();
}

void tampilkanPesanan() {
  if (antrean.isEmpty()) {
    std::cout << "Belum ada pesanan.\n";
    return;
  }

  std::cout << "\n=== Daftar Pesanan ===\n";
  printTable<Node>(antrean.front);
}

void batalkanPesanan() {
  if (antrean.isEmpty()) {
    std::cout << "Tidak ada pesanan dalam antrean.\n";
    return;
  }

  if (antrean.front == antrean.back) {
    std::cout << "Pesanan dengan nomor antrean: "
              << antrean.front->info.nomor_antrean << " berhasil dibatalkan!";
    antrean.front = antrean.back = nullptr;
    return;
  }

  Node* bantu = antrean.front;

  while (bantu->next != antrean.back) {
    bantu = bantu->next;
  }

  antrean.back = bantu;
  std::cout << "Pesanan dengan nomor antrean: "
            << bantu->next->info.nomor_antrean << " berhasil dibatalkan!";
  delete bantu->next;
  antrean.back->next = nullptr;
}

void riwayatPesanan() {
  if (riwayat.emptyStack()) {
    std::cout << "Belum ada riwayat.\n";
    return;
  }

  std::cout << "\n=== Riwayat Antrian ===\n";
  printTable<Node>(riwayat.bottom);
}

void loadRiwayat() { Pesanan p; }

void saveRiwayat() {}

int main() {
  antrean.create();
  riwayat.createStack();

  Roti data_roti[]{Roti("Tawar", 8000),  Roti("Coklat", 10000),
                   Roti("Keju", 11000),  Roti("Pisang", 12000),
                   Roti("Sobek", 15000), Roti("Gandum", 10000)};

  int pilihan;
  do {
    system("clear");
    std::cout << "\n=== Menu Toko Roti ===\n";
    std::cout << "1. Ambil Antrean\n";
    std::cout << "2. Layani Pelanggan\n";
    std::cout << "3. Tampilkan Daftar Pesanan\n";
    std::cout << "4. Batalkan Pesanan  Terakhir\n";
    std::cout << "5. Tampilkan Riwayat Pesanan\n";
    std::cout << "6. Keluar\n";
    std::cout << "Pilih: ";
    std::cin >> pilihan;

    switch (pilihan) {
      case 1:
        ambilAntrean(data_roti, std::size(data_roti));
        break;
      case 2:
        layaniPelanggan();
        break;
      case 3:
        tampilkanPesanan();
        break;
      case 4:
        batalkanPesanan();
        break;
      case 5:
        riwayatPesanan();
        break;
      case 6:
        std::cout << "Terima kasih telah menggunakan layanan kami!\n";
        break;
      default:
        std::cout << "Pilihan tidak valid!\n";
    }

    std::cin.ignore();
    std::cin.get();

  } while (pilihan != 6);
  return 0;
}