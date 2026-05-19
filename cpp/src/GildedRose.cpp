#include "GildedRose.h"

#include <algorithm>

namespace {
constexpr int kMinimumQuality = 0;
constexpr int kMaximumQuality = 50;
constexpr char kAgedBrie[] = "Aged Brie";
constexpr char kBackstagePass[] = "Backstage passes to a TAFKAL80ETC concert";
constexpr char kSulfuras[] = "Sulfuras, Hand of Ragnaros";
constexpr char kConjuredPrefix[] = "Conjured";

bool isConjured(const Item& item) {
    return item.name.rfind(kConjuredPrefix, 0) == 0;
}

void increaseQuality(Item& item, int amount) {
    item.quality = std::min(kMaximumQuality, item.quality + amount);
}

void decreaseQuality(Item& item, int amount) {
    item.quality = std::max(kMinimumQuality, item.quality - amount);
}
}

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

void GildedRose::updateQuality() {
    for (Item& item : items) {
        if (item.name == kSulfuras) {
            continue;
        }

        const int originalSellIn = item.sellIn;
        item.sellIn -= 1;

        if (item.name == kAgedBrie) {
            increaseQuality(item, originalSellIn <= 0 ? 2 : 1);
        } else if (item.name == kBackstagePass) {
            if (originalSellIn <= 0) {
                item.quality = kMinimumQuality;
            } else if (originalSellIn <= 5) {
                increaseQuality(item, 3);
            } else if (originalSellIn <= 10) {
                increaseQuality(item, 2);
            } else {
                increaseQuality(item, 1);
            }
        } else if (isConjured(item)) {
            decreaseQuality(item, originalSellIn <= 0 ? 4 : 2);
        } else {
            decreaseQuality(item, originalSellIn <= 0 ? 2 : 1);
        }
    }
}
