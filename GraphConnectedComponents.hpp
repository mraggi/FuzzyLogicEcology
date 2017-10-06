#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <unordered_set>
#include "Utility.hpp"
#include "Graph.hpp"


void DFSConnectedComponentUtil(const Graph& G, std::vector<int>& coloring, vertex_t v, int color);
int num_connected_components(const Graph& G);
std::vector<int> connected_components_coloring(const Graph& G);
std::vector<std::vector<vertex_t>> connected_components(const Graph& G);


bool is_connected(const Graph& G);
