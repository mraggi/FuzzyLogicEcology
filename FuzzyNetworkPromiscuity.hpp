#pragma once

#include "FuzzyNetworkBase.hpp"
#include "Graph.hpp"
#include "PointGraphConverter.hpp"

class FuzzyNetworkPromiscuity : public FuzzyNetworkBase<Matrix, FuzzyNetworkPromiscuity>
{
public:
	FuzzyNetworkPromiscuity(size_t _grid, 
						 const std::vector<std::vector<Point>>& U, 
						 size_t memoryAvailable);
	
	
	void PreInitialize();
	void PostInitialize();
	

	Point CalculateBorder() const;
	
	void Realize(Matrix& A, long species, long block);

	void ResetFullMatrix(Matrix& A) const
	{
		A.setZero();
	}
	
	double get_distancesq_to_spanning_tree(const Point& P, long species) const;
	
	Interval GetXInterval(long species) const;
	Interval GetYInterval(long species) const;
private:
	std::vector<double> m_radius; //in continuum
	using SpanningTree = std::vector<Edge>;
	std::vector<SpanningTree> m_spanningTree; //in continuum
	double max_radius {0};
};

