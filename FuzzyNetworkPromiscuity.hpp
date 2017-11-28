#pragma once

#include "FuzzyNetworkBase.hpp"
#include "Graph.hpp"
#include "PointGraphConverter.hpp"

class FuzzyNetworkPromiscuity : public FuzzyNetworkBase<Matrix, FuzzyNetworkPromiscuity>
{
public:
    using Base = FuzzyNetworkBase<Matrix, FuzzyNetworkPromiscuity>;
	FuzzyNetworkPromiscuity(size_t _grid, 
						 const std::vector<std::vector<Point>>& U, 
						 size_t memoryAvailable) :
			Base(_grid,U,memoryAvailable),
			m_radius(U.size(),0.0),
			m_spanningTree(U.size())
    {}
	
	
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

