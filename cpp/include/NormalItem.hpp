#pragma once
#include "GildedRoseItem.hpp"

class NormalItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {
    if (item_.getQuality() > MIN) {
      item_.downGrade();
    }
    if (item_.getSellIn() < 1 && item_.getQuality() > MIN) {
      item_.downGrade();
    }
  }
};