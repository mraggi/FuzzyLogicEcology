#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <array>
#include <string>
#include <queue>
#include <algorithm>
#include <cassert>
#include <unordered_map>

using vertex_t = int;
const vertex_t INVALID_VERTEX = -1;

using weight_t = double;

// something larger than weight_t, for when you have that weight_t doesn't properly hold a sum of weight_t
using sumweight_t = double;
const sumweight_t INF = 9999999999999999;

struct Neighbor
{
	explicit Neighbor() : vertex(INVALID_VERTEX), m_weight(0) {} 
	
	explicit Neighbor(vertex_t v, weight_t w = 1) : vertex(v), m_weight(w) {}
	
	inline operator vertex_t() const
	{
		return vertex;
	}
	
	weight_t weight() const
	{
		return m_weight;
// 		return 1;
	}
	
	void set_weight(weight_t w)
	{
		m_weight = w;
	}
	
	vertex_t vertex;
private:
	weight_t m_weight{1}; //comment if not needed, and make set_weight do nothing, and make weight() return 1
};


struct Edge
{
	Edge() : from(INVALID_VERTEX), to(INVALID_VERTEX), m_weight(0) {}
	Edge(vertex_t f, vertex_t t, weight_t w = 1) : from(f), to(t), m_weight(w) {}
	vertex_t operator[](bool i)
	{
		if (i)
			return to;
		return from;
	}
	
	weight_t weight() const
	{
		return m_weight;
	}
	
	vertex_t from;
	vertex_t to;
	weight_t m_weight;
};


//simple graph
class Graph
{
public:
	explicit Graph(vertex_t numberOfVertices):
			m_n(numberOfVertices),
			m_graph(numberOfVertices)
	{
	}
	
	explicit Graph(const std::vector<std::string>& names):
			m_n(names.size()),
			m_graph(names.size())
	{
		set_names(names);
	}
	
	int degree(vertex_t a) const
	{
		return m_graph[a].size();
	}
	
	// Graph modification functions
	void add_edge(vertex_t from, vertex_t to, weight_t w = 1)
	{
		m_graph[from].emplace_back(to,w);
		m_graph[to].emplace_back(from,w);
		m_neighbors_sorted = false;
	}
	
	void add_edge(const std::string& from, const std::string& to, weight_t w = 1)
	{
		vertex_t a = get_vertex_by_name(from);
		vertex_t b = get_vertex_by_name(to);
		add_edge(a,b,w);
	}
	
	// Get Graph Info
	vertex_t num_vertices() const { return m_n; }
	size_t num_edges() const
	{
		size_t total = 0;
		for (vertex_t u = 0; u < m_n; ++u)
		{
			total += degree(u);
		}
		return total/2;
	}
	
	std::vector<Edge> edges() const
	{
		std::vector<Edge> total;
		for (vertex_t u = 0; u < m_n; ++u)
		{
			for (auto v : m_graph[u])
			{
				if (v > u)
					total.emplace_back(u,v,v.weight());
			}
		}
		return total;
	}
	
	Graph induced_subgraph(const std::vector<vertex_t>& Vertices) const
	{
		Graph D(Vertices.size());
		
		if (are_vertices_named())
		{
			std::vector<std::string> new_names(Vertices.size());
			for (vertex_t u = 0; u < Vertices.size(); ++u)
			{
				new_names[u] = get_name(Vertices[u]);
			}
			D.set_names(new_names);
		}
		
		std::vector<vertex_t> inverse_map(m_n,INVALID_VERTEX);
		for (vertex_t u = 0; u < Vertices.size(); ++u)
		{
			inverse_map[Vertices[u]] = u;
		}
		
		for (vertex_t u = 0; u < Vertices.size(); ++u)
		{
			auto v = Vertices[u];
			for (auto v_prime : outneighbors(v))
			{
				auto u_prime = inverse_map[v_prime];
				if (u_prime != INVALID_VERTEX)
					D.add_edge(u,u_prime,v_prime.weight());
			}
		}
		
		return D;
	}
	
