#include <iomanip>
#include <cassert>
#include <memory>
#include <arrayfire.h>

#include "GraphCalculator.hpp"

void Realize(af::array& A, const vector<vector<Point>>& P, size_t malla, double Cmx, double Cmy)
{
	double sigma_x = 1.0/sqrt(2.0*Cmx);
	double sigma_y = 1.0/sqrt(2.0*Cmy);

	double dx = MaximaLongitudQueNoEs0(Cmx);
	double dy = MaximaLongitudQueNoEs0(Cmy);

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
	// 		size_t maxX = min(long(malla), long(p.x)+dxi);
	// 		size_t minY = max(long(0),long(p.y)-dyi);
	// 		size_t maxY = min(long(malla), long(p.y)+dyi);
			
			size_t minX = long(p.x+0.5)-dxi;
			size_t maxX = long(p.x+0.5)+dxi;
			size_t minY = long(p.y+0.5)-dyi;
			size_t maxY = long(p.y+0.5)+dyi;
			assert(minX >= 0);
			assert(minY >= 0);
			assert(maxX < malla);
			assert(maxY < malla);
			
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
	
	af::array A(numspecies, malla, malla,f64);
	
	vector<double> Area(numspecies,0.0);
	for (int i = 0; i < numspecies; ++i)
	{
		Area[i] = af::sum<double>(A(i,af::span,af::span));
	}
	moddims(A,numspecies,malla*malla);
	
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
	
	Cmx = Cnx/(malla*malla);
	Cmy = Cny/(malla*malla);
	
	vector<vector<Point>> Q(U.size());
	int i = 0;
	for (auto& u : U)
	{
		for (auto& P : u)
		{
			
			Point S = (P-O);
			
			S.Scale(1.0/F);
			S *= (malla-1);
			Q[i].emplace_back(S);
		}
		++i;
	}
	return Q;
}
