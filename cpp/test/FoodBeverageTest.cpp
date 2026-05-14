#define APPROVALS_GOOGLETEST
#include "ApprovalTest.cpp"
#include "ApprovalTests.hpp"
#include "GildedRose.h"
#include <gtest/gtest.h>

// F&B 테스트(FoodBeverageTest.cpp)
// TC-11: F&B 정상 동작
TEST(FoodBeverageTest, DegradesTwiceAsNormal) {
  std::vector<Item> items = {Item("[F&B] Bread", 5, 20)};
  GildedRose app(items);
  app.updateQuality();
  // EXPECT_EQ(4, items[0].sellIn);
  // EXPECT_EQ(18, items[0].quality); // 20 -2 = 18

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-12: F&B 기한 지났을 때 동작
TEST(FoodBeverageTest, DegradesFourTimesAfterSellIn) {
  std::vector<Item> items = {Item("[F&B] Milk", 0, 20)};
  GildedRose app(items);
  app.updateQuality();
  // EXPECT_EQ(16, items[0].quality); // 20 -4 = 16

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-13: F&B Quality 품질 하한 확인
TEST(FoodBeverageTest, QualityNeverBelowZero) {
  std::vector<Item> items = {Item("[F&B] Water", 0, 1)};
  GildedRose app(items);
  app.updateQuality();
  // EXPECT_EQ(0, items[0].quality); // 0 유지

  ApprovalTests::Approvals::verify(app.items[0].toString());
}
