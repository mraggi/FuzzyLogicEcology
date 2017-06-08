#pragma once
#include <random>
#include <iostream>
#include <vector>

using namespace std;

template <class T> 
std::ostream& operator<<(std::ostream& os, const vector<T>& X)
{
	for (const auto& x : X)
		os << x << " ";
	return os;
}

template <class T> 
std::ostream& operator<<(std::ostream& os, const vector<vector<T>>& X)
{
	for (const auto& x : X)
		os << x << endl;
	return os;
}

inline std::default_random_engine& random_engine()
{
	static std::default_random_engine e {};
	return e;
}

/**
 * @brief Returns true with probability p and false with probability 1-p
 * @return true or false according to probability p, which must be a number between 0 and 1.
 */
inline bool probability_of_true(double p)
{
	static std::bernoulli_distribution d(p);
	return d(random_engine());
}

/**
 * @brief The modern way of setting a random seed.
 */
inline void randomize()
{
	srand(time(nullptr));
	static std::random_device rd {};
	random_engine().seed(rd());
}


/**
 * @brief "I just wanted a random integer!
 * @return A random integer in the range [from,thru), with uniform probability distribution
 */
template <class IntType = int>
IntType random_int(IntType from, IntType thru)
{
	static std::uniform_int_distribution<IntType> d {};
	using parm_t = typename decltype(d)::param_type;
	return d(random_engine(), parm_t {from, thru - 1});
}

/**
 * @brief "I just wanted a random integer!
 * @return A random integer in the range [from,thru), with uniform probability distribution
 */
template <class FloatType = double>
FloatType random_double(FloatType from, FloatType upto)
{
	static std::uniform_real_distribution<> d {};
	using parm_t = decltype(d)::param_type;
	return d(random_engine(), parm_t {from, upto});
}

template <class T>
int signof(const T& a)
{
	if (a > 0)
		return 1;
	if (a < 0)
		return -1;
	return 0;
}


using Row = vector<double>;
using Matrix = vector<Row>;

const double tolerance = 0.0000000153; //en el continuo original


inline double MaximaLongitudQueNoEs0(double C)
{
	return sqrt(-log(tolerance)/C);
}
/*

double tolerancia_fisica(double numsigma)
{
	return exp(-numsigma*numsigma/2.0);
}*/