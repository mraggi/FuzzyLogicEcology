#pragma once

#include "Graph.hpp"
#include "GraphConnectedComponents.hpp"

inline Graph GetGraphFromDiGraph(const DiGraph& D, double umbral)
{
	Graph G(D.num_vertices());
	
	for (auto e : D.edges())
	{
		if (e.from > e.to)
			continue;
		double weightA = e.weight();
		double weightB = D.edge_value(e.to,e.from);
		double w = weightA + weightB;
		if (w > umbral)
			G.add_edge(e.from,e.to,weightA+weightB);
		
	}
	return G;
}

inline std::vector<std::vector<vertex_t>> ReduceToFindEndemism(const DiGraph& D, double umbral)
{
	Graph G = GetGraphFromDiGraph(D, umbral);
	
	return connected_components(G);	
}