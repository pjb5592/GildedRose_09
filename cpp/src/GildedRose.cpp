#include "GildedRose.h"
#include <memory>

class GildedRoseItem {
protected:
  Item &item_; // 참조로 보관
public:
  explicit GildedRoseItem(Item &item) : item_(item) {}
  static constexpr int MAX = 50;
  static constexpr int MIN = 0;
  static void updateSellIn(Item &item) {};
  virtual void updateQuality() = 0;
  virtual ~GildedRoseItem() = default;
};

class AgedBrieItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    static constexpr int MAX = 50;
    if (item_.quality < MAX) {
      item_.quality++;
      if (item_.sellIn < 1) {
        item_.quality++;
      }
    }
  }
};

class BackStagePassItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {
    static constexpr int MAX = 50;
    if (item_.quality < MAX) {
      item_.quality++;
    }
    if (item_.sellIn < 11 && item_.quality < MAX) {
      item_.quality++;
    }
    if (item_.sellIn < 6 && item_.quality < MAX) {
      item_.quality++;
    }
    if (item_.sellIn < 1) {
      item_.quality = 0;
    }
  }
};

#pragma once
#include "item.h"

class NormalItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {
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

class SulfurasItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {}
};

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

std::unique_ptr<GildedRoseItem> GildedRose::createItem(Item &item) {
  if (item.name == SULFURAS) {
    return std::make_unique<SulfurasItem>(item);
  } else if (item.name == AGED_BRIE) {
    return std::make_unique<AgedBrieItem>(item);
  } else if (item.name == BACKSTAGE_PASS) {
    return std::make_unique<BackStagePassItem>(item);
  } else {
    return std::make_unique<NormalItem>(item);
  }
}

void GildedRose::updateQuality() {
  for (size_t i = 0; i < items.size(); i++) {
    Item &item = items[i];

    if (item.name == SULFURAS) {
      updateSulfuras(item);
    } else if (item.name == AGED_BRIE) {
      updateAgedBrie(item);
    } else if (item.name == BACKSTAGE_PASS) {
      updateBackStagePass(item);
    } else {
      updateNormalItem(item);
    }

    updateSellIn(item);
  }
}

void GildedRose::updateSellIn(Item &item) {
  if (item.name != SULFURAS) {
    item.sellIn = item.sellIn - 1;
  }
}

void GildedRose::updateNormalItem(Item &item) {}

void GildedRose::updateBackStagePass(Item &item) {}

void GildedRose::updateSulfuras(Item &item) {}

void GildedRose::updateAgedBrie(Item &item) {}
