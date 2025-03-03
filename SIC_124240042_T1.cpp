#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>

/*
 * @brief Tugas Praktikum Algoritma Pemrograman dan Struktur Data
 *        Tugas 1 - Study case Toko Roti AK
 * Nama : Galang Ivandry
 * NIM  : 124240042
 * Kelas: SI-C
 * PRodi: Sistem Informasi
 */

enum MainMenu {SHOW_ROTI, CARI_KODE, CARI_NAMA, SORT_ASC, SORT_DESC, EXIT};
bool is_sorted{false};

struct Roti {
  std::string nama;
  int kode;
  int harga;
};


/* 
 * @brief Hanya untuk modifikasi tampilan pada console
 */
class Text {
public:
  static constexpr int kAnsi = 8;
  static std::string bold(std::string_view text) { 
    return "\033[1m" + std::string(text) + "\033[0m"; 
  }
  
  static std::string faintOff(std::string_view text) { 
    return "\033[2m" + std::string(text) + "\033[0m";; 
  }

  static std::string italic(std::string text) { 
    return "\033[3m" + std::string(text) + "\033[0m";
  }
  
  static std::string underline(std::string text) { 
    return "\033[4m" + std::string(text) + "\033[0m"; 
  }
};

void clearScreen() {
#ifdef __WIN32__
    system("cls");
#else
    system("clear");
#endif
}

void displayMenu(std::string* menu, size_t size) {
  std::cout << Text::italic("Menu:\n");
  for (size_t i = 0; i < size; i++) {
    std::cout << Text::bold(std::to_string(i + 1)) << ". " << *(menu + i) << "\n";
  }
}

/*
 * @brief fungsi yang dapat digunakan untuk segala jenis variable tipe numerik
 *        untuk memastikan input berupa angka
 */

template <typename TypeTemplate>
static TypeTemplate getNumberInput(const std::string& prompt = "") {
  static_assert(std::is_arithmetic<TypeTemplate>::value, "Hanya tipe data numerik!");

  TypeTemplate value{};
  do {
    std::cout << prompt;
    std::cin >> value;

    if (!std::cin.fail()) {
      return value;
    }

    std::cin.clear();
    std::cin.ignore();
    std::cout << "Harap hanya input angka!\n";
  } while (true);
}

short userInputMenu(short menu_size) {
  short input;
  bool valid{true};
  do {
    input = getNumberInput<short>(Text::faintOff("\n> Ketik Pilihan Anda: "));

    if (input > menu_size || input <= 0) {
      std::cout << "Menu tidak tersedia!\n";
      valid = false;
    }
  } while (!valid);
  
  return input - 1;
}


/*
 * @brief Fungsi menu pertama: menampilkan data roti dalam bentuk tabel menggunakan pointer
 */
void displayRoti(Roti* roti, size_t* size) {
  std::cout << std::left << std::setw(38) << std::setfill('=') << "" << std::setfill(' ') << "\n";
  std::cout << std::setw(18 + Text::kAnsi) << Text::bold("Nama")
            << std::setw(10 + Text::kAnsi) << Text::bold("Kode")
            << std::setw(10 + Text::kAnsi) << Text::bold("Harga") << "\n";
  std::cout << std::left << std::setw(38) << std::setfill('=') << "" << std::setfill(' ') << "\n";
  
  for (int i = 0; i < *size; i++) {
    std::cout << std::setw(18) << (roti+i)->nama.substr(0, 18)
              << std::right << std::setw(3) << std::setfill('0') << ((roti+i)->kode)
              << std::left << std::setw(7) << std::setfill(' ') << ""
              << std::setw(10) << (roti+i)->harga << "\n";
  }
  std::cout << std::left << std::setw(38) << std::setfill('=') << "" << std::setfill(' ') << "\n";
}

void displayHasilPencarian(Roti* roti, bool found, int index, std::string input) {
  if (found) {
    std::cout << Text::bold("\nRoti: ") << (roti+index)->nama 
              << ", " << Text::bold("Kode: ") << std::right << std::setw(3) << std::setfill('0') << (roti+index)->kode 
              << ", " << Text::bold("Harga: ") << (roti+index)->harga << "\n";
  } else {
    std::cout << "\nRoti dengan kode " << input << " tidak terdaftar.\n";
  }
}

/*
 * @brief fungsi searching berdasarkan kode menggunakan sequential search
 */

bool linearSearch(Roti* roti, size_t* size, int* index, int kode_cari) {
  for (int i = 0; i < *size; i++) {
    if ((roti+i)->kode == kode_cari) {
      *index = i;
      return true;
    }
  }
  return false;
}

