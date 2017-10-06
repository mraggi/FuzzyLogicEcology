#pragma once

#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Point.hpp"
#include "scalar_min.hpp"


#if USE_BLAZE
	#include <blaze/config/BLAS.h>
	#include <blaze/Blaze.h>
	using MatrixXd = blaze::DynamicMatrix<double>;
#elif USE_EIGEN
	#include <eigen3/Eigen/Dense>
	#if FUZZY_MIN
		using MatrixXd = Eigen::Matrix<scalar_min_t,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>;
	#else
		using MatrixXd = Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>;
	#endif
#endif

class GraphCalculator
{
public:
	GraphCalculator(size_t _grid, double VisibilityRangeInKm, const vector<vector<Point>>& U, size_t memoryAvailable);
	Matrix CalculateGraph();
	
	double GetTotalArea(int species) const;
	
private: //functions
	void Realize(MatrixXd& A, long row, long block);
	void Normalize(vector<vector<Point>>& U);
	void SetBlockSize(long memoryAvailable);
	template<class Mat>
	Matrix DivideByArea(const Mat& M) const;

	inline void UpdateFunction(const Point& p, const long& x, const long& y, double& a, int i = -1)
    {
        double XX = (p.x-x)*(p.x-x);
        double YY = (p.y-y)*(p.y-y);
        #if FUZZY_MIN
            a = max(double(a),exp(-Cmx*XX - Cmy*YY));
		#elif PROMISCUIDAD
			double r = radius[i];
			if (XX+YY < r*r)
			{
				a = 1.0;
			}
        #else
            a *= (1.0-exp(-Cmx*XX - Cmy*YY));
        #endif
    }
// public:
private: // variables
	vector<double> Area;	
	size_t grid;
	double Cx; //in continuum
	double Cy;
	
	double Cnx; // in normalized continuum
	double Cny;
	
	double Cmx; //in grid
	double Cmy;

	double bx; //border in continuum
	double by; 
	
	vector<double> radius;
	
	Point O;
	Point W;
	Point F;

	vector<vector<Point>> E;
	
	size_t num_cols_per_block;
	size_t num_full_blocks;
	size_t num_partial_blocks {0}; //0 or 1
	size_t num_cols_partial_block {0};
};



