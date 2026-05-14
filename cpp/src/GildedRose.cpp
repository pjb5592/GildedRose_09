#include "GildedRose.h"
#include "AgedBrieItem.hpp"
#include "BackStagePassItem.hpp"
#include "FoodBeverageItem.hpp"
#include "GildedRoseItem.hpp"
#include "NormalItem.hpp"
#include "SulfurasItem.hpp"
#include "item.h"
#include <memory>

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

std::unique_ptr<GildedRoseItem> GildedRose::createItem(Item &item) {
  std::unique_ptr<GildedRoseItem> gi;

  if (item.name == SULFURAS) {
    gi = std::make_unique<SulfurasItem>(item);
  } else if (item.name == AGED_BRIE) {
    gi = std::make_unique<AgedBrieItem>(item);
  } else if (item.name == BACKSTAGE_PASS) {
    gi = std::make_unique<BackStagePassItem>(item);
  } else if (item.name.substr(0, 5) == FOODANDBEVERAGE) {
    gi = std::make_unique<FoodBeverageItem>(item);
  } else {
    gi = std::make_unique<NormalItem>(item);
  }
  return gi;
}

void GildedRose::updateQuality() {
  for (size_t i = 0; i < items.size(); i++) {
    Item &item = items[i];
    std::unique_ptr<GildedRoseItem> gildedRoseItem = createItem(item);
    gildedRoseItem->updateQuality();
    updateSellIn(item);
  }
}

void GildedRose::updateSellIn(Item &item) {
  if (item.name != SULFURAS) {
    item.sellIn = item.sellIn - 1;
  }
}
