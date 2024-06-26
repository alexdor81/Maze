#ifndef SRC_MODEL_MAZE_H_
#define SRC_MODEL_MAZE_H_

#include <time.h>

#include <fstream>
#include <iostream>
#include <vector>

namespace s21 {

using position = std::pair<int, int>;

class Maze {
 public:
  Maze();
  Maze(int rows, int cols);
  Maze(const Maze& other);
  ~Maze();

  bool& operator()(int i, int j);
  Maze operator=(const Maze& other);

  int GetRows() { return rows_; }
  int GetCols() { return cols_; }

 private:
  int rows_;
  int cols_;
  bool** maze_;
  void InitMaze(int rows, int cols);
  void CopyMaze(bool** other);
  void DestroyMaze();
};

class MazeGen {
 public:
  int GetRows() { return rows_; }
  int GetCols() { return cols_; }
  Maze& GetVertWalls() { return v_walls_; }
  Maze& GetHorWalls() { return h_walls_; }
  void SetSize(int rows, int cols);
  void FromFile(std::string filename);
  void GenerateMaze();
  std::vector<position> FindWay(position begin, position end);
  void SaveFile(const std::string& filepath);

 private:
  int rows_ = 0;
  int cols_ = 0;
  int count_ = 1;
  Maze v_walls_;
  Maze h_walls_;
  std::vector<int> num_;
  std::ifstream file_;
  int** map_ = nullptr;

  void InitWalls();
  void ParseSize();
  void ParseWalls(Maze& maze);
  void UniqSet();
  void MergeSet(int i, int number);
  void AddVertWalls(int row);
  void AddHorWalls(int row);
  void AddEndLine();
  bool CheckValue(const char value);
  void InitMap();
  void DestroyMap();
  int ChangeCell(int value, int cell);
  int PossibleSteps(int step);
};

}  // namespace s21

#endif  // SRC_MODEL_MAZE_H_
