#include "GraphConnectedComponents.hpp"

void DFSConnectedComponentUtil(const Graph& G, std::vector<int>& coloring, vertex_t v, int color)
{
    coloring[v] = color;
    for (auto u : G.neighbors(v))
    {
        if (coloring[u] == -1)
            DFSConnectedComponentUtil(G,coloring,u,color);
    }
}


int num_connected_components(const Graph& G)
{
    std::vector<int> coloring(G.num_vertices(),-1);
    coloring[0] = 0;
    int color = 0;
    for (vertex_t v = 0; v < G.num_vertices(); ++v)
    {
        if (coloring[v] == -1)
        {
            DFSConnectedComponentUtil(G,coloring, v, color);
            ++color;
        }
    }
    return color;
}

std::vector<int> connected_components_coloring(const Graph& G)
{
    std::vector<int> coloring(G.num_vertices(),-1);
    int color = 0;
    for (vertex_t v = 0; v < G.num_vertices(); ++v)
    {
        if (coloring[v] == -1)
        {
            DFSConnectedComponentUtil(G,coloring, v, color);
            ++color;
        }
    }
    return coloring;
}

std::vector<std::vector<vertex_t>> connected_components(const Graph& G)
{
    auto coloring = connected_components_coloring(G);
    std::vector<std::vector<vertex_t>> cc(*std::max_element(coloring.begin(), coloring.end())+1);
    for (vertex_t v = 0; v < G.num_vertices(); ++v)
    {
        cc[coloring[v]].push_back(v);
    }
    return cc;
}

bool is_connected(const Graph& G)
{
    std::vector<int> coloring(G.num_vertices(),-1);
    coloring[0] = 0;
    DFSConnectedComponentUtil(G,coloring, 0, 0);
    for (vertex_t v = 1; v < G.num_vertices(); ++v)
    {
        if (coloring[v] == -1)
           return false;
    }
    return true;
}