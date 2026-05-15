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

  if (item.getName().find(SULFURAS) != std::string::npos) {
    gi = std::make_unique<SulfurasItem>(item);
  } else if (item.getName().find(AGED_BRIE) != std::string::npos) {
    gi = std::make_unique<AgedBrieItem>(item);
  } else if (item.getName().find(BACKSTAGE_PASS) != std::string::npos) {
    gi = std::make_unique<BackStagePassItem>(item);
  } else if (item.getName().find(FOODBEVERAGE) != std::string::npos) {
    gi = std::make_unique<FoodBeverageItem>(item);
  } else {
    gi = std::make_unique<NormalItem>(item);
  }
  return gi;
}

void GildedRose::updateQuality() {
  for (auto &item : items) {
    createItem(item)->updateQuality();
    updateSellIn(item);
  }
}

void GildedRose::updateSellIn(Item &item) {
  if (item.getName() != SULFURAS) {
    item.decreaseSellIn();
  }
}
