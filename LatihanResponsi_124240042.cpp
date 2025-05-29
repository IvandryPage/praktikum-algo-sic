
/**
 * @brief Program IDLIX Tube -- Platofrm streaming video internal kampus
 *
 * Galang Ivandry
 * 124240042
 * Sistem Informasi
 * SI-C
 *
 * Praktikum Algoritma dan Struktur Data
 *
 */

#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>

#pragma region DECLARATIONS
constexpr size_t kMaxArray = 255;

enum class StatusVideo { TERSEDIA = 1, ANTRIAN, DIPUTAR };
struct Video {
  char judul[255];
  int durasi_dalam_menit;
  StatusVideo status;

  Video(const char* judul_input, int durasi_input)
      : durasi_dalam_menit(durasi_input) {
    status = StatusVideo::TERSEDIA;
    std::strcpy(judul, judul_input);
  }

  std::string getStatus() {
    switch (status) {
      case StatusVideo::TERSEDIA:
        return "Tersedia";
      case StatusVideo::ANTRIAN:
        return "Dalam Antrean";
      case StatusVideo::DIPUTAR:
        return "Sedang Diputar";
      default:
        return "Undefined";
    }
  }

  bool operator==(const Video& rhs) const {
    return std::strcmp(judul, rhs.judul) == 0;
  }

  bool operator<(const Video& rhs) const {
    return std::strcmp(judul, rhs.judul) < 0;
  }
  bool operator>(const Video& rhs) const {
    return std::strcmp(judul, rhs.judul) > 0;
  }
};

// Struktur Data
struct TreeNode {
  Video data_video;
  TreeNode* anak_kiri;
  TreeNode* anak_kanan;

  TreeNode(const Video& video_input)
      : data_video(video_input), anak_kiri(nullptr), anak_kanan(nullptr) {}
};

class Tree {
 public:
  size_t node_counter;
  Tree() : akar(nullptr) { node_counter = 0; }

  bool empty();
  void insert(const Video& video_input);
  void deleteNode(TreeNode*&, Video& target);
  void inorder(TreeNode* parent, std::function<void(Video& video)> print_style);

  template <bool ReturnValue>
  auto searchBST(const char* target) {
    TreeNode* bantu = this->akar;
    while (bantu) {
      int comparison = std::strcmp(target, bantu->data_video.judul);
      if (comparison == 0) {
        break;
      }

      bantu = (comparison < 0) ? bantu->anak_kiri : bantu->anak_kanan;
    }

    if constexpr (ReturnValue) {
      return bantu;
    }
  }

  TreeNode* akar;
};

struct QueueNode {
  Video data_video;
  QueueNode* next;

  QueueNode(const Video& video_input)
      : data_video(video_input), next(nullptr) {};
};

class Queue {
 public:
  Queue() : depan(nullptr), belakang(nullptr) {}
  void dequeue();
  void enqueue(Video& video);

  QueueNode* depan;
  QueueNode* belakang;
};

enum class Action { ADD_VIDEO, REMOVE_VIDEO, ADD_TO_PLAYLIST, WATCH_VIDEO };
struct StackNode {
  Video data_video;
  Action action;
  StackNode* next;
  StackNode(const Video& video_input)
      : data_video(video_input), next(nullptr) {};
};

class Stack {
 public:
  Stack() : bawah(nullptr), atas(nullptr) {}
  void push(const Video& video);
  void pop();

  StackNode* bawah;
  StackNode* atas;
};

struct UI {
  static void clearScreen();
  static void clearLine();
  static void moveCursor(int baris, int kolom);
  static void moveUpCursor();
  static void saveCursor();
  static void restoreCursor();
  static void printBorder(const std::string& judul_halaman, int jumlah_baris);
  static void printTable(Tree& data_video);
};

struct Input {
  static void pause();

  template <typename TypeTemplate>
  static TypeTemplate getNumber(const std::string& prompt = "");

  static void getText(char* output_text, const std::string& prompt = "");

  static void normalize(const char* string_input);
};

// Main App
class IdlixTube {
 public:
  void run();

 private:
  Tree data_video_;
  Queue playlist_;
  Stack action_history_;
  Stack watch_history_;

  void addNewVideo();
  void showVideos();
  void searchVideo();
  void addToPlaylist();
  void watchVideo();
  void watchHistory();
  void removeVideo();
  void undoAction();

  void getStarterData();
};
#pragma endregion

// ENTRY POINT
int main() {
  IdlixTube app;
  app.run();
  return 0;
}

