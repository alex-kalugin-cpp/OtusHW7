/*!
\file
\brief Файл с тестами
Данный файл содержит в себе тестовые примеры

*/

#include "../include/PubSub.h"
#include "gtest/gtest.h"

/// только фиксированный размер блока
TEST(HWexamples, example1) {
  StringPrint sprinter;
  {
    CommandSaver com_sav(3);
    com_sav.add(sprinter);

    com_sav.newCmd("cmd1");
    com_sav.newCmd("cmd2");
    com_sav.newCmd("cmd3");
    com_sav.newCmd("cmd4");
    com_sav.newCmd("cmd5");
  }
  EXPECT_EQ(sprinter.get().size(), 2);
  EXPECT_EQ(sprinter.get().at(0), "bulk: cmd1, cmd2, cmd3");
  EXPECT_EQ(sprinter.get().at(1), "bulk: cmd4, cmd5");
}

/// только фиксированный размер блока + динамический
TEST(HWexamples, example2) {
  StringPrint sprinter;
  {
    CommandSaver com_sav(3);
    com_sav.add(sprinter);

    com_sav.newCmd("cmd1");
    com_sav.newCmd("cmd2");
    com_sav.newCmd("cmd3");
    com_sav.newCmd("{");
    com_sav.newCmd("cmd4");
    com_sav.newCmd("cmd5");
    com_sav.newCmd("cmd6");
    com_sav.newCmd("cmd7");
    com_sav.newCmd("}");
  }
  EXPECT_EQ(sprinter.get().size(), 2);
  EXPECT_EQ(sprinter.get().at(0), "bulk: cmd1, cmd2, cmd3");
  EXPECT_EQ(sprinter.get().at(1), "bulk: cmd4, cmd5, cmd6, cmd7");
}

/// только динамический размер блока с вложенным
TEST(HWexamples, example3) {
  StringPrint sprinter;
  {
    CommandSaver com_sav(3);
    com_sav.add(sprinter);

    com_sav.newCmd("{");
    com_sav.newCmd("cmd1");
    com_sav.newCmd("cmd2");
    com_sav.newCmd("{");
    com_sav.newCmd("cmd3");
    com_sav.newCmd("cmd4");
    com_sav.newCmd("}");
    com_sav.newCmd("cmd5");
    com_sav.newCmd("cmd6");
    com_sav.newCmd("}");
  }
  EXPECT_EQ(sprinter.get().size(), 1);
  EXPECT_EQ(sprinter.get().at(0), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6");
}

/// обрыв динамического блока
TEST(HWexamples, example4) {
  StringPrint sprinter;
  {
    CommandSaver com_sav(3);
    com_sav.add(sprinter);

    com_sav.newCmd("cmd1");
    com_sav.newCmd("cmd2");
    com_sav.newCmd("cmd3");
    com_sav.newCmd("{");
    com_sav.newCmd("cmd4");
    com_sav.newCmd("cmd5");
    com_sav.newCmd("cmd6");
    com_sav.newCmd("cmd7");
  }
  EXPECT_EQ(sprinter.get().size(), 1);
  EXPECT_EQ(sprinter.get().at(0), "bulk: cmd1, cmd2, cmd3");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
