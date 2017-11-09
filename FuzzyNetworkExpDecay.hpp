#pragma once

#include "FuzzyNetworkBase.hpp"

template <class Mat>
class FuzzyNetworkExpDecay : public FuzzyNetworkBase<Mat>
{
public:
	FuzzyNetworkExpDecay(size_t _grid, 
							const std::vector< std::vector< Point > >& U, 
							size_t memoryAvailable, 
							double VisibilityRangeInKm);
	
	
	
	Point CalculateBorder() const override;

	void PreInitialize() override;
	void PostInitialize() override;
	
	void Realize(Mat& A, long species, long block) override;

	virtual void UpdateFunction(double& a, const Point& p, long x, long y, int species) = 0;
	virtual void PostRealizeAction(Mat&  /*A*/, long  /*species*/) {}

protected:
	double Cx {1}; //in continuum
	double Cy {1};
	
	double Cnx {1}; // in normalized continuum
	double Cny {1};
	
	double Cmx {1}; //in grid
	double Cmy {1};
	double sigma;
};

template <class Mat>
FuzzyNetworkExpDecay<Mat>::FuzzyNetworkExpDecay(size_t _grid, 
										   const std::vector<std::vector<Point>>& U, 
										   size_t memoryAvailable, 
										   double VisibilityRangeInKm) 
	:	FuzzyNetworkBase<Mat>(_grid,U,memoryAvailable),
		sigma(VisibilityRangeInKm)
{
	
}

template <class Mat>
void FuzzyNetworkExpDecay<Mat>::PreInitialize()
{
	Cx = 1/(2*sigma*sigma);
	Cy = Cx;
}

template <class Mat>
void FuzzyNetworkExpDecay<Mat>::PostInitialize()
{
	// fucking c++!
	const auto& F = FuzzyNetworkBase<Mat>::F;
	const auto& grid = FuzzyNetworkBase<Mat>::grid;
	
	Cnx = Cx*F.x*F.x;
	Cny = Cy*F.y*F.y;
	
	Cmx = Cnx/(grid*grid);
	Cmy = Cny/(grid*grid);
}

template <class Mat>
Point FuzzyNetworkExpDecay<Mat>::CalculateBorder() const
{
	Point B;
	B.x = MaxNonZeroDistance(Cx);
	B.y = MaxNonZeroDistance(Cy);
	return B;
}


template <class Mat>
void FuzzyNetworkExpDecay<Mat>::Realize(Mat& A, long species, long block)
{
	//fucking c++
	const auto& E = FuzzyNetworkBase<Mat>::E;
	const auto& grid = FuzzyNetworkBase<Mat>::grid;
	const auto& num_cols_per_block = FuzzyNetworkBase<Mat>::num_cols_per_block;
	
	const std::vector<Point>& P = E[species];
	long N = grid;
	
	for (const Point& p : P)
	{
		auto X = FuzzyNetworkBase<Mat>::GetXInterval(p,block, A.cols());
		
		long offset = block*num_cols_per_block;
		
		for (long x = X.L; x < X.R; ++x)
		{
			auto Y = FuzzyNetworkBase<Mat>::GetYInterval(p,block,A.cols(),x);
			
			for (long y = Y.L; y < Y.R; ++y)
			{
				long index = x*N+y - offset;
				
				// Manual inline
				UpdateFunction(A(species,index),p,x,y,species);
			}
		}
	}
	PostRealizeAction(A,species);
}
