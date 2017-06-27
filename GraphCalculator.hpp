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
		
		auto numspecies = E.size();
		
        size_t memoryPerColumn = numspecies*sizeof(double)  + 36; //36 is just a number I came up with. Not based on reality or anything really.
        
        size_t numberOfColumns = grid*grid;
        
        size_t memoryNeeded = numberOfColumns*memoryPerColumn;
        
        size_t memoryAvailable = getTotalSystemMemory();
        
        if (memoryAvailable < 2*GB)
        {
            throw "Not enough memory to run this program!";
        }
		
		memoryAvailable -= 1*GB; // Leave at least 1 GB for the OS
		
		num_cols_per_block = memoryAvailable/memoryPerColumn;
		num_blocks = (grid*grid)/num_cols_per_block  + 1; //adjust later
		
		
		cout << "Memory needed: " << double(memoryNeeded)/GB << "GB" << endl;
        cout << "Available memory: " << double(memoryAvailable)/GB << "GB" << endl;
        cout << "So I need " << num_blocks << " blocks" << endl;
		cout << "Each of size " << num_cols_per_block << endl;
        cout << "Each consumes " << double(num_cols_per_block*memoryPerColumn)/GB << "GB" << endl; 
        cout << "Except perhaps the last one, which will be of size " << (grid*grid)%num_blocks << endl;
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
	
	size_t num_cols_per_block;
	size_t num_blocks;
	
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
