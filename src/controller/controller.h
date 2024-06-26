#ifndef SRC_CONTROLLER_CONTROLLER_H_
#define SRC_CONTROLLER_CONTROLLER_H_

#include <string>
#include <vector>

#include "model/cave.h"

namespace s21 {

class Controller {
 public:
  explicit Controller(s21::Cave *cave) : cave_(cave) {}
  ~Controller() = default;

  void SetSettingsForCave(int rows, int columns, int chance);
  void SetLimitsForCave(int birth, int death);

  int GetCaveRows();
  int GetCaveColumns();
  bool ***GetCaveMap();

  void OpenFileCave(const char *path);
  void GenerateCave();
  void StartStepCave();

 private:
  s21::Cave *cave_;
};

}  // namespace s21

#endif  // SRC_CONTROLLER_CONTROLLER_H_
