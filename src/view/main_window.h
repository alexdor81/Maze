#ifndef SRC_VIEW_MAINWINDOW_H_
#define SRC_VIEW_MAINWINDOW_H_

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QWidget>

#include "controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Controller *controller, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void ClickLoadFileCave();
  void ClickGenerateCave();
  void ClickStepCave();
  void ClickAutoStepCave();
  void ClickLoadFileMaze();
  void ClickGenerateMaze();
  void ClickSaveFileMaze();

 private:
  Ui::MainWindow *ui_;
  s21::Controller *controller_;
  QTimer *timer_;

  void PaintCave();
  void StepCave();
};

}  // namespace s21

#endif  // SRC_VIEW_MAINWINDOW_H_
