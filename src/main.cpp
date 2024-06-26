#include <QApplication>

#include "view/main_window.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Cave cave;
  s21::Controller controller(&cave);
  s21::MainWindow window(&controller);
  window.show();

  return a.exec();
}
