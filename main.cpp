#include <iostream>
#include <iomanip>

#include "Point.hpp"
#include "TimeHelpers.hpp"

const double tolerance = 0.00001;
const size_t malla = 10000;

using Row = vector<real>;
using Matrix = vector<Row>;

class Mu
{
public:
	Mu(size_t n) : m_M(n,Row(n,0))
	{ 
	}
	
	real operator()(size_t x, size_t y) const
	{
		return m_M[x][y];
	}
	real& operator()(size_t x, size_t y)
	{
		return m_M[x][y];
	}
	size_t numcols() const { return m_M.size(); }
	size_t numrows() const { return m_M.size(); }
	
	void Realize(const vector<Point>& P, real C);
	
	real Integrate() const;
	
private:
	
	
	Matrix m_M;
};

Mu operator+(const Mu& A, const Mu& B)
{
	Mu C(A.numrows());
	for (size_t x = 0; x < C.numcols(); ++x)
	{
		for (size_t y = 0; y < C.numrows(); ++y)
		{
			C(x,y) = A(x,y) + B(x,y) - A(x,y)*B(x,y);
		}
	}
	return C;
}

Mu operator*(const Mu& A, const Mu& B)
{
	Mu C(A.numrows());
	for (size_t x = 0; x < C.numcols(); ++x)
	{
		for (size_t y = 0; y < C.numrows(); ++y)
		{
			C(x,y) = A(x,y)*B(x,y);
		}
	}
	return C;
}

void Mu::Realize(const vector<Point>& P, real C)
{
	// TODO: Pasar lo de los puntos para afuera y dependiendo de C, ver qué debo modificar para cada punto.
	// Seguramente búsuqeda binaria
	
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			m_M[x][y] = -1;
		}
	}
	
	double d = sqrt(-log(tolerance)/C);
	long di = long(d+1);
// 	cout << "di = " << di << endl;
	for (const Point& p : P)
	{
		size_t minX = max(long(0),long(p.x)-di);
		size_t maxX = min(long(numcols()), long(p.x)+di);
		size_t minY = max(long(0),long(p.y)-di);
		size_t maxY = min(long(numrows()), long(p.y)+di);
		
		for (size_t x = minX; x < maxX; ++x)
		{
			for (size_t y = minY; y < maxY; ++y)
			{
				real d2 = p.DistanceSq(Point(x,y));
				m_M[x][y] *= (1-exp(-C*d2));
			}
		}
		
	}
	
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			m_M[x][y] += 1;
			if (abs(m_M[x][y]) < tolerance)
				m_M[x][y] = 0.0;
		}
	}
	
}

real Mu::Integrate() const
{
	real result = 0;
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			result += m_M[x][y];
		}
	}
	return result;
}

std::ostream& operator<<(std::ostream& os, const Mu& C)
{
	const size_t maxtoprint = 20;
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

real random_real()
{
	int r = rand();
	return double(r)/RAND_MAX;
}

vector<Point> GenerateRandomPoints(int n, int resolution)
{
	vector<Point> result;
	
	result.reserve(n);
	
	while (result.size() < n)
	{
		result.emplace_back(random_real()*resolution, random_real()*resolution);
	}

	
	return result;
}

int main() 
{
	
	Chronometer T;
	
    vector<Point> Especie1 = GenerateRandomPoints(1000,malla);
    vector<Point> Especie2 = GenerateRandomPoints(1000,malla);
	
	Mu A(malla);
	A.Realize(Especie1,0.005);
	
	cout << "Para la malla 1 tardé " << T.Reset() << endl; 
	
	Mu B(malla);
	B.Realize(Especie2,0.005);
	
	cout << "Para la malla 2 tardé " << T.Reset() << endl; 

	
	cout << "La arista de A a B debería tener peso: " << (A*B).Integrate()/A.Integrate() << endl;
	cout << "La arista de B a A debería tener peso: " << (A*B).Integrate()/B.Integrate() << endl;
	
	
    return 0;
}
