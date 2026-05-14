#define APPROVALS_GOOGLETEST
#include "GildedRose.h"
#include "ApprovalTest.cpp"
#include "ApprovalTests.hpp"
#include <gtest/gtest.h>

TEST(GildedRoseTest, Foo) {
  std::vector<Item> items = {Item("foo", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-01: 일반 아이템 품질 하한 확인
TEST(GildedRoseTest, NormalItemQualityLowerLimit) {
  std::vector<Item> items = {Item("noname", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-02: 기한 지난 일반 아이템
TEST(GildedRoseTest, OutOfDateNormalItemQualityDisplacement) {
  std::vector<Item> items = {Item("noname", 0, 5)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-03: 전설 아이템은 변하지 않음
TEST(GildedRoseTest, FixedQualityOfSulfurasItem) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", 0, 5)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-04: 기한 마감 후에도 변하진 않음
TEST(GildedRoseTest, FixedQualityOfOutOfDateSulfurasItem) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", -1, 5)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-05: 기한 지난 Aged Brie는 품질 +2
TEST(GildedRoseTest, QualityUp2WhenOutOfDateAgedBrieItem) {
  std::vector<Item> items = {Item("Aged Brie", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-06: 품질 상한 테스트
TEST(GildedRoseTest, QualityUpperLimit) {
  std::vector<Item> items = {Item("Aged Brie", 0, 50)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-07: Backstage: 10일 초과 시 +1
TEST(GildedRoseTest, QualityUpWhenBackstageAndSellinOver10) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 15, 0)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-08: 공연 지난 후 품질 0
TEST(GildedRoseTest, QualityInitialAfterBackstage) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 0)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-09: 품질 50 초과 후 공연 종료
TEST(GildedRoseTest, EndBackStageOverQuality50) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 51)};
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items[0].toString());
}

// TC-10: 아이템이 없을 경우 처리
TEST(GildedRoseTest, EmptyItem) {
  std::vector<Item> items;
  GildedRose app(items);
  app.updateQuality();

  ApprovalTests::Approvals::verify(app.items.size());
}

// F&B 테스트(FoodBeverageTest.cpp)
// TC-11: F&B 정상 동작
TEST(FoodBeverageTest, DegradesTwiceAsNormal) {
  std::vector<Item> items = {Item("[F&B] Bread", 5, 20)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(4, items[0].sellIn);
  EXPECT_EQ(18, items[0].quality); // 20 -2 = 18
}

// TC-12: F&B 기한 지났을 때 동작
TEST(FoodBeverageTest, DegradesFourTimesAfterSellIn) {
  std::vector<Item> items = {Item("[F&B] Milk", 0, 20)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(16, items[0].quality); // 20 -4 = 16
}

// TC-13: F&B Quality 품질 하한 확인
TEST(FoodBeverageTest, QualityNeverBelowZero) {
  std::vector<Item> items = {Item("[F&B] Water", 0, 1)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(0, items[0].quality); // 0 유지
}

- TC -
    11. "[F&B] Bread",
    5, 20 → 4, 18 - TC - 12. "[F&B] Milk", 0, 20 → 16 - TC - 13. "[F&B] Water",
    0, 1 → 0