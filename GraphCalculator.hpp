#pragma once

#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Point.hpp"

constexpr double EarthRadiusKm = 6371.0;
constexpr double KmInADegree = (pi*EarthRadiusKm)/180.0;

class GraphCalculator
{
public:
	GraphCalculator(size_t _grid, double VisibilityRangeInKm, const vector<vector<Point>>& U) : grid(_grid)
	{
		double sigma = VisibilityRangeInKm;

		Cx = 1/(2*sigma*sigma);
		Cy = Cx;
		bx = MaxNonZeroDistance(Cx);
		by = MaxNonZeroDistance(Cy);
		
		auto UinKm = U;
		for (auto& conjDePuntos : UinKm)
		{
			for (auto& punto : conjDePuntos)
			{
				punto *= KmInADegree;
			
			}
		}
			
		E = Normalized(UinKm);
	}
	
	Matrix CalculateGraph();
	
private:
	vector<vector<Point>> Normalized(const vector<vector<Point>>& U);
	size_t grid;
	double Cx; //in continuum
	double Cy;
	
	double Cnx; // in normalized continuum
	double Cny;
	
	double Cmx; //in grid
	double Cmy;

	double bx; //border in continuum
	double by; 
		
	Point O;
	Point W;
	Point F;

	vector<vector<Point>> E;
};


inline vector<vector<Point>> GraphCalculator::Normalized(const vector<vector<Point>>& U)
{
	double minX = 9999999999;
	double maxX = -9999999999;
	double minY = 9999999999;
	double maxY = -9999999999;
	
	for (auto& u : U)
	{
		
		for (auto p : u)
		{
			double x = p.x;
			double y = p.y;
			if (x < minX)
				minX = x;
			if (y < minY)
				minY = y;
			if (x > maxX)
				maxX = x;
			if (y > maxY)
				maxY = y;
		}
	}
	O = Point(minX-bx,minY-by);
	W = Point(maxX+bx,maxY+by);
	F = W-O;
	
	Cnx = Cx*F.x*F.x;
	Cny = Cx*F.y*F.y;
	
	Cmx = Cnx/(grid*grid);
	Cmy = Cny/(grid*grid);
	
	vector<vector<Point>> Q(U.size());
	int i = 0;
	for (auto& u : U)
	{
		for (auto& P : u)
		{
			Point S = (P-O);
			
			S.Scale(1.0/F);
			S *= grid;
			Q[i].emplace_back(S);
		}
		++i;
	}
	return Q;
}
