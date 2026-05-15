#pragma once
#include "Item.h"

class GildedRoseItem {
protected:
  Item &item_; // 참조로 보관
public:
  explicit GildedRoseItem(Item &item) : item_(item) {}
  static constexpr int MAX = 50;
  static constexpr int MIN = 0;
  static void updateSellIn(Item &item) {};
  virtual void updateQuality() = 0;
  virtual ~GildedRoseItem() = default;
};
