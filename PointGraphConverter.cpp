
#include "PointGraphConverter.hpp"

Graph CreateGraphFromPoints(const std::vector<Point>& P)
{
	vertex_t n = P.size();
	Graph G(n);
	
	for (vertex_t i = 0; i < n; ++i)
	{
		for (vertex_t j = i+1; j < n; ++j)
		{
			G.add_edge(i,j,distance(P[i],P[j]));
		}
	}
	
	return G;
}
