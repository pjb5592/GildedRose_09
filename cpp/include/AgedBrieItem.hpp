#pragma once
#include "GildedRoseItem.hpp"

class AgedBrieItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    if (item_.quality < MAX) {
      item_.quality++;
      if (item_.sellIn < 1) {
        item_.quality++;
      }
    }
  }
};