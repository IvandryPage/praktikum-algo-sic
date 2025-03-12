#include <iostream>

/*
 * @brief Latihan Kuis Algoritma Pemrograman dan Struktur Data SI-C
 *        NAMA : GALANG IVANDRY
 *        NIM  : 124240042
 *        Plug : SI-C
 *        Prodi: Sistem Informasi
 */

/*
 * @brief Kantor Kecamatan Depok membutuhkan program untuk mencatat data
 * kependudukan dengan detail sbb.
 * - Data  : Kependudukan (nomor kependudukan, nama, alamat, dan jenis kelamin)
 * - Fitur : Input Data, Output Data, Search Data, Edit Data, Hapus Data
 * - Requirement : 1. Data disimpan dalam file (append bukan overwrite)
 *                 2. Search, Edit, dan Hapus berdasarkan Nomor Kependudukan
 *                 3. Output data urut secara ascending berdasarkan nomor
 * kependudukan
 * - Bonus       : Menambahkan error handling
 */

struct Penduduk {
  char nik[16], nama[100], alamat[255], jenis_kelamin;
};

constexpr int kMaxData = 1000;
constexpr const char *kFileName = "depok.db";

void clearScreen();
void tampilkanPenduduk(const Penduduk &penduduk);
Penduduk inputPenduduk();
void saveToFile(Penduduk *kependudukan, const char *mode, int jumlah_data = 1);
void loadFromFile(Penduduk *kependudukan, int &jumlah_data);
void quickSort(Penduduk *kependudukan, int awal, int akhir);
bool binarySearch(Penduduk *array, int size, int *index,
                  const char *nik_dicari);
int getValidNumberInput();

void menuInputData(Penduduk *kependudukan, int &jumlah_data);
void menuOutputData(Penduduk *kependudukan, int jumlah_data);
void menuSearchData(Penduduk *kependudukan, int jumlah_data);
void menuEditData(Penduduk *kependudukan, int &jumlah_data);
void menuHapusData(Penduduk *kependudukan, int &jumlah_data);

int main() {
  int jumlah_data = 0;
  Penduduk kependudukan[kMaxData]{};
  loadFromFile(kependudukan, jumlah_data);

  while (true) {
    clearScreen();

    std::cout << "==================================\n";
    std::cout << "     SISTEM KEPENDUDUKAN DEPOK    \n";
    std::cout << "==================================\n";
    std::cout << "(1) Input Data Penduduk\n";
    std::cout << "(2) Tampilkan Data Penduduk\n";
    std::cout << "(3) Cari Data Penduduk\n";
    std::cout << "(4) Edit Data Penduduk\n";
    std::cout << "(5) Hapus Data Penduduk\n";
    std::cout << "(6) Keluar\n";
    std::cout << "---------------------------------\n";

    std::cout << "\n> Ketikkan pilihan: ";
    int user_menu = getValidNumberInput();
    std::cout << '\n';

    switch (user_menu) {
      case 1:
        menuInputData(kependudukan, jumlah_data);
        break;
      case 2:
        menuOutputData(kependudukan, jumlah_data);
        break;
      case 3:
        menuSearchData(kependudukan, jumlah_data);
        break;
      case 4:
        menuEditData(kependudukan, jumlah_data);
        break;
      case 5:
        menuHapusData(kependudukan, jumlah_data);
        break;
      case 6:
        std::cout << "- Terima kasih telah menggunakan sistem!\n";
        return 0;
        break;
      default:;
        std::cout << "Pilihan tidak tersedia!\n";
    }

    std::cout << "Press any key to continue...";
    std::cin.ignore();
    std::cin.get();
  }
}

// Definition dari fitur
void menuInputData(Penduduk *kependudukan, int &jumlah_data) {
  if (jumlah_data >= kMaxData) {
    std::cout << "Kapasitas penuh!";
    return;
  }

  std::cout << "Input Data Penduduk\n";
  std::cout << "Jumlah Input: ";
  int jumlah_input = getValidNumberInput();
  std::cout << "==================================\n";

  if (jumlah_input + jumlah_data >= kMaxData) {
    std::cout << "TIdak cukup kapasitas!\n";
    return;
  }

  for (int i = 0; i < jumlah_input; i++) {
    std::cout << "Data Penduduk ke-" << jumlah_data + i << '\n';
    Penduduk p = inputPenduduk();

    int index_data = -1;
    if (binarySearch(kependudukan, jumlah_data, &index_data, p.nik)) {
      std::cout
          << "\nData gagal ditambahkan karena NIK sudah ada dalam database!\n";
      continue;
    }

    kependudukan[jumlah_data] = p;
    saveToFile(&kependudukan[jumlah_data], "ab");
    jumlah_data++;
  }
}

void menuOutputData(Penduduk *kependudukan, int jumlah_data) {
  std::cout << "Data Penduduk\n";
  std::cout << "==================================\n";
  for (int i = 0; i < jumlah_data; i++) {
    tampilkanPenduduk(*(kependudukan + i));
  }
}

