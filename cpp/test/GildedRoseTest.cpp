#include "GildedRose.h"
#include <gtest/gtest.h>

// TEST(GildedRoseTest, Foo) {
//   std::vector<Item> items = {Item("foo", 0, 0)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ("foo", app.items[0].name);
// }

// // TC-01: 일반 아이템 품질 하한 확인
// TEST(GildedRoseTest, NormalItemQualityLowerLimit) {
//   std::vector<Item> items = {Item("noname", 0, 0)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(-1, app.items[0].sellIn);
//   EXPECT_EQ(0, app.items[0].quality);
// }

// // TC-02: 기한 지난 일반 아이템
// TEST(GildedRoseTest, OutOfDateNormalItemQualityDisplacement) {
//   std::vector<Item> items = {Item("noname", 0, 5)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(-1, app.items[0].sellIn);
//   EXPECT_EQ(3, app.items[0].quality);
// }

// // TC-03: 전설 아이템은 변하지 않음
// TEST(GildedRoseTest, FixedQualityOfSulfurasItem) {
//   std::vector<Item> items = {Item("Sulfuras", 0, 5)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(0, app.items[0].sellIn);
//   EXPECT_EQ(5, app.items[0].quality);
// }

// // TC-04: 기한 마감 후에도 변하진 않음
// TEST(GildedRoseTest, FixedQualityOfOutOfDateSulfurasItem) {
//   std::vector<Item> items = {Item("Sulfuras", -1, 5)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(-1, app.items[0].sellIn);
//   EXPECT_EQ(5, app.items[0].quality);
// }

// // TC-05: 기한 지난 Aged Brie는 품질 +2
// TEST(GildedRoseTest, QualityUp2WhenOutOfDateAgedBrieItem) {
//   std::vector<Item> items = {Item("Aged Brie", 0, 0)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(-1, app.items[0].sellIn);
//   EXPECT_EQ(2, app.items[0].quality);
// }

// // TC-06: 품질 상한 테스트
// TEST(GildedRoseTest, QualityUpperLimit) {
//   std::vector<Item> items = {Item("Aged Brie", 0, 50)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(-1, app.items[0].sellIn);
//   EXPECT_EQ(50, app.items[0].quality);
// }

// // TC-07: Backstage: 10일 초과 시 +1
// TEST(GildedRoseTest, QualityUpWhenBackstageAndSellinOver10) {
//   std::vector<Item> items = {
//       Item("Backstage passes to a TAFKAL80ETC concert", 15, 0)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(14, app.items[0].sellIn);
//   EXPECT_EQ(1, app.items[0].quality);
// }

// // TC-08: 공연 지난 후 품질 0
// TEST(GildedRoseTest, QualityInitialAfterBackstage) {
//   std::vector<Item> items = {
//       Item("Backstage passes to a TAFKAL80ETC concert", 0, 0)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(-1, app.items[0].sellIn);
//   EXPECT_EQ(0, app.items[0].quality);
// }

// // TC-09: 품질 50 초과 후 공연 종료
// TEST(GildedRoseTest, EndBackStageOverQuality50) {
//   std::vector<Item> items = {
//       Item("Backstage passes to a TAFKAL80ETC concert", 0, 51)};
//   GildedRose app(items);
//   app.updateQuality();
//   EXPECT_EQ(-1, app.items[0].sellIn);
//   EXPECT_EQ(0, app.items[0].quality);
// }

// TC-10: 아이템이 없을 경우 처리
TEST(GildedRoseTest, EmptyItem) {
  std::vector<Item> items;
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(0, app.items.size());
}