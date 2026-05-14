#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (size_t i = 0; i < items.size(); i++) {
    Item &item = items[i];

    if (item.name == SULFURAS) {
      updateSulfuras(item);
    } else if (item.name == AGED_BRIE) {
      updateAgedBrie(item);
    } else if (item.name == BACKSTAGE_PASS) {
      updateBackStagePass(item);
    } else {
      updateNormalItem(item);
    }

    updateSellIn(item);
  }
}

void GildedRose::updateSellIn(Item &item) {
  if (item.name != SULFURAS) {
    item.sellIn = item.sellIn - 1;
  }
}

void GildedRose::updateNormalItem(Item &item) {}

void GildedRose::updateBackStagePass(Item &item) {}

void GildedRose::updateSulfuras(Item &item) {}

void GildedRose::updateAgedBrie(Item &item) {}