	inline const std::vector<Neighbor>& neighbors(vertex_t n) const { return m_graph[n]; }
	inline const std::vector<Neighbor>& outneighbors(vertex_t n) const { return m_graph[n]; }
	inline const std::vector<Neighbor>& inneighbors(vertex_t n) const { return m_graph[n]; }
	
	
	inline const weight_t is_neighbor(vertex_t from, vertex_t to) const
	{
		if (degree(from) > degree(to))
			std::swap(from,to);
		if (m_neighbors_sorted)
		{
			return std::binary_search(neighbors(from).begin(), neighbors(from).end(),to);
		} else
		{
			for (auto& a : neighbors(from))
			{
				if (a == to)
					return true;
			}
			return false;
		}
	}	
	
	// Very slow!
	inline const weight_t edge_value(vertex_t from, vertex_t to) const 
	{
		if (degree(from) > degree(to))
			std::swap(from,to);
		if (m_neighbors_sorted)
		{
			auto it = std::partition_point(neighbors(from).begin(), neighbors(from).end(), [to](const vertex_t& a)
			{
				return vertex_t(a) < to;
			});
			if (it == neighbors(from).end())
				return 0;
			if (*it == to)
				return it->weight();
			return 0;
		} else
		{
			for (auto& a : neighbors(from))
			{
				if (a == to)
					return a.weight();
			}
			return 0;
		}
	}
	
	Graph& operator+=(const Graph& G)
	{
		auto oldn = m_n;
		auto newn = oldn+G.num_vertices();
		m_graph.resize(newn);
		
		for (vertex_t u = 0; u < G.num_vertices(); ++u)
		{
			for (auto v : G.neighbors(u))
				add_edge(u+oldn,v+oldn,v.weight());
		}
		m_n = newn;
		return *this;
	}
	
	void sort_neighbors()
	{
		if (m_neighbors_sorted)
			return;
		for (vertex_t v = 0; v < m_n; ++v)
		{
			sort(m_graph[v].begin(), m_graph[v].end());
		}
		m_neighbors_sorted = true;
	}
	
	size_t get_neighbor_index(vertex_t from, vertex_t to) const
	{
		if (m_neighbors_sorted)
		{
			auto it = std::partition_point(neighbors(from).begin(), neighbors(from).end(), [to](const vertex_t& a)
			{
				return a < to;
			});
			return it - neighbors(from).begin();
		} else
		{
			for (size_t i = 0; i < neighbors(from).size(); ++i)
			{
				if (m_graph[from][i] == to)
					return i;
			}
		}
		return m_graph[from].size();
	}
	
	void change_m_weight(vertex_t vertex, size_t index, weight_t newm_weight)
	{
		m_graph[vertex][index].set_weight(newm_weight);
	}
	
	
	static Graph ReadFromstdin(int n, int m);
	static Graph ReadWeightedFromstdin(int n, int m);
	
	// Related to names

	bool are_vertices_named() const { return !m_vertex_names.empty(); }
	
	void set_names(const std::vector<std::string>& names)
	{
		if (are_vertices_named())
			m_name_map.clear();
		m_vertex_names = names;
		for (size_t i = 0; i < names.size(); ++i)
		{
			m_name_map[names[i]] = i;
		}
	}
	
	vertex_t get_vertex_by_name(const std::string& name) const
	{
		assert(!are_vertices_named());
		auto it = m_name_map.find(name);
		assert(it != m_name_map.end());
		return it->second;
	}
	
	const std::string& get_name(vertex_t a) const
	{
		return m_vertex_names[a];
	}
	
	void change_name(vertex_t a, const std::string& new_name)
	{
		auto old_name = get_name(a);
		m_vertex_names[a] = new_name;
		auto it = m_name_map.find(old_name);
		m_name_map.erase(it);
		m_name_map[new_name] = a;
	}
	
private:
	// Graph insides
	vertex_t m_n;
	std::vector<std::vector<Neighbor>> m_graph;
	bool m_neighbors_sorted {false};
	
	std::vector<std::string> m_vertex_names;
	std::unordered_map<std::string, vertex_t> m_name_map;
	
};

class DiGraph
{
public:
	explicit DiGraph(vertex_t numberOfVertices = 0):
			m_n(numberOfVertices),
			m_outgraph(numberOfVertices),
			m_ingraph(numberOfVertices)
	{
	}
	