#pragma region IDLIXTUBE DEFINITIONS
void IdlixTube::getStarterData() {
  Video arr[]{
      Video("robot pemilah sampah", 5),  Video("printer 3d mini", 4),
      Video("lampu pintar sensor", 3),   Video("sepeda listrik rakitan", 6),
      Video("cctv webcam raspberry", 4), Video("panel surya charger", 3),
      Video("mesin jual otomatis", 5),   Video("drone buatan sendiri", 6),
      Video("baterai kentang led", 2),   Video("kalkulator sentuh", 3)};

  for (size_t i = 0; i < std::size(arr); ++i) {
    Video new_video = arr[i];
    data_video_.insert(new_video);
    action_history_.push(new_video);
    action_history_.atas->action = Action::ADD_VIDEO;
  }
}

void IdlixTube::run() {
  IdlixTube::getStarterData();
  while (true) {
    UI::clearScreen();
    UI::printBorder("Idlix Tube", 11);

    std::cout << "Pilihan Menu:\n";
    std::cout << "1. Add Video\n";
    std::cout << "2. Show Videos\n";
    std::cout << "3. Add to Playlist\n";
    std::cout << "4. Watch Video\n";
    std::cout << "5. Watch History\n";
    std::cout << "6. Remove Video\n";
    std::cout << "7. Undo Last Action\n";
    std::cout << "8. Exit\n\n";

    enum MainMenu {
      ADD_NEW_VIDEO = 1,
      SHOW_VIDEOS,
      ADD_TO_PLAYLIST,
      WATCH_VIDEO,
      WATCH_HISTORY,
      REMOVE_VIDEO,
      UNDO_ACTION,
      EXIT
    };

    short menu = Input::getNumber<short>("> Pilih Menu");

    UI::clearScreen();
    switch (menu) {
      case ADD_NEW_VIDEO:
        IdlixTube::addNewVideo();
        break;
      case SHOW_VIDEOS:
        IdlixTube::showVideos();
        break;
      case ADD_TO_PLAYLIST:
        IdlixTube::addToPlaylist();
        break;
      case WATCH_VIDEO:
        IdlixTube::watchVideo();
        break;
      case WATCH_HISTORY:
        IdlixTube::watchHistory();
        break;
      case REMOVE_VIDEO:
        IdlixTube::removeVideo();
        break;
      case UNDO_ACTION:
        IdlixTube::undoAction();
        break;
      case EXIT:
        return;
      default:
        std::cout << "Unavailable menu option!\n";
        Input::pause();
    }
  }
}

void IdlixTube::addNewVideo() {
  UI::printBorder("IdlixTube : Add New Videos", 5);

  char judul[kMaxArray];
  Input::getText(judul, "Judul\t\t");

  int durasi_dalam_menit;
  durasi_dalam_menit = Input::getNumber<int>("Durasi(menit)\t");

  Video new_video(judul, durasi_dalam_menit);
  data_video_.insert(new_video);

  action_history_.push(new_video);
  action_history_.atas->action = Action::ADD_VIDEO;

  UI::clearLine();
  std::cout << "\nAdd Video: " << action_history_.atas->data_video.judul
            << " berhasil!\n";
  Input::pause();
}

void IdlixTube::showVideos() {
  UI::printBorder("IdlixTube : Show Videos", data_video_.node_counter + 5);

  if (data_video_.empty()) {
    std::cout << "Data video masih kosong. Silahkan ke menu tambah data!\n";
    Input::pause();
    return;
  }

  UI::printTable(data_video_);

  std::cout << "\nApakah Anda ingin mencari video (y/t)? ";
  char search;
  std::cin >> search;
  search = std::tolower(search);

  if (search == 'y') {
    IdlixTube::searchVideo();
  }

  return;
}

void IdlixTube::searchVideo() {
  UI::clearScreen();
  UI::printBorder("Idlix Tube : Search Video", 5);

  char judul_input[kMaxArray];
  Input::getText(judul_input, "Cari Judul");

  TreeNode* target = data_video_.searchBST<true>(judul_input);
  if (target) {
    std::cout << "Judul\t\t:" << target->data_video.judul
              << "\nDurasi(menit)\t:" << target->data_video.durasi_dalam_menit
              << "\nStatus\t\t:" << target->data_video.getStatus() << "\n";
  } else {
    std::cout << "Search Video: " << judul_input
              << " gaga!\nData tidak ditemukan\n";
  }

  Input::pause();
}

