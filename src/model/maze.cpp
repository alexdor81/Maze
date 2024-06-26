#include "maze.h"

namespace s21 {

Maze::Maze() : rows_(0), cols_(0), maze_(nullptr) {}

Maze::Maze(int rows, int cols) { InitMaze(rows, cols); }

Maze::Maze(const Maze& other) : Maze(other.rows_, other.cols_) {
  CopyMaze(other.maze_);
}

Maze::~Maze() { DestroyMaze(); }

bool& Maze::operator()(int i, int j) { return maze_[i][j]; }

Maze Maze::operator=(const Maze& other) {
  if (this != &other) {
    DestroyMaze();
    InitMaze(other.rows_, other.cols_);
    CopyMaze(other.maze_);
  }
  return *this;
}

void Maze::InitMaze(int rows, int cols) {
  rows_ = rows;
  cols_ = cols;
  maze_ = new bool*[rows];
  for (int i = 0; i < rows; i++) maze_[i] = new bool[cols]();
}

void Maze::CopyMaze(bool** other) {
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) maze_[i][j] = other[i][j];
}

void Maze::DestroyMaze() {
  if (maze_) {
    for (int i = 0; i < rows_; i++) delete[] maze_[i];
    delete[] maze_;
    maze_ = nullptr;
  }
}

void MazeGen::SetSize(int rows, int cols) {
  rows_ = rows;
  cols_ = cols;
  InitWalls();
}

void MazeGen::InitWalls() {
  v_walls_ = Maze(rows_, cols_);
  h_walls_ = Maze(rows_, cols_);
}

void MazeGen::FromFile(std::string filename) {
  file_ = std::ifstream(filename);
  std::string line;
  ParseSize();
  InitWalls();
  ParseWalls(v_walls_);
  getline(file_, line);
  ParseWalls(h_walls_);
}

void MazeGen::ParseSize() {
  std::string line;
  getline(file_, line);
  size_t sub_pos = 0;
  rows_ = std::stoi(line, &sub_pos);
  cols_ = std::stoi(line.substr(sub_pos + 1), &sub_pos);
  if ((rows_ < 0 || rows_ > 50) && (cols_ < 0 || cols_ > 50))
    throw std::invalid_argument("Wrong maze size");
}

void MazeGen::ParseWalls(Maze& maze) {
  for (int i = 0; i < rows_; i++) {
    std::string line;
    getline(file_, line);
    for (int j = 0, k = 0; j < cols_; j++, k += 2)
      maze(i, j) = CheckValue(line.at(k));
  }
}

bool MazeGen::CheckValue(const char value) {
  if (value != '1' && value != '0')
    throw std::invalid_argument("Incorrect value");
  return value == '1';
}

void MazeGen::UniqSet() {
  for (int i = 0; i < cols_; i++)
    if (num_[i] == 0) {
      num_[i] = count_;
      count_++;
    }
}

void MazeGen::MergeSet(int i, int number) {
  int set = num_[i + 1];
  for (int j = 0; j < cols_; j++)
    if (num_[j] == set) num_[j] = number;
}

void MazeGen::AddVertWalls(int row) {
  for (int i = 0; i < cols_ - 1; i++) {
    bool wall = rand() % 2;
    if (wall || num_[i] == num_[i + 1])
      v_walls_(row, i) = true;
    else
      MergeSet(i, num_[i]);
  }
  v_walls_(row, cols_ - 1) = true;
}

void MazeGen::AddHorWalls(int row) {
  for (int i = 0; i < cols_; i++) h_walls_(row, i) = rand() % 2;
  for (int i = 0; i < cols_;) {
    int walls = 0;
    int j = i;
    while (j < cols_ && num_[i] == num_[j]) {
      if (!h_walls_(row, j)) walls++;
      j++;
    }
    if (!walls) h_walls_(row, i) = false;
    i = j;
  }
  for (int i = 0; i < cols_; i++)
    if (h_walls_(row, i)) num_[i] = 0;
}

