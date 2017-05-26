#pragma once
#include <vector>
#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"

using namespace std;

using Row = vector<real>;
using Matrix = vector<Row>;

const real tolerance = 0.00000001; //en el continuo original


inline real MaximaLongitudQueNoEs0(real C)
{
	return sqrt(-log(tolerance)/C);
}

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
	
	void Realize(const vector<Point>& P, real Cx, real Cy);
	
	real Integrate() const;
	
private:
	Matrix m_M;
};

std::ostream& operator<<(std::ostream& os, const Mu& C);

Mu operator+(const Mu& A, const Mu& B);
Mu operator*(const Mu& A, const Mu& B);