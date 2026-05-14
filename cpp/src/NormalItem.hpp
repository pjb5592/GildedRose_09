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