	static DiGraph FromAdjacencyMatrix(const std::vector<std::vector<double>>& A, double tolerance = 0.001)
	{
		auto n = A.size();
		DiGraph D(n);
		for (size_t x = 0; x < n; ++x)
		{
			for (size_t y = 0; y < n; ++y)
			{
				if (x == y)
					continue;
				double weight = A[x][y];
				if (weight > tolerance)
					D.add_edge(x,y,weight);
			}
		}
		return D;
	}
	
	size_t outdegree(vertex_t a) const
	{
		return m_outgraph[a].size();
	}
	
	size_t indegree(vertex_t a) const
	{
		return m_ingraph[a].size();
	}
	
	// DiGraph modification functions
	void add_edge(vertex_t from, vertex_t to, weight_t w = 1)
	{
		m_outgraph[from].emplace_back(to,w);
		m_ingraph[to].emplace_back(from,w);
		m_neighbors_sorted = false;
	}
	
	void add_edge(const std::string& from, const std::string& to, weight_t w = 1)
	{
		vertex_t a = get_vertex_by_name(from);
		vertex_t b = get_vertex_by_name(to);
		add_edge(a,b,w);
	}
	
	
	// Get DiGraph Info
	vertex_t num_vertices() const { return m_n; }
	size_t num_edges() const
	{
		size_t total = 0;
		for (vertex_t u = 0; u < m_n; ++u)
		{
			total += outdegree(u);
		}
		return total;
	}
	
	std::vector<Edge> edges() const
	{
		std::vector<Edge> total;
		for (vertex_t u = 0; u < m_n; ++u)
		{
			for (auto v : m_outgraph[u])
			{
				total.emplace_back(u,v,v.weight());
			}
		}
		return total;
	}
	
	inline const std::vector<Neighbor>& outneighbors(vertex_t n) const { return m_outgraph[n]; }
	inline const std::vector<Neighbor>& inneighbors(vertex_t n) const { return m_ingraph[n]; }
	inline const weight_t edge_value(vertex_t from, vertex_t to) const
	{
		return edge_value(from,to,std::less<vertex_t>());
	}
	
	template<class Compare>
	inline const weight_t edge_value(vertex_t from, vertex_t to, Compare outcomp) const 
	{
		auto it = std::partition_point(outneighbors(from).begin(), outneighbors(from).end(), [to,outcomp](const vertex_t& a)
		{
			return outcomp(a, to);
		});
		if (it == outneighbors(from).end())
			return 0;
		if (*it == to)
			return it->weight();
		return 0;
	}
	
	void sort_neighbors()
	{
		if (m_neighbors_sorted)
			return;
		for (vertex_t v = 0; v < m_n; ++v)
		{
			std::sort(m_outgraph[v].begin(), m_outgraph[v].end());
			std::sort(m_ingraph[v].begin(), m_ingraph[v].end());
		}
		m_neighbors_sorted = true;
	}
	
	//Name stuff
	
	bool are_vertices_named() const { return !m_vertex_names.empty(); }
	
	void set_names(const std::vector<std::string>& names)
	{
		if (are_vertices_named())
			m_name_map.clear();
		m_vertex_names = names;
		for (size_t i = 0; i < names.size(); ++i)
		{
			m_name_map[names[i]] = i;
		}
	}
	
	vertex_t get_vertex_by_name(const std::string& name) const
	{
		assert(!are_vertices_named());
		auto it = m_name_map.find(name);
		assert(it != m_name_map.end());
		return it->second;
	}
	
	const std::string& get_name(vertex_t a) const
	{
		return m_vertex_names[a];
	}
	
	void change_name(vertex_t a, const std::string& new_name)
	{
		auto old_name = get_name(a);
		m_vertex_names[a] = new_name;
		auto it = m_name_map.find(old_name);
		m_name_map.erase(it);
		m_name_map[new_name] = a;
	}
	
private:
	vertex_t m_n;
	std::vector<std::vector<Neighbor>> m_outgraph;
	std::vector<std::vector<Neighbor>> m_ingraph;
	bool m_neighbors_sorted {false};
	
	std::vector<std::string> m_vertex_names;
	std::unordered_map<std::string, vertex_t> m_name_map;
};
