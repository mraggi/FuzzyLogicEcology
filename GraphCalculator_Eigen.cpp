#include <iomanip>
#include "GraphCalculator.hpp"
#include "Mu.hpp"
#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;

void Realize(MatrixXd& A, const vector<Point>& P, int row, int N, double Cmx, double Cmy)
{
	for (int x = 0; x < N; ++x)
	{
		for (int y = 0; y < N; ++y)
		{
			int index = x*N+y;
			
			A(row,index) = -1.0;
		}
	}
	double dx = MaxNonZeroDistance(Cmx);
	double dy = MaxNonZeroDistance(Cmy);
	long dxi = long(dx+1);
	long dyi = long(dy+1);
	
	for (const Point& p : P)
	{
		size_t minX = max(long(0),long(p.x)-dxi);
		size_t maxX = min(long(N), long(p.x)+dxi);
		size_t minY = max(long(0),long(p.y)-dyi);
		size_t maxY = min(long(N), long(p.y)+dyi);
		
		for (int x = minX; x < maxX; ++x)
		{
			for (int y = minY; y < maxY; ++y)
			{
				int index = x*N+y;
				double XX = (p.x-x)*(p.x-x);
				double YY = (p.y-y)*(p.y-y);
				A(row,index) *= (1.0-exp(-Cmx*XX - Cmy*YY));
			}
		}
	}
	
	for (size_t x = 0; x < N; ++x)
	{
		for (size_t y = 0; y < N; ++y)
		{			
			int index = x*N+y;
			A(row,index) += 1.0;
		}
	}
	
}

Matrix GraphCalculator::CalculateGraph()
{
	int numspecies = E.size();
	
	MatrixXd A(numspecies,grid*grid);
	cout << "A.rows() = " << A.rows() << endl;
	cout << "A.cols() = " << A.cols() << endl;
	
	
	
	for (int species = 0; species < numspecies; ++species)
	{
		Realize(A,E[species],species,grid,Cmx,Cmy);
	}
	
	MatrixXd Area = A.rowwise().sum();
	cout << Area << endl;
	
	
	MatrixXd M = A*(A.transpose());

	cout << "Finished multiplying! " << M.rows() << " " << M.cols() << endl;
	
	Matrix R(numspecies,Row(numspecies));
	for (int x = 0; x < numspecies; ++x)
	{
		for (int y = 0; y < numspecies; ++y)
		{
			R[x][y] = M(x,y)/Area(x,0);
		}
	}
	
	return R;
}
