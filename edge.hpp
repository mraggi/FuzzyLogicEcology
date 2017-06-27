#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
const int INVALID_VERTEX = -1;

struct Edge
{
	Edge(int f, int t, double w = 1) : from(f), to(t), weight(w) {}
	
	int from;
	int to;
	double weight;
};

inline std::vector<Edge> FromAdjacencyMatrix(const std::vector<std::vector<double>>& A, double tolerance = 0.001)
{
	std::vector<Edge> result;
	size_t n = A.size();
	for (size_t x = 0; x < n; ++x)
	{
		for (size_t y = 0; y < n; ++y)
		{
			if (x == y)
				continue;
			if (A[x][y] > tolerance)
				result.emplace_back(x,y,A[x][y]);
		}
	}
	return result;
}
