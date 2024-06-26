#include "paint_scene.h"

namespace s21 {

PaintScene::PaintScene(QWidget *parent)
    : QWidget{parent},
      map_maze_(QSize(500, 500), QImage::Format_ARGB32),
      map_cave_(QSize(500, 500), QImage::Format_ARGB32),
      pacman_(QSize(500, 500), QImage::Format_ARGB32) {
  ClearScene();
  begin_click_ = false;
  end_click_ = false;
}

/* _______ DRAW MAIN RECTANGLE IMAGE _______ */

void PaintScene::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setPen(QPen(Qt::darkGray, 2));
  painter.drawRect(1, 1, 498, 498);
  QRect rect = event->rect();
  painter.drawImage(rect, map_maze_, rect);
  painter.drawImage(rect, map_cave_, rect);
  painter.drawImage(rect, pacman_, rect);
  this->update();
}

/* _______ CLEAR _______ */

void PaintScene::ClearImage(QImage &image) { image.fill(QColor(0, 0, 0, 0)); }

void PaintScene::ClearScene() {
  ClearImage(map_maze_);
  ClearImage(map_cave_);
  ClearImage(pacman_);
}

void PaintScene::ClearPacman() { ClearImage(pacman_); }

/* _______ CAVE _______ */

void PaintScene::DrawCave(bool ***cave, int columns, int rows) {
  ClearScene();
  QPainter painter(&map_cave_);
  painter.setPen(QPen(Qt::darkGray, 1));
  painter.drawRect(0, 0, 499, 499);
  painter.setBrush(QBrush(Qt::darkGray));
  width_ = 500.0 / columns;
  height_ = 500.0 / rows;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      double x = width_ * j;
      double y = height_ * i;
      if ((*cave)[i][j]) {
        painter.drawRect(x, y, width_, height_);
      }
    }
  }

  update();
}

/* _______ MAZE _______ */

void PaintScene::DrawMazeGenerate(int rows, int columns) {
  ClearScene();
  maze_.SetSize(rows, columns);
  maze_.GenerateMaze();
  DrawMaze();
}

void PaintScene::DrawMazeFromFile(QString filename) {
  ClearScene();
  maze_.FromFile(filename.toStdString());
  DrawMaze();
}

void PaintScene::SaveMazeFile(QString filename) {
  maze_.SaveFile(filename.toStdString());
}

void PaintScene::DrawMaze() {
  int rows = maze_.GetRows();
  int columns = maze_.GetCols();
  Maze horizontal_walls = maze_.GetHorWalls();
  Maze vertical_walls = maze_.GetVertWalls();
  QPainter painter(&map_maze_);
  painter.setPen(QPen(Qt::darkGray, 2));
  painter.drawLine(0, 1, 499, 1);
  painter.drawLine(1, 499, 1, 0);
  width_ = 500.0 / columns;
  height_ = 500.0 / rows;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      int x0 = width_ * j;
      int y0 = height_ * i;
      int x1 = width_ * (j + 1);
      int y1 = height_ * (i + 1);
      if (vertical_walls(i, j)) {
        painter.drawLine(x1, y0, x1, y1);
      }
      if (horizontal_walls(i, j)) {
        painter.drawLine(x0, y1, x1, y1);
      }
    }
  }
  update();
}

void PaintScene::DrawPacman() {
  QPainter painter(&pacman_);
  painter.setPen(QPen(QColor(255, 229, 31), 2));
  start_.first = start_point_.x() / width_;
  start_.second = start_point_.y() / height_;
  finish_.first = finish_point_.x() / width_;
  finish_.second = finish_point_.y() / height_;
  path_ = maze_.FindWay(start_, finish_);
  for (int i = 0; i < path_.size() - 1; i++) {
    int startx = path_[i].first * width_ + width_ / 2;
    int srarty = path_[i].second * height_ + height_ / 2;
    int finishx = path_[i + 1].first * width_ + width_ / 2;
    int finishy = path_[i + 1].second * height_ + height_ / 2;
    painter.drawLine(startx, srarty, finishx, finishy);
  }
  update();
}

void PaintScene::mousePressEvent(QMouseEvent *event) {
  ClearPacman();

  if (event->button() == Qt::LeftButton) {
    start_point_ = event->pos();
    begin_click_ = true;
  } else if (event->button() == Qt::RightButton) {
    finish_point_ = event->pos();
    end_click_ = true;
  }
  if (begin_click_ && end_click_) {
    DrawPacman();
    begin_click_ = false;
    end_click_ = false;
  }
  update();
}

}  // namespace s21
