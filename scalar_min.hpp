#pragma once
#include <iostream>
#include <algorithm>

struct scalar_min_t
{
	scalar_min_t(double a = 0.0) : value(a) {} 
	scalar_min_t(const scalar_min_t& x) = default;
	operator double&() { return value; }
	operator double() const { return value; }
// 	scalar_min_t operator-() const { return scalar_min_t(-value); }
	double value;
};

// inline bool operator<(const scalar_min_t& x, const scalar_min_t& y)
// {
// 	return x.value < y.value;
// }

inline scalar_min_t operator*(const scalar_min_t& x, const scalar_min_t& y)
{
	return std::min(x,y);
// 	return 1;
}

inline scalar_min_t operator+(const scalar_min_t& x, const scalar_min_t& y)
{
	return scalar_min_t(x.value+y.value);
}

inline scalar_min_t operator-(const scalar_min_t& x, const scalar_min_t& y)
{
	return scalar_min_t(x.value-y.value);
}

