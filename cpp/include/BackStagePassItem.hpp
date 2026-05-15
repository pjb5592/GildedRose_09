#pragma once
#include "GildedRoseItem.hpp"

class BackStagePassItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {
    if (item_.getQuality() < MAX) {
      item_.upGrade();
    }
    if (item_.getSellIn() < 11 && item_.getQuality() < MAX) {
      item_.upGrade();
    }
    if (item_.getSellIn() < 6 && item_.getQuality() < MAX) {
      item_.upGrade();
    }
    if (item_.getSellIn() < 1) {
      item_.setQuality(0);
    }
  }
};