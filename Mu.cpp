#include "Mu.hpp"

#include <iomanip>

Mu operator+(const Mu& A, const Mu& B)
{
	Mu C(A.numrows());
	for (size_t x = 0; x < A.numrows(); ++x)
	{
		for (size_t y = 0; y < A.numcols(); ++y)
		{
			C(x,y) = A(x,y)+B(x,y);
		}
	}
	return C;

}


Mu operator*(const Mu& A, const Mu& B)
{
	Mu C(A.numrows());
	for (size_t x = 0; x < A.numrows(); ++x)
	{
		for (size_t y = 0; y < A.numcols(); ++y)
		{
			C(x,y) = A(x,y)*B(x,y);
		}
	}
	return C;
}

void Mu::Realize(const vector<Point>& P, double Cmx, double Cmy)
{
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			m_M[x][y] = -1.0;
		}
	}
	double dx = MaximaLongitudQueNoEs0(Cmx);
	double dy = MaximaLongitudQueNoEs0(Cmy);
	long dxi = long(dx+2);
	long dyi = long(dy+2);
	
// 	cout << "di = " << di << endl;
	
// 	cout << "Número de pixeles: " << 2*(dxi) << "*" << 2*dyi << " = " << 4*dxi*dyi << endl;
	
	for (const Point& p : P)
	{
	
		size_t minX = max(long(0),long(p.x)-dxi);
		size_t maxX = min(long(numcols()), long(p.x)+dxi);
		size_t minY = max(long(0),long(p.y)-dyi);
		size_t maxY = min(long(numrows()), long(p.y)+dyi);
		
		for (size_t x = minX; x < maxX; ++x)
		{
			for (size_t y = minY; y < maxY; ++y)
			{
				double XX = (p.x-x)*(p.x-x);
				double YY = (p.y-y)*(p.y-y);
				m_M[x][y] *= (1.0-exp(-Cmx*XX - Cmy*YY));
			}
		}
	}
	
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			m_M[x][y] += 1.0;
			if (abs(m_M[x][y]) < tolerance)
				m_M[x][y] = 0.0;
		}
	}
}

double Mu::Integrate() const
{
	double result = 0.0;
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			result += m_M[x][y];
		}
	}
// 	double n = m_M.size();
// 	return result/(n*n);
	return result;
}

std::ostream& operator<<(std::ostream& os, const Mu& C)
{
	const size_t maxtoprint = 30;
	os << '\n';
	os << setprecision(3);
	for (size_t x = 0; x < min(C.numcols(),maxtoprint); ++x)
	{
		for (size_t y = 0; y < min(C.numrows(),maxtoprint); ++y)
		{
			os << C(x,y) << '\t';
		}
		os << '\n';
	}
	return os;
}