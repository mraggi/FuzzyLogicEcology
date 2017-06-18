#include <iomanip>
#include "GraphCalculator.hpp"
#include "Mu.hpp"
#include <blaze/Math.h>

// using namespace blaze;

using MatrixXd = blaze::DynamicMatrix<double>;

void Realize(MatrixXd& A, const vector<Point>& P, int row, int N, double Cmx, double Cmy)
{
	double dx = MaxNonZeroDistance(Cmx);
	double dy = MaxNonZeroDistance(Cmy);
	long dxi = long(dx+1);
	long dyi = long(dy+1);
	
// 	cout << "di = " << di << endl;
	
// 	cout << "Número de pixeles: " << 2*(dxi) << "*" << 2*dyi << " = " << 4*dxi*dyi << endl;
	
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
	
	cout << "Number of nonzeros: " << double(A.nonZeros())/(numspecies*grid*grid) << endl;
	
// 	A = blaze::forEach(A,[](double d) { return d+1.0; });
	
	cout << "Finished creating matrix. Time taken: " << T.Reset() << endl;
	
// 	MatrixXd Area = A.rowwise().sum();
// 	cout << Area << endl;
	
	
	MatrixXd M = blaze::declsym( A * blaze::trans(A) );

	Matrix R(numspecies,Row(numspecies));
	for (int x = 0; x < numspecies; ++x)
	{
		for (int y = 0; y < numspecies; ++y)
		{
			R[x][y] = M(x,y)/Area[x];
// 			R[x][y] = M(x,y);
		}
	}
	
	return R;
}

vector<vector<Point>> GraphCalculator::Normalized(const vector<vector<Point>>& U)
{
	double minX = 9999999999;
	double maxX = -9999999999;
	double minY = 9999999999;
	double maxY = -9999999999;
	
	for (auto& u : U)
	{
		
		for (auto p : u)
		{
			double x = p.x;
			double y = p.y;
			if (x < minX)
				minX = x;
			if (y < minY)
				minY = y;
			if (x > maxX)
				maxX = x;
			if (y > maxY)
				maxY = y;
		}
	}
	O = Point(minX-bx,minY-by);
	W = Point(maxX+bx,maxY+by);
	F = W-O;
	
	Cnx = Cx*F.x*F.x;
	Cny = Cx*F.y*F.y;
	
	Cmx = Cnx/(grid*grid);
	Cmy = Cny/(grid*grid);
	
	vector<vector<Point>> Q(U.size());
	int i = 0;
	for (auto& u : U)
	{
		for (auto& P : u)
		{
			Point S = (P-O);
			
			S.Scale(1.0/F);
			S *= grid;
			Q[i].emplace_back(S);
		}
		++i;
	}
	return Q;
}
