#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "Point.hpp"

std::vector<std::vector<std::string>> ReadTable(std::istream& is);

std::unordered_map<std::string,std::vector<Point>> ExtractLocations(const std::vector<std::vector<std::string>>& U, const std::vector<int>& names, int longitude = -1, int latitude = -1);