#pragma once
#include "GildedRoseItem.hpp"

class AgedBrieItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    if (item_.getQuality() < MAX) {
      item_.upGrade();
      if (item_.getSellIn() < 1) {
        item_.upGrade();
      }
    }
  }
};