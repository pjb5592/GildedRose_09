#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (size_t i = 0; i < items.size(); i++) {
    Item &item = items[i];

    if (item.name == SULFURAS) {
      continue;
    } else if (item.name == AGED_BRIE) {
      if (item.quality < MAX_QUALITY) {
        item.quality++;
      }
      if (item.sellIn < 1) {
        if (item.quality < MAX_QUALITY) {
          item.quality++;
        }
      }
    } else if (item.name == BACKSTAGE_PASS) {
      if (item.quality < MAX_QUALITY) {
        item.quality++;
      }
      if (item.sellIn < 11) {
        if (item.quality < 50) {
          item.quality++;
        }
      }
      if (item.sellIn < 6) {
        if (item.quality < 50) {
          item.quality++;
        }
      }
      if (item.sellIn < 1) {
        item.quality = 0;
      }
    } else {
      if (item.quality > MIN_QUALITY) {
        item.quality--;
      }
      if (item.sellIn < 1) {

        if (item.quality > MIN_QUALITY) {
          item.quality--;
        }
      }
    }

    item.sellIn = item.sellIn - 1;
  }
}
