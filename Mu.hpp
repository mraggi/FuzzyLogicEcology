#pragma once
#include <vector>

#ifdef USE_GPU
	#include <arrayfire.h>
#endif

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
#ifdef USE_GPU
	Mu(size_t n) : m_n(n), m_M(af::constant(0,n,n,f64))
	{ 
	}
#else
	Mu(size_t n) : m_n(n), m_M(n,Row(n,0))
	{ 
	}
#endif
	
#ifndef USE_GPU
	double operator()(size_t x, size_t y) const
	{
		return m_M[x][y];
	}
	double& operator()(size_t x, size_t y)
	{
		return m_M[x][y];
	}
#endif
	size_t numcols() const { return m_n; }
	size_t numrows() const { return m_n; }
	
	void Realize(const vector<Point>& P, double Cmx, double Cmy);
	
	double Integrate() const;
	
#ifdef USE_GPU
	const af::array& get_array() const
	{
		return m_M;
	}
#endif
	
	friend Mu operator+(const Mu& A, const Mu& B);
	friend Mu operator*(const Mu& A, const Mu& B);
	
private:
// 	Matrix m_M;
	size_t m_n;
#ifdef USE_GPU
	af::array m_M;
#else
	Matrix m_M;
#endif
};

std::ostream& operator<<(std::ostream& os, const Mu& C);

Mu operator+(const Mu& A, const Mu& B);
Mu operator*(const Mu& A, const Mu& B);