void cariRotiByKode(Roti* roti, size_t* size) {
  std::cout << "\nMasukkan kode: ";
  int kode_cari = getNumberInput<int>();

  int index{0};
  bool found = linearSearch(roti, size, &index, kode_cari);
  displayHasilPencarian(roti, found, index, std::to_string(kode_cari));
}

/*
 * @brief Fungsi searching berdasarkan nama menggunakan binary Search
 */

bool binarySearch(Roti* roti, size_t* size, int* index, std::string nama_cari) {
  int awal{0}, tengah, akhir{*size - 1};
  bool found{false};

  for (size_t i = 0; i < *size - 1; i++) {
    for (size_t j = 0; j < *size - 1 - i; j++) {
      if ((roti+j)->nama > (roti + j + 1)->nama) {
        std::swap(roti[j], roti[j + 1]);
      }
    }
  }

  std::transform(nama_cari.begin(), nama_cari.end(), nama_cari.begin(), ::tolower);

  do {
    tengah = (awal + akhir) / 2;
    std::string mid = (roti+tengah)->nama;
    std::transform(mid.begin(), mid.end(), mid.begin(), ::tolower);
    if (mid < nama_cari) {
      awal = tengah + 1;
    } else if (mid > nama_cari) {
      akhir = tengah - 1;
    } else {
      found = true;
      *index = tengah;
    }
  } while (awal <= akhir && !found);

  return found;
}

void cariRotiByNama(Roti* roti, size_t* size) {
  std::cout << "\nMasukkan nama: ";
  std::string nama_cari;
  std::cin.ignore();
  std::getline(std::cin, nama_cari);

  int index{0};
  bool found = binarySearch(roti, size, &index, nama_cari);
  displayHasilPencarian(roti, found, index, nama_cari);
}

/*
 * @brief Fungsi urutan secara ascending dengan quick sort
 */
void quickSort(Roti roti[], int awal, int akhir) {
  int low = awal, high = akhir;
  int pivot = roti[(awal + akhir) / 2].harga;

  do {
    while (roti[low].harga < pivot) {
      low++;
    }

    while (roti[high].harga > pivot) {
      high--;
    }

    if (low <= high) {
      std::swap(roti[low], roti[high]);
      low++; high--;
    }
  } while (low <= high);

  if (low < akhir) {
    quickSort(roti, low, akhir);
  }

  if (awal < high) {
    quickSort(roti, awal, high);
  }
}

/*
 * @brief Fungsi pengurutan Bubble Sort secara descending
 */

void bubbleSort(Roti* roti, size_t size) {
  for (size_t i = 0; i < size - 1; i++) {
    for (size_t j = 0; j < size - 1 - i; j++) {
      if ((roti+j)->harga < (roti + j + 1)->harga) {
        std::swap(roti[j], roti[j + 1]);
      }
    }
  }
}

int main() {
  Roti data_roti[] {
    {"Roti Keju", 1 , 15000},
    {"Roti Coklat", 2,18000},
    {"Roti Tawar", 3, 20000},
    {"Roti Gandum", 4, 22000},
    {"Roti Pisang", 5, 17000}
  };
  size_t size{std::size(data_roti)};

  clearScreen();
  std::cout << "For better experience, please turn on ANSI character support on your terminal";
  do {
    std::cout << "\n\n" << std::left << std::setw(12) << std::setfill('=') << ""
              << Text::bold(" Toko Roti AK ") << std::setw(12)
              << std::setfill('=') << "" << "\n\n";

    std::string menu[]{"Tampilkan Roti", "Cari berdasarkan kode", "Cari berdasarkan nama", "Sort Harga Roti (asc)", "Sort Harga Roti (desc)", "Exit"};
    displayMenu(menu, std::size(menu));

    int pilihan_menu = userInputMenu(std::size(menu));
    clearScreen();
    switch (pilihan_menu) {
      case SHOW_ROTI:
        displayRoti(data_roti, &size);
        break;
      case CARI_KODE:
        cariRotiByKode(data_roti, &size);
        break;
      case CARI_NAMA:
        cariRotiByNama(data_roti, &size);
        quickSort(data_roti, 0, size - 1);
        break;
      case SORT_ASC:
        quickSort(data_roti, 0, size - 1);
        displayRoti(data_roti, &size);
        std::cout << "\nData telah diurutkan secara ascending berdasarkan harga\n";
        break;
      case SORT_DESC:
        bubbleSort(data_roti, size);
        displayRoti(data_roti, &size);
        std::cout << "\nData telah diurutkan secara descending berdasarkan harga\n";
        break;
      case EXIT: 
        exit(0);
        break;
    }
  } while (1);

  return 0;
}