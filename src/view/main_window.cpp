#include "main_window.h"

#include "ui_main_window.h"

namespace s21 {

MainWindow::MainWindow(s21::Controller *controller, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(controller) {
  ui_->setupUi(this);
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &MainWindow::StepCave);
  connect(ui_->pb_load_file_maze, SIGNAL(clicked()), this,
          SLOT(ClickLoadFileMaze()));
  connect(ui_->pb_generate_maze, SIGNAL(clicked()), this,
          SLOT(ClickGenerateMaze()));
  connect(ui_->pb_save_file_maze, SIGNAL(clicked()), this,
          SLOT(ClickSaveFileMaze()));
  connect(ui_->pb_load_file_cave, SIGNAL(clicked()), this,
          SLOT(ClickLoadFileCave()));
  connect(ui_->pb_generate_cave, SIGNAL(clicked()), this,
          SLOT(ClickGenerateCave()));
  connect(ui_->pb_step_cave, SIGNAL(clicked()), this, SLOT(ClickStepCave()));
  connect(ui_->pb_auto_cave, SIGNAL(clicked()), this,
          SLOT(ClickAutoStepCave()));
}

MainWindow::~MainWindow() {
  delete ui_;
  delete timer_;
}

/* _______ CAVE _______ */

void MainWindow::PaintCave() {
  int get_columns = controller_->GetCaveColumns();
  int get_rows = controller_->GetCaveRows();
  bool ***get_cave = controller_->GetCaveMap();
  ui_->cave_scene->DrawCave(get_cave, get_columns, get_rows);
}

void MainWindow::StepCave() {
  int birth = ui_->spinbox_birth_cave->value();
  int death = ui_->spinbox_death_cave->value();
  controller_->SetLimitsForCave(birth, death);
  controller_->StartStepCave();
  PaintCave();
}

void MainWindow::ClickLoadFileCave() {
  QString path = QFileDialog::getOpenFileName(this, "Выбрать файл",
                                              QDir::currentPath(), "*.txt");
  if (timer_ != nullptr) timer_->stop();
  QFileInfo fileinfo(path);
  QString filename = fileinfo.fileName();
  ui_->filename_cave->setText(filename);
  try {
    controller_->OpenFileCave(path.toStdString().c_str());
  } catch (std::exception &ex) {
    QMessageBox::warning(this, "Ошибка", "Загрузите верный файл!");
  }
  PaintCave();
}

void MainWindow::ClickGenerateCave() {
  if (timer_ != nullptr) timer_->stop();
  int rows = ui_->spinbox_rows_cave->value();
  int columns = ui_->spinbox_columns_cave->value();
  int chance = ui_->spinbox_chance_cave->value();
  controller_->SetSettingsForCave(rows, columns, chance);
  controller_->GenerateCave();
  PaintCave();
}

void MainWindow::ClickStepCave() {
  if (timer_ != nullptr) timer_->stop();
  StepCave();
}

void MainWindow::ClickAutoStepCave() {
  int milliseconds = ui_->spinbox_milliseconds_cave->value();
  timer_->start(milliseconds);
}

/* _______ MAZE _______ */

void MainWindow::ClickLoadFileMaze() {
  QString path = QFileDialog::getOpenFileName(this, "Выбрать файл",
                                              QDir::currentPath(), "*.txt");
  QFileInfo fileinfo(path);
  QString filename = fileinfo.fileName();
  ui_->filename_maze->setText(filename);
  try {
    ui_->maze_scene->DrawMazeFromFile(path);
  } catch (std::exception &ex) {
    QMessageBox::warning(this, "Ошибка", "Загрузите верный файл!");
  }
}

void MainWindow::ClickGenerateMaze() {
  int rows = ui_->spinbox_rows_maze->value();
  int columns = ui_->spinbox_columns_maze->value();
  ui_->maze_scene->DrawMazeGenerate(rows, columns);
}

void MainWindow::ClickSaveFileMaze() {
  QString path = QFileDialog::getSaveFileName(this, "Сохранить файл",
                                              QDir::homePath(), "*.txt");
  try {
    ui_->maze_scene->SaveMazeFile(path);
  } catch (std::exception &ex) {
    QMessageBox::warning(this, "Ошибка", "Ошибка сохранения файла!");
  }
}

}  // namespace s21
