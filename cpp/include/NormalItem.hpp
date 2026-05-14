#pragma once
#include "GildedRoseItem.hpp"

class NormalItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {
    if (item_.quality > MIN) {
      item_.quality--;
    }
    if (item_.sellIn < 1 && item_.quality > MIN) {
      item_.quality--;
    }
  }
};