void menuSearchData(Penduduk *kependudukan, int jumlah_data) {
  std::cout << "Cari Data Penduduk\n";
  std::cout << "==================================\n";
  char nik_dicari[16];
  std::cout << "NIK dicari : ";
  std::cin >> nik_dicari;

  int index_data = -1;
  quickSort(kependudukan, 0, jumlah_data);

  if (binarySearch(kependudukan, jumlah_data, &index_data, nik_dicari)) {
    tampilkanPenduduk(*(kependudukan + index_data));
  } else {
    std::cout << "\nData dengan NIK " << nik_dicari << " tidak ditemukan!\n";
  }
}

void menuEditData(Penduduk *kependudukan, int &jumlah_data) {
  std::cout << "Edit Data Penduduk\n";
  std::cout << "==================================\n";
  char nik_dicari[16];
  std::cout << "NIK diedit : ";
  std::cin >> nik_dicari;

  int index_data = -1;
  quickSort(kependudukan, 0, jumlah_data);

  if (binarySearch(kependudukan, jumlah_data, &index_data, nik_dicari)) {
    std::cout << "Alamat baru   : ";
    std::cin.ignore();
    std::cin.getline((kependudukan + index_data)->alamat, 255);
    saveToFile(kependudukan, "wb", jumlah_data);
  } else {
    std::cout << "\nData dengan NIK " << nik_dicari << " tidak ditemukan!\n";
  }
}

void menuHapusData(Penduduk *kependudukan, int &jumlah_data) {
  std::cout << "Hapus Data Penduduk\n";
  std::cout << "==================================\n";
  char nik_dicari[16];
  std::cout << "NIK dihapus : ";
  std::cin >> nik_dicari;

  int index_data = -1;
  if (binarySearch(kependudukan, jumlah_data, &index_data, nik_dicari)) {
    for (int i = index_data; i < jumlah_data; i++) {
      *(kependudukan + i) = *(kependudukan + i + 1);
    }
    jumlah_data--;
    saveToFile(kependudukan, "wb", jumlah_data);
  } else {
    std::cout << "\nData dengan NIK " << nik_dicari << " tidak ditemukan!\n";
  }
}

// Definition dari fungsi bantuan / helper function
void clearScreen() {
#ifdef __WIN32__
  system("cls");
#else
  system("clear");
#endif
}

int getValidNumberInput() {
  int num;
  while (true) {
    std::cin >> num;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "X input tidak  valid! Hanya angka.\n";
    } else {
      std::cin.ignore(10000, '\n');
      return num;
    }
  }
}

void tampilkanPenduduk(const Penduduk &penduduk) {
  std::cout << "NIK          : " << penduduk.nik
            << "\nNama         : " << penduduk.nama
            << "\nAlamat       : " << penduduk.alamat
            << "\nJenis Kelamin: " << penduduk.jenis_kelamin << "\n\n";
}

Penduduk inputPenduduk() {
  Penduduk p;
  std::cout << "NIK          : ";
  std::cin >> p.nik;

  std::cout << "Nama         : ";
  std::cin.ignore();
  std::cin.getline(p.nama, 100);

  std::cout << "Alamat       : ";
  std::cin.getline(p.alamat, 255);

  std::cout << "Jenis Kelamin: ";
  std::cin >> p.jenis_kelamin;
  return p;
}

void saveToFile(Penduduk *kependudukan, const char *mode, int jumlah_data) {
  FILE *fptr;
  fptr = fopen(kFileName, mode);

  if (fptr == NULL) {
    std::cerr << "Gagal fopen()\n";
    exit(-1);
  }

  fwrite(kependudukan, sizeof(Penduduk), jumlah_data, fptr);
  fclose(fptr);
}

void loadFromFile(Penduduk *kependudukan, int &jumlah_data) {
  FILE *fptr = fopen(kFileName, "rb");

  if (fptr == NULL) {
    return;
  }

  // // menghitung berapa data yang ada di dalam file biner
  // // dihitung berdasarkan ukuran data atau file size dari data yang disimpan
  // // pada kasus ini, ukuran struct Penduduk
  // fseek(fptr, 0, SEEK_END);
  // int dat_size = ftell(fptr);
  // rewind(fptr);
  // jumlah_data = dat_size / sizeof(Penduduk);

  jumlah_data = fread(kependudukan, sizeof(Penduduk), kMaxData, fptr);
  fclose(fptr);
}

void quickSort(Penduduk *array, int awal, int akhir) {
  if (awal >= akhir) {
    return;
  }

  int low = awal, high = akhir;
  std::string pivot = array[(awal + akhir) / 2].nik;

  do {
    while (array[low].nik < pivot) {
      low++;
    }

    while (array[high].nik > pivot) {
      high--;
    }

    if (low <= high) {
      std::swap(array[low], array[high]);
      low++;
      high--;
    }
  } while (low <= high);

  if (low < akhir) {
    quickSort(array, low, akhir);
  }

  if (awal < high) {
    quickSort(array, awal, high);
  }
}

bool binarySearch(Penduduk *array, int size, int *index,
                  const char *nik_dicari) {
  quickSort(array, 0, size);
  int awal{0}, tengah, akhir{size - 1};
  bool found{false};

  do {
    tengah = (awal + akhir) / 2;
    std::string mid = (array + tengah)->nik;

    if (mid < nik_dicari) {
      awal = tengah + 1;
    } else if (mid > nik_dicari) {
      akhir = tengah - 1;
    } else {
      found = true;
      *index = tengah;
    }
  } while (awal <= akhir && !found);

  return found;
}