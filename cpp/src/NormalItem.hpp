#pragma once
#include "item.h"

class NormalItem {
  Item &item_; // 참조로 보관
public:
  explicit NormalItem(Item &item) : item_(item) {}

  void updateQuality() {
    static constexpr int MAX = 50;
    static constexpr int MIN = 0;
    if (item_.quality > MIN) {
      item_.quality--;
    }
    if (item_.sellIn < 1 && item_.quality > MIN) {
      item_.quality--;
    }
  }
};
