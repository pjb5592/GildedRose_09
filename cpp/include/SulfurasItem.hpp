#pragma once
#include "GildedRoseItem.hpp"

class SulfurasItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {}
};