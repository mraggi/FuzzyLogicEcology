#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "Point.hpp"

// Input Format: 

// First line: the number of species n

// After that, every species is defined as:
// 		string: name of species
// 		int: # of points m_i in the species i, 
//		(m_i lines, each with two space separated doubles: ) containing x and y coords

vector<Point> ReadPointsFromSTDin(size_t n);

vector<vector<string>> ReadTableFromSTDIN(const string& filename);

unordered_map<string,vector<Point>> ExtractLocations(const vector<vector<string>>& U, const vector<int>& names, int longitude = -1, int latitude = -1);

unordered_map<string,vector<Point>> QuercusExtractLocations(const vector<vector<string>>& U);
unordered_map<string,vector<Point>> LobataeExtractLocations(const vector<vector<string>>& U);
unordered_map<string,vector<Point>> CentroExtractLocations(const vector<vector<string>>& U);

