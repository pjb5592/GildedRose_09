#include <gtest/gtest.h>
#include "GildedRose.h"

class GildedRoseItemRulesTest : public ::testing::Test {
protected:
    static constexpr const char* kNormalItem = "Elixir of the Mongoose";
    static constexpr const char* kAgedBrie = "Aged Brie";
    static constexpr const char* kBackstagePass = "Backstage passes to a TAFKAL80ETC concert";
    static constexpr const char* kSulfuras = "Sulfuras, Hand of Ragnaros";
    static constexpr const char* kConjured = "Conjured Mana Cake";
};

TEST_F(GildedRoseItemRulesTest, NormalItemDecreasesSellInAndQualityBeforeSellDate) {
    // Given
    std::vector<Item> items = { Item(kNormalItem, 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(19, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, NormalItemDegradesTwiceAsFastWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(kNormalItem, 0, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(18, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, NormalItemDegradesTwiceAsFastAfterSellDate) {
    // Given
    std::vector<Item> items = { Item(kNormalItem, -1, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-2, app.items[0].sellIn);
    EXPECT_EQ(18, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, NormalItemQualityDoesNotDropBelowZeroAtQualityBoundary) {
    // Given
    std::vector<Item> items = { Item(kNormalItem, 5, 0) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(4, app.items[0].sellIn);
    EXPECT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, NormalItemQualityStopsAtZeroWhenExpiredDecrementIsLargerThanQuality) {
    // Given
    std::vector<Item> items = { Item(kNormalItem, 0, 1) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, AgedBrieIncreasesQualityBeforeSellDate) {
    // Given
    std::vector<Item> items = { Item(kAgedBrie, 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(21, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, AgedBrieIncreasesTwiceAsFastWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(kAgedBrie, 0, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(22, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, AgedBrieIncreasesTwiceAsFastAfterSellDate) {
    // Given
    std::vector<Item> items = { Item(kAgedBrie, -1, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-2, app.items[0].sellIn);
    EXPECT_EQ(22, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, AgedBrieQualityDoesNotExceedFiftyAtQualityBoundary) {
    // Given
    std::vector<Item> items = { Item(kAgedBrie, 5, 50) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(4, app.items[0].sellIn);
    EXPECT_EQ(50, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, AgedBrieExpiredQualityIncreaseIsCappedAtFifty) {
    // Given
    std::vector<Item> items = { Item(kAgedBrie, 0, 49) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(50, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, BackstagePassIncreasesQualityByOneWhenMoreThanTenDaysRemain) {
    // Given
    std::vector<Item> items = { Item(kBackstagePass, 11, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(10, app.items[0].sellIn);
    EXPECT_EQ(21, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, BackstagePassIncreasesQualityByTwoWhenTenDaysRemain) {
    // Given
    std::vector<Item> items = { Item(kBackstagePass, 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(22, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, BackstagePassIncreasesQualityByThreeWhenFiveDaysRemain) {
    // Given
    std::vector<Item> items = { Item(kBackstagePass, 5, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(4, app.items[0].sellIn);
    EXPECT_EQ(23, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, BackstagePassQualityDoesNotExceedFiftyAtQualityBoundary) {
    // Given
    std::vector<Item> items = { Item(kBackstagePass, 5, 49) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(4, app.items[0].sellIn);
    EXPECT_EQ(50, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, BackstagePassQualityDropsToZeroWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(kBackstagePass, 0, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, BackstagePassQualityStaysZeroAfterConcert) {
    // Given
    std::vector<Item> items = { Item(kBackstagePass, -1, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-2, app.items[0].sellIn);
    EXPECT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, SulfurasDoesNotChangeBeforeSellDate) {
    // Given
    std::vector<Item> items = { Item(kSulfuras, 10, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(10, app.items[0].sellIn);
    EXPECT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, SulfurasDoesNotChangeWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(kSulfuras, 0, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(0, app.items[0].sellIn);
    EXPECT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, SulfurasDoesNotChangeAfterSellDate) {
    // Given
    std::vector<Item> items = { Item(kSulfuras, -1, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, SulfurasIsNotCappedAtNormalMaximumQuality) {
    // Given
    std::vector<Item> items = { Item(kSulfuras, 5, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(5, app.items[0].sellIn);
    EXPECT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, SulfurasKeepsNonStandardQualityUnchanged) {
    // Given
    std::vector<Item> items = { Item(kSulfuras, 5, 50) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(5, app.items[0].sellIn);
    EXPECT_EQ(50, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, ConjuredItemDecreasesQualityByTwoBeforeSellDate) {
    // Given
    std::vector<Item> items = { Item(kConjured, 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(18, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, ConjuredItemDecreasesQualityByFourWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(kConjured, 0, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(16, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, ConjuredItemDecreasesQualityByFourAfterSellDate) {
    // Given
    std::vector<Item> items = { Item(kConjured, -1, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-2, app.items[0].sellIn);
    EXPECT_EQ(16, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, ConjuredItemQualityDoesNotDropBelowZeroAtQualityBoundary) {
    // Given
    std::vector<Item> items = { Item(kConjured, 5, 0) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(4, app.items[0].sellIn);
    EXPECT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, ConjuredItemQualityStopsAtZeroWhenExpiredDecrementIsLargerThanQuality) {
    // Given
    std::vector<Item> items = { Item(kConjured, 0, 3) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseItemRulesTest, ConjuredPrefixNameUsesConjuredRules) {
    // Given
    std::vector<Item> items = { Item("Conjured Sulfuras Replica", 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(18, app.items[0].quality);
}
