#pragma once
#include <iostream>
#include <random>
#include <vector>

template <class T> 
std::ostream& operator<<(std::ostream& os, const std::vector<T>& X)
{
	for (const auto& x : X)
		os << x << " ";
	return os;
}

// template <class T> 
// std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T>>& X)
// {
// 	for (const auto& x : X)
// 		os << x << std::endl;
// 	return os;
// }

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


const double tolerance = 0.00000153; //en el continuo original
const double inf = 999999999999999.0;

inline double MaxNonZeroDistance(double C)
{
	return sqrt(-log(tolerance)/C);
}
/*

double tolerancia_fisica(double numsigma)
{
	return exp(-numsigma*numsigma/2.0);
}*/
const int64_t KB = 1024;
const int64_t MB = KB*1024;
const int64_t GB = MB*1024;


#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
	#include <unistd.h>

	inline int64_t getTotalSystemMemory()
	{
		int64_t  pages = sysconf(_SC_PHYS_PAGES);
		int64_t  page_size = sysconf(_SC_PAGE_SIZE);
		return pages * page_size;
	}
#else 
	#include <windows.h>

	inline int64_t getTotalSystemMemory()
	{
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(status);
		GlobalMemoryStatusEx(&status);
		return status.ullTotalPhys;
	}
#endif
