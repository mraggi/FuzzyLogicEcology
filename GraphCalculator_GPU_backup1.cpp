#include <iomanip>
#include <cassert>
#include <memory>
#include <arrayfire.h>

#include "GraphCalculator.hpp"

void Realize(af::array& A, const vector<vector<Point>>& P, size_t grid, double Cmx, double Cmy)
{
	double sigma_x = 1.0/sqrt(2.0*Cmx);
	double sigma_y = 1.0/sqrt(2.0*Cmy);

	double dx = MaxNonZeroDistance(Cmx);
	double dy = MaxNonZeroDistance(Cmy);

	long dxi = long(dx);
	long dyi = long(dy);
	
	size_t szx = 2*dxi;
	size_t szy = 2*dyi;
	
	af::array GK = af::gaussianKernel(szx,szy,sigma_x,sigma_y);
	GK /= max<double>(GK);
	
	for (int s = 0; s < P.size(); ++s)
	{
		for (const Point& p : P[s])
		{
	// 		size_t minX = max(long(0),long(p.x)-dxi);
	// 		size_t maxX = min(long(grid), long(p.x)+dxi);
	// 		size_t minY = max(long(0),long(p.y)-dyi);
	// 		size_t maxY = min(long(grid), long(p.y)+dyi);
			
			size_t minX = long(p.x+0.5)-dxi;
			size_t maxX = long(p.x+0.5)+dxi;
			size_t minY = long(p.y+0.5)-dyi;
			size_t maxY = long(p.y+0.5)+dyi;
			assert(minX >= 0);
			assert(minY >= 0);
			assert(maxX < grid);
			assert(maxY < grid);
			
			A(s,af::seq(minX,maxX-1),af::seq(minY,maxY-1)) *= (1.0-GK);
		}
	}
	A += 1.0;
	
}

Matrix GraphCalculator::CalculateGraph()
{
	int numspecies = E.size();
	Matrix M(numspecies,Row(numspecies,1.0));

	// Simplemente para adivinar cuánto me voy a tardar :)
	Chronometer T;	
	int total = (numspecies*(numspecies-1))/2;
	int num = 0;
	
	af::array A(numspecies, grid, grid,f64);
	
	vector<double> Area(numspecies,0.0);
	for (int i = 0; i < numspecies; ++i)
	{
		Area[i] = af::sum<double>(A(i,af::span,af::span));
	}
	moddims(A,numspecies,grid*grid);
	
	af::array M = af::matmul(A,transpose(A));
	
	for (int i = 0; i < numspecies; ++i)
	{
		
		double areai = af::sum<double>(A(i,af::span,af::span));
		for (int j = i+1; j < numspecies; ++j)
		{
			
			double areaj = af::sum<double>(A(j,af::span,af::span));
			
			double overlap = af::sum<double>(A*B);
			if (areai != 0.0)
				M[i][j] = overlap/areai;
			if (areaj != 0.0)
				M[j][i] = overlap/areaj;
			if (M[i][j] < 0.001)
				M[i][j] = 0.0;
			if (M[j][i] < 0.001)
				M[j][i] = 0.0;
			
			++num;
			double t = T.Peek();
			cout << "Estimated time: " << t*total/num - t << "s" << endl;
			
		}
	}
	
	for (auto ptr : SavedArrays)
		delete ptr;
	
	return M;
}
