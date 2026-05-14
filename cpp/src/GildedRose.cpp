#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (size_t i = 0; i < items.size(); i++) {
    Item &item = items[i];

    if (item.name == SULFURAS) {
      continue;
    } else if (item.name == AGED_BRIE) {
      if (item.quality < MAX_QUALITY) {
        item.quality = item.quality + 1;
      }
    } else if (item.name == BACKSTAGE_PASS) {
      if (item.quality < MAX_QUALITY) {
        item.quality = item.quality + 1;
      }
      if (items[i].sellIn < 11) {
        if (items[i].quality < 50) {
          items[i].quality = items[i].quality + 1;
        }
      }

      if (items[i].sellIn < 6) {
        if (items[i].quality < 50) {
          items[i].quality = items[i].quality + 1;
        }
      }
    } else {
      if (item.quality > MIN_QUALITY) {
        item.quality = item.quality - 1;
      }
    }

    item.sellIn = item.sellIn - 1;

    if (item.sellIn < 0) {
      if (item.name == AGED_BRIE) {
        if (item.quality < MAX_QUALITY) {
          item.quality = item.quality + 1;
        }
      } else if (item.name == BACKSTAGE_PASS) {
        item.quality = 0;
      } else {
        if (item.quality > MIN_QUALITY) {
          item.quality = item.quality - 1;
        }
      }
    }
  }
}
