#pragma once
#include "item.h"

class SulfurasItem {
  Item &item_; // 참조로 보관
public:
  explicit SulfurasItem(Item &item) : item_(item) {}

  void updateQuality() {}
};
