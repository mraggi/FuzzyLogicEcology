#pragma once
#include <iostream>
#include <algorithm>

// this struct is used to replace double when using the version of fuzzy logic that uses min instead of prod.
struct scalar_min_t
{
	scalar_min_t(double a = 0.0) : value(a) {} 
	scalar_min_t(const scalar_min_t& x) = default;
	operator double&() { return value; }
	operator double() const { return value; }
	double value;
};

inline scalar_min_t operator*(const scalar_min_t& x, const scalar_min_t& y)
{
	return std::min(x,y);
}

inline scalar_min_t operator+(const scalar_min_t& x, const scalar_min_t& y)
{
	return scalar_min_t(x.value+y.value);
}

inline scalar_min_t operator-(const scalar_min_t& x, const scalar_min_t& y)
{
	return scalar_min_t(x.value-y.value);
}
