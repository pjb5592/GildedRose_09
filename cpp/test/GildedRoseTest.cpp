#include "GildedRose.h"
#include <gtest/gtest.h>

TEST(GildedRoseTest, Foo) {
  std::vector<Item> items = {Item("foo", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ("foo", app.items[0].name);
}

// 일반 아이템 품질 하한 확인
TEST(GildedRoseTest, CheckNormalItemQualityLowerLimit) {
  std::vector<Item> items = {Item("noname", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(0, app.items[0].quality);
}