#pragma once

#include "FuzzyNetworkExpDecay.hpp"

class FuzzyNetworkMin : public FuzzyNetworkExpDecay<MatrixFM, FuzzyNetworkMin>
{
public:
    using BaseED = FuzzyNetworkExpDecay<MatrixFM, FuzzyNetworkMin>;
	FuzzyNetworkMin(size_t _grid, const std::vector<std::vector<Point>>& U, size_t memoryAvailable, double VisibilityRangeInKm)  :
		BaseED(_grid, U, memoryAvailable, VisibilityRangeInKm)
	{ }

	void ResetFullMatrix(MatrixFM& A) const
	{
		A = MatrixFM::Constant(A.rows(), A.cols(), scalar_min_t(0.0));
	}

	void UpdateFunction(double& a, const Point& p, long x, long y, int  species)
	{
		double XX = (p.x - x) * (p.x - x);
		double YY = (p.y - y) * (p.y - y);
		a = std::max(double(a), exp(-Cmx * XX - Cmy * YY));
	}
	
	void PostRealizeAction(MatrixFM& A, long species)
	{
        
	}


};
