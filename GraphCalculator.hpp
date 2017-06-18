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
