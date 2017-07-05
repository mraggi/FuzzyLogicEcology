#pragma once

#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Point.hpp"
#include "scalar_min.hpp"

// Blaze stuff
#if USE_BLAZE
	#include <blaze/config/BLAS.h>
	#include <blaze/Blaze.h>
	using MatrixXd = blaze::DynamicMatrix<double>;
#else
	#include <eigen3/Eigen/Dense>
	#if FUZZY_MIN
		using MatrixXd = Eigen::Matrix<scalar_min_t,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>;
	#else
		using MatrixXd = Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>;
	#endif
#endif
// using MatrixXd = blaze::DynamicMatrix<scalar_min_t>;

//End blaze stuff

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
		
	Point O;
	Point W;
	Point F;

	vector<vector<Point>> E;
	
	size_t num_cols_per_block;
	size_t num_full_blocks;
	size_t num_partial_blocks {0}; //0 or 1
	size_t num_cols_partial_block {0};
};



