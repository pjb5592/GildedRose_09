#pragma once
#include "GildedRoseItem.hpp"

class FoodBeverageItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    // 일반의 2배 감소
    if (item_.getQuality() > MIN) {
      item_.downGrade();
      if (item_.getQuality() > MIN)
        item_.downGrade(); // 2배
    }
    // sellIn 지나면 추가 2배 (총 4배)
    if (item_.getSellIn() < 1) {
      if (item_.getQuality() > MIN) {
        item_.downGrade();
        if (item_.getQuality() > MIN)
          item_.downGrade();
      }
    }
  }
};