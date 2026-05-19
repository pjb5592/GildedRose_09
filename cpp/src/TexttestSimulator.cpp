#include "TexttestSimulator.h"

#include "GildedRose.h"

#include <sstream>

std::vector<Item> createTexttestItems() {
    std::vector<Item> items;
    items.emplace_back("+5 Dexterity Vest", 10, 20);
    items.emplace_back("Aged Brie", 2, 0);
    items.emplace_back("Elixir of the Mongoose", 5, 7);
    items.emplace_back("Sulfuras, Hand of Ragnaros", 0, 80);
    items.emplace_back("Backstage passes to a TAFKAL80ETC concert", 15, 20);
    return items;
}

void printInventory(std::ostream& out, const std::vector<Item>& items) {
    for (const Item& item : items) {
        out << item.toString() << '\n';
    }
}

std::string runTexttestSimulation(int days) {
    std::ostringstream out;
    std::vector<Item> items = createTexttestItems();
    GildedRose app(items);

    out << "OMGHAI!\n";
    printInventory(out, items);

    for (int day = 1; day <= days; ++day) {
        out << "-------- day " << day << " --------\n";
        app.updateQuality();
        printInventory(out, items);
    }

    return out.str();
}
