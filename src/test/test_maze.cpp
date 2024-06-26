#include <gtest/gtest.h>

#include <vector>

#include "../model/maze.h"

TEST(generateMaze, checkSize) {
  s21::MazeGen maze;
  maze.SetSize(10, 10);
  ASSERT_EQ(maze.GetRows(), 10);
  ASSERT_EQ(maze.GetCols(), 10);
}

TEST(generateMaze, checkVerticlal) {
  s21::MazeGen maze;
  maze.SetSize(10, 10);
  maze.GenerateMaze();
  auto matrix = maze.GetVertWalls();
  for (int i = 0; i < maze.GetRows(); i++) {
    ASSERT_EQ(matrix(i, maze.GetCols() - 1), true);
  }
}

TEST(generateMaze, checkHorizontal) {
  s21::MazeGen maze;
  maze.SetSize(10, 10);
  maze.GenerateMaze();
  auto matrix = maze.GetHorWalls();
  for (int i = 0; i < maze.GetCols(); i++) {
    ASSERT_EQ(matrix(maze.GetRows() - 1, i), true);
  }
}

TEST(findWay10x10, checkFindWay) {
  s21::MazeGen maze;
  maze.FromFile("test/test_maze_10x10.txt");
  auto way = maze.FindWay({0, 0}, {5, 5});
  int result[]{5, 5, 5, 4, 6, 4, 6, 3, 5, 3, 4, 3, 4, 4, 3, 4, 3,
               3, 2, 3, 1, 3, 1, 2, 2, 2, 2, 1, 2, 0, 1, 0, 0, 0};
  for (long unsigned int i = 0, pos = 0; i < way.size(); i++) {
    ASSERT_EQ(way[i].first, result[pos++]);
    ASSERT_EQ(way[i].second, result[pos++]);
  }
}
