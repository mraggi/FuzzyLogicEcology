#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "Point.hpp"

vector<vector<string>> ReadTable(istream& is);

unordered_map<string,vector<Point>> ExtractLocations(const vector<vector<string>>& U, const vector<int>& names, int longitude = -1, int latitude = -1);

unordered_map<string,vector<Point>> QuercusExtractLocations(const vector<vector<string>>& U);
unordered_map<string,vector<Point>> LobataeExtractLocations(const vector<vector<string>>& U);
unordered_map<string,vector<Point>> CentroExtractLocations(const vector<vector<string>>& U);

