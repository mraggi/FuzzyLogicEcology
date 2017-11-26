#pragma once

#include "FuzzyNetworkExpDecay.hpp"

class FuzzyNetworkProduct : public FuzzyNetworkExpDecay<Matrix,FuzzyNetworkProduct>
{
public:
	FuzzyNetworkProduct(size_t _grid, 
						 const std::vector<std::vector<Point>>& U, 
						 size_t memoryAvailable, 
						 double VisibilityRangeInKm) :
			FuzzyNetworkExpDecay<Matrix,FuzzyNetworkProduct>(_grid,U,memoryAvailable,VisibilityRangeInKm)
	{
	}
	
	void ResetFullMatrix(Matrix& A) const
	{
		A = Matrix::Constant(A.rows(), A.cols(), -1.0);
	}
	
	void PostRealizeAction(Matrix& A, long species)
	{
		for (long col = 0; col < A.cols(); ++col)
			A(species,col) += 1.0;
	}

	inline void UpdateFunction(double& a, const Point& p, long x, long y, int species)
	{
		double XX = (p.x-x)*(p.x-x);
		double YY = (p.y-y)*(p.y-y);
		a *= (1.0-exp(-Cmx*XX - Cmy*YY));
	}
	

};
