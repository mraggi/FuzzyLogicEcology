#pragma once
#include <vector>
#include <arrayfire.h>
#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"

using namespace std;

using Row = vector<double>;
using Matrix = vector<Row>;

const double tolerance = 0.0000000153; //en el continuo original


inline double MaximaLongitudQueNoEs0(double C)
{
	return sqrt(-log(tolerance)/C);
}

class Mu
{
public:
	Mu(size_t n) : m_n(n), m_M(af::constant(0,n,n,f64))
	{ 
	}
	
// 	double operator()(size_t x, size_t y) const
// 	{
// 		return m_M[x][y];
// 	}
// 	double& operator()(size_t x, size_t y)
// 	{
// 		return m_M[x][y];
// 	}
	size_t numcols() const { return m_n; }
	size_t numrows() const { return m_n; }
	
	void Realize(const vector<Point>& P, double Cmx, double Cmy);
	
	double Integrate() const;
	
	const af::array& get_array() const
	{
		return m_M;
	}
	
	friend Mu operator+(const Mu& A, const Mu& B);
	friend Mu operator*(const Mu& A, const Mu& B);
	
private:
// 	Matrix m_M;
	size_t m_n;
	af::array m_M;
};

std::ostream& operator<<(std::ostream& os, const Mu& C);

Mu operator+(const Mu& A, const Mu& B);
Mu operator*(const Mu& A, const Mu& B);
