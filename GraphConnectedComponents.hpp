#pragma once
#include "Graph.hpp"
#include "Utility.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>


void DFSConnectedComponentUtil(const Graph& G, std::vector<int>& coloring, vertex_t v, int color);
int num_connected_components(const Graph& G);
std::vector<int> connected_components_coloring(const Graph& G);
std::vector<std::vector<vertex_t>> connected_components(const Graph& G);


bool is_connected(const Graph& G);
