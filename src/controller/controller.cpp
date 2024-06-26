#include "controller.h"

namespace s21 {

void Controller::SetSettingsForCave(int rows, int columns, int chance) {
  cave_->SetWidth(rows);
  cave_->SetHeight(columns);
  cave_->SetChance(chance);
}

void Controller::SetLimitsForCave(int birth, int death) {
  cave_->SetBirthLimit(birth);
  cave_->SetDeathLimit(death);
}

int Controller::GetCaveRows() { return cave_->GetRows(); }

int Controller::GetCaveColumns() { return cave_->GetColumns(); }

bool ***Controller::GetCaveMap() { return cave_->GetCaveMap(); }

void Controller::OpenFileCave(const char *path) { cave_->Init(path); }

void Controller::GenerateCave() { cave_->Init(); }

void Controller::StartStepCave() { cave_->StepRun(); }

}  // namespace s21