void MazeGen::AddEndLine() {
  UniqSet();
  AddVertWalls(rows_ - 1);
  for (int i = 0; i < cols_ - 1; i++) {
    if (num_[i] != num_[i + 1]) {
      v_walls_(rows_ - 1, i) = false;
      MergeSet(i, num_[i]);
    }
    h_walls_(rows_ - 1, i) = true;
  }
  h_walls_(rows_ - 1, cols_ - 1) = true;
}

void MazeGen::GenerateMaze() {
  srand(time(NULL));
  for (int i = 0; i < cols_; i++) num_.push_back(0);
  for (int i = 0; i < rows_ - 1; i++) {
    UniqSet();
    AddVertWalls(i);
    AddHorWalls(i);
  }
  AddEndLine();
  num_.clear();
  count_ = 1;
}

void MazeGen::InitMap() {
  map_ = new int*[rows_];
  for (int i = 0; i < rows_; i++) {
    map_[i] = new int[cols_];
    for (int j = 0; j < cols_; j++) map_[i][j] = -1;
  }
}

void MazeGen::DestroyMap() {
  if (map_) {
    for (int i = 0; i < rows_; i++) delete[] map_[i];
    delete[] map_;
    map_ = nullptr;
  }
}

int MazeGen::ChangeCell(int value, int cell) {
  if (cell == -1)
    cell = value;
  else
    cell = std::min(cell, value);
  return cell;
}

int MazeGen::PossibleSteps(int step) {
  int result = 0;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (map_[i][j] == step) {
        result++;
        if (i < rows_ - 1 && !h_walls_(i, j))
          map_[i + 1][j] = ChangeCell(step + 1, map_[i + 1][j]);
        if (i > 0 && !h_walls_(i - 1, j))
          map_[i - 1][j] = ChangeCell(step + 1, map_[i - 1][j]);
        if (j < cols_ - 1 && !v_walls_(i, j))
          map_[i][j + 1] = ChangeCell(step + 1, map_[i][j + 1]);
        if (j > 0 && !v_walls_(i, j - 1))
          map_[i][j - 1] = ChangeCell(step + 1, map_[i][j - 1]);
      }
    }
  }
  return result;
}

std::vector<position> MazeGen::FindWay(position begin, position end) {
  std::vector<position> path;
  int x = end.first;
  int y = end.second;
  int count = 1;
  int step = 0;
  InitMap();
  map_[begin.second][begin.first] = 0;
  while (count > 0 && map_[y][x] == -1) count = PossibleSteps(step++);
  if (map_[y][x] != -1) {
    step = map_[y][x];
    path.push_back({x, y});
    while (y != begin.second || x != begin.first) {
      if (y < rows_ - 1 && !h_walls_(y, x) && map_[y + 1][x] == step - 1)
        y++;
      else if (y > 0 && !h_walls_(y - 1, x) && map_[y - 1][x] == step - 1)
        y--;
      else if (x < cols_ - 1 && !v_walls_(y, x) && map_[y][x + 1] == step - 1)
        x++;
      else if (x > 0 && !v_walls_(y, x - 1) && map_[y][x - 1] == step - 1)
        x--;
      path.push_back({x, y});
      step--;
    }
  }
  DestroyMap();
  return path;
}

void MazeGen::SaveFile(const std::string& filepath) {
  std::ofstream file;
  file.open(filepath);
  if (!file.is_open()) {
    throw std::invalid_argument("Failed saving to file");
  } else {
    file << GetRows() << ' ' << GetCols() << '\n';
    for (int i = 0; i < GetRows(); i++) {
      for (int j = 0; j < GetCols(); j++) {
        file << GetVertWalls()(i, j) << ' ';
      }
      file << '\n';
    }
    file << '\n';
    for (int i = 0; i < GetRows(); i++) {
      for (int j = 0; j < GetCols(); j++) {
        file << GetHorWalls()(i, j) << ' ';
      }
      file << '\n';
    }
  }
  file.close();
}

}  // namespace s21
