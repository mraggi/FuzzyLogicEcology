#pragma once
#include <iostream>

#include <eigen3/Eigen/Dense>
#include "scalar_min.hpp"

using Matrix = Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>;

using MatrixFM = Eigen::Matrix<scalar_min_t,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>;

template <class Mat>
void printRowAsMatrix(const Mat& A, int row, int grid)
{
	for (int index = 0; index < A.cols(); ++index)
	{
// 		int x = index/grid;
		int y = index%grid;
		if (y == 0)
			std::cout << std::endl;
		std::cout << A(row,index) << ' ';
	}
	std::cout << std::endl;
}

template <class Mat>
void printNonZeros(const Mat& A)
{
	for (int x = 0; x < A.rows(); ++x)
	{
		for (int y = 0; y < A.cols(); ++y)
		{
			if (A(x,y) > scalar_min_t(0.001))
			{
				std::cout << "(" << x << "," << y << "): " << A(x,y) << '\n';
			}
		}
	}
}

// This does M += A*A^T
template <class Mat>
inline void AddTransposeProduct(Mat& M, Mat& A)
{
	M += A*A.transpose();
}

template <>
inline void AddTransposeProduct(Matrix& M, Matrix& A)
{
	M += A*A.transpose();
}

template <>
inline void AddTransposeProduct(MatrixFM& M, MatrixFM& A)
{
 	M += A.lazyProduct(A.transpose());	
}