void IdlixTube::addToPlaylist() {
  UI::printBorder("IdlixTube : Add to Playlist", data_video_.node_counter + 7);
  UI::printTable(data_video_);
  char target[255];

  std::cout << "\n";
  Input::getText(target, "> Input Judul");

  TreeNode* target_node = data_video_.searchBST<true>(target);

  if (target_node && target_node->data_video.status == StatusVideo::TERSEDIA) {
    target_node->data_video.status = StatusVideo::ANTRIAN;

    playlist_.enqueue(target_node->data_video);
    action_history_.push(target_node->data_video);
    action_history_.atas->action = Action::ADD_TO_PLAYLIST;

    std::cout << "Add To Playlist: " << action_history_.atas->data_video.judul
              << " berhasil!\n";
    Input::pause();
  } else {
    std::cout << "Add To Playlist: " << target << " gagal!\n";
    Input::pause();
  }
}

void IdlixTube::watchVideo() {
  UI::printBorder("IdlixTube : Watch", 4);

  if (!playlist_.depan) {
    std::cout << "Playlist masih kosong!\n";
    Input::pause();
    return;
  }

  std::cout << "Sedang diputar: " << playlist_.depan->data_video.judul << "\n";
  TreeNode* ditonton =
      data_video_.searchBST<true>(playlist_.depan->data_video.judul);
  ditonton->data_video.status = StatusVideo::TERSEDIA;

  action_history_.push(playlist_.depan->data_video);
  action_history_.atas->action = Action::REMOVE_VIDEO;

  watch_history_.push(ditonton->data_video);

  playlist_.dequeue();
  if (playlist_.depan) {
    playlist_.depan->data_video.status = StatusVideo::DIPUTAR;
  }
  Input::pause();
}

void IdlixTube::watchHistory() {
  UI::printBorder("IdlixTube : Watch History", 10);
  StackNode* node = watch_history_.atas;

  std::cout << "Riwayat: \n";
  while (node) {
    std::cout << std::setw(50)
              << std::string(node->data_video.judul).substr(0, 50)
              << std::setw(7) << node->data_video.durasi_dalam_menit << "\n";
    node = node->next;
  }

  std::cout << "\n";
  Input::pause();
}

void IdlixTube::removeVideo() {
  UI::printBorder("IdlixTube : Delete Video", data_video_.node_counter + 6);
  UI::printTable(data_video_);
  char target[255];

  std::cout << "\n";
  Input::getText(target, "> Input Judul");

  TreeNode* target_node = data_video_.searchBST<true>(target);

  if (target_node) {
    target_node->data_video.status = StatusVideo::TERSEDIA;
    action_history_.push(target_node->data_video);
    action_history_.atas->action = Action::REMOVE_VIDEO;

    data_video_.deleteNode(data_video_.akar, target_node->data_video);

    std::cout << "Delete Video: " << action_history_.atas->data_video.judul
              << " berhasil!\n";
  } else {
    std::cout << "Delete Video: " << target << " gagal!\n";
  }

  Input::pause();
}

void IdlixTube::undoAction() {
  UI::clearScreen();
  UI::printBorder("IdlixTube : Undo Action", 4);
  if (!action_history_.atas) {
    std::cout << "Tidak ada action!\n";
    Input::pause();
    return;
  }

  StackNode* last_action = action_history_.atas;

  switch (last_action->action) {
    case Action::ADD_TO_PLAYLIST:
      playlist_.dequeue();
      std::cout << "Add To Playlist: " << last_action->data_video.judul
                << " dibatalkan!\n";
      break;
    case Action::ADD_VIDEO:
      data_video_.deleteNode(data_video_.akar, last_action->data_video);
      std::cout << "Add Video: " << last_action->data_video.judul
                << " dibatalkan!\n";
      break;
    case Action::REMOVE_VIDEO:
      data_video_.insert(last_action->data_video);
      std::cout << "Delete Video: " << last_action->data_video.judul
                << " dibatalkan!\n";
      break;
    case Action::WATCH_VIDEO:
      playlist_.enqueue(last_action->data_video);
      std::cout << "Watch Video: " << last_action->data_video.judul
                << " dibatalkan!\n";
      break;
  }

  action_history_.pop();
  Input::pause();
}

#pragma endregion

#pragma region UI DEFINITIONS
void UI::clearScreen() {
#ifdef __WIN32__
  system("cls");
#else
  system("clear");
#endif
}

void UI::clearLine() { std::cout << "\033[K"; }

void UI::moveUpCursor() { std::cout << "\033[F"; }

void UI::restoreCursor() { std::cout << "\033[u"; }

void UI::saveCursor() { std::cout << "\033[s"; }

void UI::printBorder(const std::string& judul_halaman, int jumlah_baris) {
  const int width = 61;
  UI::moveCursor(1, 1);
  std::cout << std::setfill('-') << std::setw(width) << '-';
  UI::moveCursor(jumlah_baris + 4, 1);
  std::cout << std::setfill('-') << std::setw(width) << '-'
            << std::setfill(' ');

  UI::moveCursor(1, width / 2 - 7);
  std::cout << " " << judul_halaman << " ";

  UI::moveCursor(3, 1);
}

