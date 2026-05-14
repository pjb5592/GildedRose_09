#pragma once
#include <string>

class Item {
private:
  int sellIn_;
  int quality_;
  std::string name_;

public:
  Item(std::string name, int sellIn, int quality)
      : name_(std::move(name)), sellIn_(sellIn), quality_(quality) {}

  std::string toString() const {
    return name_ + ", " + std::to_string(sellIn_) + ", " +
           std::to_string(quality_);
  }

  std::string getName() const { return name_; }
  int getQuality() const { return quality_; }
  void setQuality(int q) { quality_ = q; }
  void downGrade() { quality_--; }
  void upGrade() { quality_++; }
  int getSellIn() const { return sellIn_; }
  void setSellIn(int s) { sellIn_ = s; }
  void decreaseSellIn() { sellIn_--; }
};
