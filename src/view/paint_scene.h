#ifndef SRC_VIEW_PAINTSCENE_H_
#define SRC_VIEW_PAINTSCENE_H_

#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <iostream>

#include "model/maze.h"

namespace s21 {

class PaintScene : public QWidget {
  Q_OBJECT

 public:
  explicit PaintScene(QWidget *parent = nullptr);

  void DrawCave(bool ***cave, int columns, int rows);
  void DrawMazeGenerate(int rows, int columns);
  void DrawMazeFromFile(QString filename);
  void SaveMazeFile(QString filename);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

 private:
  s21::MazeGen maze_;
  QImage map_maze_;
  QImage map_cave_;
  QImage pacman_;
  QPoint start_point_;
  QPoint finish_point_;
  double width_;
  double height_;
  bool begin_click_;
  bool end_click_;

  std::vector<s21::position> path_;
  position start_;
  position finish_;

  void DrawMaze();
  void DrawPacman();
  void ClearImage(QImage &image);
  void ClearScene();
  void ClearPacman();
};

}  // namespace s21

#endif  // SRC_VIEW_PAINTSCENE_H_
