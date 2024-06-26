#include <gtest/gtest.h>

#include "../model/cave.h"

TEST(sizeCave, checkSize) {
  s21::Cave cave;
  cave.SetWidth(10);
  cave.SetHeight(15);
  ASSERT_EQ(cave.GetColumns(), 10);
  ASSERT_EQ(cave.GetRows(), 15);
}

TEST(sizeCave, checkLoad) {
  s21::Cave cave;
  cave.Init("test/test_cave_5x10.txt");
  ASSERT_EQ(cave.GetColumns(), 10);
  ASSERT_EQ(cave.GetRows(), 5);
}

TEST(initCave, checkInit) {
  s21::Cave cave;
  cave.SetWidth(10);
  cave.SetHeight(10);
  cave.SetChance(50);
  cave.Init();
  bool*** cave_map = cave.GetCaveMap();
  int points = 0;
  for (int i = 0; i < cave.GetRows(); i++)
    for (int j = 0; j < cave.GetColumns(); j++)
      if ((*cave_map)[i][j]) points++;
  ASSERT_TRUE(points > 40 && points < 60);

  cave.SetChance(75);
  cave.Init();
  points = 0;
  for (int i = 0; i < cave.GetRows(); i++)
    for (int j = 0; j < cave.GetColumns(); j++)
      if ((*cave_map)[i][j]) points++;
  ASSERT_TRUE(points > 65 && points < 85);

  cave.SetChance(25);
  cave.Init();
  points = 0;
  for (int i = 0; i < cave.GetRows(); i++)
    for (int j = 0; j < cave.GetColumns(); j++)
      if ((*cave_map)[i][j]) points++;
  ASSERT_TRUE(points > 15 && points < 35);
}

TEST(cellarCave, checkCellar) {
  s21::Cave cave;
  cave.SetDeathLimit(4);
  cave.SetBirthLimit(4);
  cave.Init("test/test_cave_5x10.txt");
  bool*** cave_map = cave.GetCaveMap();
  cave.StepRun();
  ASSERT_FALSE((*cave_map)[1][1]);
  ASSERT_TRUE((*cave_map)[1][4]);
}
