#pragma once

#include "FuzzyNetworkBase.hpp"
#include "Graph.hpp"
#include "PointGraphConverter.hpp"

class FuzzyNetworkPromiscuity : public FuzzyNetworkBase<Matrix>
{
public:
	FuzzyNetworkPromiscuity(size_t _grid, 
						 const std::vector<std::vector<Point>>& U, 
						 size_t memoryAvailable);
	
	
	void PreInitialize() override;
	void PostInitialize() override;
	

	Point CalculateBorder() const override;
	
	void Realize(Matrix& A, long row, long block) override;

	void ResetFullMatrix(Matrix& A) override
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

