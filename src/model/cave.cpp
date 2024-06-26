#include "cave.h"

namespace s21 {

void Cave::Init() {
  cave_map_ = new bool*[height_];
  for (int i = 0; i < height_; i++) {
    cave_map_[i] = new bool[width_];
  }
  FillCaveMap();
}

void Cave::Init(const char* path) { ReadCave(path); }

void Cave::FillCaveMap() {
  srand(time(NULL));
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      cave_map_[i][j] = (((rand() % 100) < chance_));
    }
  }
}

void Cave::FillCaveMap(string* data) {
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      cave_map_[i][j] = (data[height_ * i + j] != "1");
    }
  }
}

void Cave::StepRun() {
  bool** prev_state = CopyCaveMap();
  CellularAutomation(prev_state);
  DeleteMap(prev_state);
}

void Cave::ReadCave(const char* path) {
  ifstream fin;
  fin.open(path);

  if (!fin.is_open()) {
    throw std::invalid_argument("Error reading the file");
  } else {
    string temp;
    fin >> temp;
    int height = atoi(temp.c_str());
    fin >> temp;
    int width = atoi(temp.c_str());
    if (!((height > 0 && height <= 50) || (width > 0 && width <= 50))) {
      cout << "Max size: 50 x 50\n";
    } else {
      height_ = height;
      width_ = width;
      Init();
      string* data = new string[height_ * width_];
      int i = 0;
      while (!fin.eof() && i < height_ * width_) {
        fin >> data[i++];
      }
      if (i != height_ * width_) {
        throw std::invalid_argument("Not enought items in file");
      }
      FillCaveMap(data);
    }
    fin.close();
  }
}

void Cave::WriteCave(const char* path) {
  ofstream fout;
  fout.open(path);

  if (!fout.is_open()) {
    throw std::invalid_argument("Error reading the file");
  } else {
    fout << height_ << " " << width_ << "\n";
    for (int i = 0; i < height_; i++) {
      for (int j = 0; j < width_; j++) {
        fout << ((cave_map_[i][j]) ? "1 " : "0 ");
      }
      fout << "\n";
    }
    cout << "Weights saved \n";
    fout.close();
  }
}

void Cave::CellularAutomation(bool** prev_state) {
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      int neighborhoods = 0;
      for (int i_x = i - 1; i_x < i + 2; i_x++) {
        for (int j_y = j - 1; j_y < j + 2; j_y++) {
          if ((i_x < 0 || j_y < 0 || i_x == height_ || j_y == width_) ||
              (i_x == i && j_y == j))
            continue;
          if (prev_state[i_x][j_y]) neighborhoods++;
        }
      }
      if (prev_state[i][j] && neighborhoods <= death_limit_) {
        cave_map_[i][j] = false;
      }
      if (!prev_state[i][j] && neighborhoods > birth_limit_) {
        cave_map_[i][j] = true;
      }
    }
  }
}

bool** Cave::CopyCaveMap() {
  bool** copy_map = new bool*[height_];
  for (int i = 0; i < height_; i++) {
    copy_map[i] = new bool[width_];
    for (int j = 0; j < width_; j++) {
      copy_map[i][j] = cave_map_[i][j];
    }
  }
  return copy_map;
}

bool Cave::CompareMap(bool** mapA, bool** mapB) {
  bool result = true;
  for (int i = 0; i < height_ && result; i++) {
    for (int j = 0; j < width_ && result; j++) {
      if (mapA[i][j] != mapB[i][j]) result = false;
    }
  }
  return result;
}

void Cave::DeleteMap(bool** map) {
  for (int i = 0; i < height_; i++) delete map[i];
  delete map;
}

void Cave::SetWidth(int width) { width_ = width; }
void Cave::SetHeight(int height) { height_ = height; }
void Cave::SetBirthLimit(int birth_limit) { birth_limit_ = birth_limit; }
void Cave::SetDeathLimit(int death_limit) { death_limit_ = death_limit; }
void Cave::SetChance(int chance) { chance_ = chance; }

int Cave::GetRows() { return height_; }
int Cave::GetColumns() { return width_; }
bool*** Cave::GetCaveMap() { return &cave_map_; }

void Cave::PrintMap() { PrintMap(cave_map_); }

void Cave::PrintMap(bool** map) {
  for (int i = 0; i < height_; i++) {
    cout << "|";
    for (int j = 0; j < width_; j++) {
      if (map[i][j])
        cout << "██";
      else
        cout << "  ";
    }
    cout << "|" << endl;
  }
}

}  // namespace s21
