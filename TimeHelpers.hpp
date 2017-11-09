#pragma once

#include <chrono>
#include <ctime>

inline double diffclock(clock_t a, clock_t b)
{
	const double c = 1.0 / CLOCKS_PER_SEC;
	return double(a - b) * c;
}

using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

inline double diff_time_point(time_point a, time_point b)
{

	const double t = 0.000001;
	return std::chrono::duration_cast<std::chrono::microseconds>(a - b).count() * t;
}

class RClock
{
public:
	static RClock& Instance()
	{
		static RClock A;
		return A;
	}

	time_point start_timer;


private:
	RClock() : start_timer(std::chrono::high_resolution_clock::now()) {}
};

class Chronometer
{
public:
	Chronometer() : m_timer(std::chrono::high_resolution_clock::now()) {}

	double Reset()
	{
		auto tlast = m_timer;
		m_timer = std::chrono::high_resolution_clock::now();

		return diff_time_point(m_timer, tlast);
	}

	double Peek() const
	{
		auto tnow = std::chrono::high_resolution_clock::now();

		return diff_time_point(tnow, m_timer);
	}

	time_point m_timer;
};

inline double TimeFromStart()
{
	auto tnow = std::chrono::high_resolution_clock::now();

	return diff_time_point(tnow, RClock::Instance().start_timer);
}
