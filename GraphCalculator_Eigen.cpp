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
	double dx = MaximaLongitudQueNoEs0(Cmx);
	double dy = MaximaLongitudQueNoEs0(Cmy);
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
	int numspecies = E.size();
	
	MatrixXd A(numspecies,malla*malla);
	cout << "A.rows() = " << A.rows() << endl;
	cout << "A.cols() = " << A.cols() << endl;
	
	
	
	for (int species = 0; species < numspecies; ++species)
	{
		Realize(A,E[species],species,malla,Cmx,Cmy);
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
// 	cout << "O = " << O << endl;
// 	cout << "W = " << W << endl;
// 	cout << "En km, F = " << F << endl;
	
	Cnx = Cx*F.x*F.x;
	Cny = Cx*F.y*F.y;
	
// 	cout << "Cnx = " << Cnx << endl;
	
	Cmx = Cnx/(malla*malla);
	Cmy = Cny/(malla*malla);
	
// 	cout << "Cmx = " << Cmx << endl;
// 	cout << "O = " << O << " y W = " << W << endl;
	
// 	cout << "bordeEnContinuo = " << bordeEnContinuo << endl;
// 	cout << "b = " << b << endl;
	
	vector<vector<Point>> Q(U.size());
	int i = 0;
	for (auto& u : U)
	{
		for (auto& P : u)
		{
			
			Point S = (P-O);
			
// 			cout << S << endl;
			
			S.Scale(1.0/F);
			S *= malla;
			Q[i].emplace_back(S);
		}
		++i;
// 		cout << "---------------------------------" << endl;
	}
	return Q;
}
