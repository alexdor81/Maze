#ifndef SRC_MODEL_CAVE_H_
#define SRC_MODEL_CAVE_H_

#include <fstream>
#include <iostream>

using namespace std;

namespace s21 {

class Cave {
 public:
  void Init();                  // Инициализая
  void Init(const char* path);  // Инициализация через файл
  void StepRun();  // Запуск пошаговой генерации
  void SetWidth(int width);    // Установка ширины
  void SetHeight(int height);  // Установка высоты
  void SetBirthLimit(int birth_limit);  // Установка границы рождения
  void SetDeathLimit(int death_limit);  // Установка границы смерти
  void SetChance(int chance);  // Установка шанса инициализации клетки
  int GetRows();
  int GetColumns();
  bool*** GetCaveMap();
  void PrintMap();  // Отрисовка текущего состаяния карты

 private:
  void FillCaveMap();
  void FillCaveMap(string* data);
  void ReadCave(const char* path);
  void WriteCave(const char* path);
  void CellularAutomation(bool** prev_state);
  bool** CopyCaveMap();
  bool CompareMap(bool** mapA, bool** mapB);
  void DeleteMap(bool** map);
  void PrintMap(bool** map);
  int width_ = 0;
  int height_ = 0;
  int birth_limit_ = 0;
  int death_limit_ = 0;
  int chance_ = 0;
  bool** cave_map_;
};

}  // namespace s21

#endif  // SRC_MODEL_CAVE_H_
