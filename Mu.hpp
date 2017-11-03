#pragma once
#include <vector>


#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"

class Mu
{
public:
	explicit Mu(size_t n) : m_n(n), m_M(n,Row(n,0))
	{ 
	}
	
	double operator()(size_t x, size_t y) const
	{
		return m_M[x][y];
	}
	double& operator()(size_t x, size_t y)
	{
		return m_M[x][y];
	}
	size_t numcols() const { return m_n; }
	size_t numrows() const { return m_n; }
	
	void Realize(const std::vector<Point>& P, double Cmx, double Cmy);
	
	double Integrate() const;
	
	friend Mu operator+(const Mu& A, const Mu& B);
	friend Mu operator*(const Mu& A, const Mu& B);
	
private:
	size_t m_n;
	Matrix m_M;
};

std::ostream& operator<<(std::ostream& os, const Mu& C);

Mu operator+(const Mu& A, const Mu& B);
Mu operator*(const Mu& A, const Mu& B);
