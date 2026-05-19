#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "Item.h"

std::vector<Item> createTexttestItems();
void printInventory(std::ostream& out, const std::vector<Item>& items);
std::string runTexttestSimulation(int days);
