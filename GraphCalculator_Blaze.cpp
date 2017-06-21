#include <iomanip>
#include "GraphCalculator.hpp"
#include "Mu.hpp"
#include <blaze/Math.h>
#include <blaze/config/BLAS.h>
// using namespace blaze;

using MatrixXd = blaze::DynamicMatrix<double>;
// using MatrixXd = blaze::CompressedMatrix<double>;

void Realize(MatrixXd& A, const vector<Point>& P, int row, int N, double Cmx, double Cmy)
{
	double dx = MaxNonZeroDistance(Cmx);
	double dy = MaxNonZeroDistance(Cmy);
	long dxi = long(dx+2);
	long dyi = long(dy+2);
	
// 	cout << "di = " << di << endl;
	
// 	cout << "Número de pixeles: " << 2*(dxi) << "*" << 2*dyi << " = " << 4*dxi*dyi << endl;
	
	for (const Point& p : P)
	{
		size_t minX = max(long(0),long(p.x)-dxi);
		size_t maxX = min(long(N), long(p.x)+dxi);
		size_t minY = max(long(0),long(p.y)-dyi);
		size_t maxY = min(long(N), long(p.y)+dyi);
		assert(maxX-minX > 0);
		assert(maxY-minY > 0);
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
	blaze::setNumThreads( blaze::getNumThreads() );
	Chronometer T;
	int numspecies = E.size();
	
	MatrixXd A(numspecies,grid*grid,-1.0);
	vector<double> Area(numspecies,0.0);
	
	#pragma omp parallel for
	for (int species = 0; species < numspecies; ++species)
	{
		Realize(A,E[species],species,grid,Cmx,Cmy);
		for (int col = 0; col < A.columns(); ++col)
			Area[species] += A(species,col);
	}
	
// 	cout << "Number of nonzeros: " << double(A.nonZeros())/(numspecies*grid*grid) << endl;
	
// 	A = blaze::forEach(A,[](double d) { return d+1.0; });
	
	cout << "Finished creating matrix. Time taken: " << T.Reset() << endl;
	
// 	MatrixXd Area = A.rowwise().sum();
// 	cout << Area << endl;
	
	
	MatrixXd M = blaze::declsym( A * blaze::trans(A) );

	Matrix R(numspecies,Row(numspecies));
	for (int x = 0; x < numspecies; ++x)
	{
		if (Area[x] < tolerance)
		{
			cout << "Area of X = 0! Increase grid size!!" << endl;
		}
		for (int y = 0; y < numspecies; ++y)
		{
			if (Area[x] > tolerance)
				R[x][y] = sqrt(M(x,y)/Area[x]);
			else
				R[x][y] = sqrt(M(x,y));
// 			R[x][y] = M(x,y);
		}
	}
	
	return R;
}
