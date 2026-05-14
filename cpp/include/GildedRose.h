#pragma once
#include "Item.h"
#include <memory>
#include <vector>

class GildedRoseItem;
class AgedBrieItem;
class BackStagePassItem;
class NormalItem;
class SulfurasItem;
class GildedRose {
private:
  static constexpr const char *AGED_BRIE = "Aged Brie";
  static constexpr const char *BACKSTAGE_PASS =
      "Backstage passes to a TAFKAL80ETC concert";
  static constexpr const char *SULFURAS = "Sulfuras, Hand of Ragnaros";
  static constexpr int MAX_QUALITY = 50;
  static constexpr int MIN_QUALITY = 0;

public:
  std::vector<Item> &items;

  std::unique_ptr<GildedRoseItem> createItem(Item &item);
  GildedRose(std::vector<Item> &items);
  void updateQuality();
  void updateSellIn(Item &item);
};
