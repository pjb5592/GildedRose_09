#pragma once
#include "GildedRoseItem.hpp"

class FoodBeverageItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    // 일반의 2배 감소
    if (item_.quality > MIN) {
      item_.quality--;
      if (item_.quality > MIN)
        item_.quality--; // 2배
    }
    // sellIn 지나면 추가 2배 (총 4배)
    if (item_.sellIn < 1) {
      if (item_.quality > MIN) {
        item_.quality--;
        if (item_.quality > MIN)
          item_.quality--;
      }
    }
  }
}