void UI::printTable(Tree& data_video) {
  std::cout << std::left << std::setw(50) << "Judul Video" << std::setw(7)
            << "Durasi" << std::setw(15) << "Status"
            << "\n\n";
  auto print_style = [](Video& video) {
    std::cout << std::setw(50) << std::string(video.judul).substr(0, 50)
              << std::setw(7) << video.durasi_dalam_menit << std::setw(15)
              << video.getStatus() << "\n";
  };

  data_video.inorder(data_video.akar, print_style);
}

void UI::moveCursor(int baris, int kolom) {
  std::cout << "\033[" << baris << ";" << kolom << "H";
}
#pragma endregion

#pragma region INPUT DEFINITIONS
void Input::pause() {
#ifdef __WIN32__
  system("pause");
#else
  std::cin.ignore();
  std::cout << "\033[KTekan tombol enter untuk melanjutkan...";
  std::cin.get();
#endif
}

template <typename TypeTemplate>
TypeTemplate Input::getNumber(const std::string& prompt) {
  static_assert(std::is_arithmetic<TypeTemplate>::value,
                "Hanya tipe data numerik!");

  TypeTemplate value{};

  do {
    UI::saveCursor();
    std::cout << prompt << ": ";

    std::cin >> value;
    if (!std::cin.fail()) {
      return value;
    }
    UI::restoreCursor();
    std::cout << "\nHanya menerima input angka!";
    UI::moveUpCursor();
    UI::clearLine();

    std::cin.clear();
    std::cin.ignore();
  } while (true);
}

void Input::getText(char* output_text, const std::string& prompt) {
  std::cout << "\033[s" << prompt << ": ";

  std::cin.ignore();
  std::cin.getline(output_text, kMaxArray);
  output_text[254] = '\0';
}
#pragma endregion

#pragma region TREE DEFINITIONS
bool Tree::empty() { return !akar; }

void Tree::insert(const Video& video_input) {
  if (!akar) {
    akar = new TreeNode(video_input);
    ++node_counter;
    return;
  }

  TreeNode* bantu = akar;

  while (true) {
    if (video_input == bantu->data_video) {
      return;
    }

    TreeNode*& next_node = (video_input < bantu->data_video)
                               ? bantu->anak_kiri
                               : bantu->anak_kanan;
    if (!next_node) {
      next_node = new TreeNode(video_input);
      ++node_counter;
      return;
    }

    bantu = next_node;
  }
}

void Tree::inorder(TreeNode* parent,
                   std::function<void(Video& video)> print_style) {
  if (!parent) {
    return;
  }

  inorder(parent->anak_kiri, print_style);
  print_style(parent->data_video);
  inorder(parent->anak_kanan, print_style);
}

void Tree::deleteNode(TreeNode*& parent, Video& target) {
  if (parent == nullptr) return;

  if (target < parent->data_video)
    deleteNode(parent->anak_kiri, target);
  else if (target > parent->data_video)
    deleteNode(parent->anak_kanan, target);
  else {
    TreeNode* bantu = parent;
    if (parent->anak_kiri == nullptr)
      parent = parent->anak_kanan;
    else if (parent->anak_kanan == nullptr)
      parent = parent->anak_kiri;
    else {
      TreeNode* pengganti = parent->anak_kanan;
      while (pengganti->anak_kiri) {
        pengganti = pengganti->anak_kiri;
      }
      parent->data_video = pengganti->data_video;
      deleteNode(parent->anak_kanan, pengganti->data_video);
      return;
    }

    delete bantu;
  }
}
#pragma endregion

#pragma region QUEUE DEFINITIONS
void Queue::enqueue(Video& video) {
  QueueNode* queue = new QueueNode(video);

  if (!depan) {
    queue->data_video.status = video.status = StatusVideo::DIPUTAR;
    depan = belakang = queue;
    return;
  }

  belakang->next = queue;
  belakang = queue;
}

void Queue::dequeue() {
  if (!depan) {
    std::cout << "Playlist masih kosong!\n";
    return;
  }

  QueueNode* target = depan;
  depan = depan->next;
  delete target;
}
#pragma endregion

#pragma region STACK DEFINITIONS
void Stack::push(const Video& video) {
  StackNode* new_node = new StackNode(video);

  if (!bawah) {
    bawah = atas = new_node;
    return;
  }
  new_node->next = atas;
  atas = new_node;
}

void Stack::pop() {
  if (!bawah) {
    std::cout << "Belum ada riwayat tontonan!\n";
    return;
  }

  StackNode* x = atas;
  atas = atas->next;
  delete x;
}
#pragma endregion