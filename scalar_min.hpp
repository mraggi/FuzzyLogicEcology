#pragma once
#include <algorithm>
#include <iostream>

// this struct is used to replace double when using the version of fuzzy logic that uses min instead of prod.
struct scalar_min_t
{
	scalar_min_t(double a = 0.0) : value(a) {}  //NOLINT
	operator double&() { return value; } //NOLINT
	operator double() const { return value; } //NOLINT
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
