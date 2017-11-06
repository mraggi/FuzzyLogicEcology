#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "Utility.hpp"

constexpr double pi = 3.1415926535897932384626433832795028841971694;
constexpr double EarthRadiusKm = 6371.0;
constexpr double KmInADegree = (pi*EarthRadiusKm)/180.0;

inline double RadiansToDegrees(double angle)
{
	return angle*180.0/pi;
}

//Returns an angle that is between -pi and pi.
inline double MakeValidAngle(double theta)
{
	while (theta > pi) theta -= 2*pi;
	while (theta < -pi) theta += 2*pi;
	return theta;
}


inline bool isAngleBetweenAngles(double a, double b1, double b2)
{
	double c = MakeValidAngle(a-b1+pi);
	double c2 = MakeValidAngle(b2-b1+pi);
	
	return c2 <= c ;
}



struct Point
{
	double x {0};
	double y {0};

	Point() = default;
	Point(double X, double Y) : x(X), y(Y) { }

	void Zero(){x=0.0; y=0.0;}
	bool isZero() const {return (x == 0 && y == 0);}

	inline double Length() const   { return sqrt(x*x + y*y); }
	inline double LengthSq() const { return x*x + y*y; }

	inline double Angle() const	{ return atan2(y,x); }
	inline double AngleTo(const Point& vec)const { return MakeValidAngle(vec.Angle()-Angle()); }

	inline bool IsCloserToFirstThanSecond(const Point& A, const Point& B) const { return DistanceSq(A) < DistanceSq(B); }
	
	Point VectorWithAngle(double t) const; //std::vector of same length and angle t
	void SetAngle(double t);

	void Rotate(double t);
	Point Rotated(double t) const;
	
	void Rotate(double sint, double cost); //so that you don't calculate sin and cos again.
	Point Rotated(double sint, double cost) const; //so that you don't calculate sin and cos again.

	void Normalize();
	Point Normalized() const;

	void Scale(double factor) { x*=factor; y*=factor; }
	void Scale(double xfactor, double yfactor) { x*=xfactor; y*=yfactor; }
	void Scale(const Point& P) { x*=P.x; y*=P.y; }

	Point Scaled(const Point& P) const	{ return {x*P.x, y*P.y};	}

	double DistanceSq(const Point& vec) const
	{
		double disX = (vec.x - x);
		double disY = (vec.y - y);
		return disX*disX + disY*disY;
	}

	double Distance(const Point& vec) const
	{
		return sqrt(DistanceSq(vec));
	}
	
	void SetLength(double r);
	void SetLengthSq(double r2); //This computes sqrt only once, instead of twice
	void SetPolar(double r, double t);

	Point WithLength(double r) const;
	Point WithLengthSq(double r) const;

	Point Projection(const Point& H) const;
	Point ProjectionToLine(const Point& A, const Point& B) const;
	
	
	bool IsToTheLeftOfLine(const Point& A, const Point& B) const; //strictly to the left
	bool IsOnLine(const Point& A, const Point& B) const;
	bool IsToTheRightOfLine(const Point& A, const Point& B) const; //strictly to the right
	
	//This returns the projection if it's "positive" (wrt H) and returns 0 if it's negative
	Point ForwardProjection(const Point& H) const;

	void Truncate(double r);
	Point Truncated(double r) const;

	//Returns a point with same length that is rotated pi/2 counter-clockwise
	inline Point Perp() const	{ return Point(-y, x); }

	/* start Operator definitions */

	inline void operator+=(const Point &vec) { x += vec.x; y += vec.y;}
	inline void operator-=(const Point &vec) { x -= vec.x; y -= vec.y;}
	inline void operator*=(double num)			{ x *= num; y *= num;}
	inline void operator/=(double num)			{ x /= num; y /= num;}
	inline Point operator-(void) const { return Point(-x,-y); }

	bool operator!=(const Point &vec) const;
	bool operator==(const Point& vec) const;

	
	Point LocalToGlobal(const Point& origin, const Point& U, const Point& V) const;

	
	Point GlobalToLocal(const Point& origin,
										const Point& U,
										const Point& V) const;
										
	static Point Polar(double r, double theta);
	static Point RandomPoint(double maxLength);
	static Point RandomPoint(double minLength, double maxLength);
	
};

inline Point operator+(const Point &vecA, const Point &vecB)
{
	return Point(vecA.x+vecB.x,vecA.y+vecB.y);
}

inline Point operator-(const Point &vecA, const Point &vecB)
{
	return Point(vecA.x-vecB.x,vecA.y-vecB.y);
}

//Multiply and divide by scalars on both sides;
inline Point operator*(double num, const Point &vec)
{
	return Point(num*vec.x,num*vec.y);
}

inline Point operator/(double num, const Point &vec)
{
	return Point(num/vec.x,num/vec.y);
}

inline Point operator*(const Point &vec, double num)
{
	return Point(num*vec.x,num*vec.y);
}

inline Point operator/(const Point &vec, double num)
{
	return Point(vec.x/num,vec.y/num);
}

//dot product
inline double operator*(const Point &vecA, const Point &vecB)
{
	return (vecA.x*vecB.x+vecA.y*vecB.y);
}

inline double distance(const Point& A, const Point& B)
{
	return A.Distance(B);
}

inline double distance_squared(const Point& A, const Point& B)
{
	return A.DistanceSq(B);
}

std::ostream& operator<<(std::ostream& os, const Point& rhs);

bool operator<(const Point& A, const Point& B);

std::vector<Point> GenerateRandomPoints(int n, int resolution);
