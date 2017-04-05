#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Point.hpp"

// Input Format: 

// First line: the number of species n

// After that, every species is defined as:
// 		string: name of species
// 		int: # of points m_i in the species i, 
//		(m_i lines, each with two space separated doubles: ) containing x and y coords

vector<Point> ReadPointsFromSTDin(size_t n)
{
	vector<Point> P(n);
	
	for (auto& p : P)
	{
		cin >> p.x >> p.y;
	}
	
	return P;
}

struct Species
{
	string name;
	
};

vector<SpecimenData> ReadSpeciesFromStdin()