#pragma once

#include <iomanip>
#include <cassert>
#include <fstream>

#include "TimeHelpers.hpp"
#include "Point.hpp"
#include "MatrixUtils.hpp"
#include "Graph.hpp"

struct Interval
{
	Interval(long l, long r) : L(l), R(r) {}
	long L;
	long R;
};

template <class Mat>
class FuzzyNetworkBase
{
public:
	FuzzyNetworkBase(size_t _grid, const std::vector<std::vector<Point>>& U, size_t memoryAvailable) : 
		Area(U.size(),0.0)
		, grid(_grid)
		, E(U)
	{
		SetBlockSize(memoryAvailable);
// 		std::cout << "At beginning, Area = " << Area << std::endl;
	}
	
	void PrintEverything(const std::vector<std::string>& names, const std::string& sageoutfile);
	
	Mat CalculateGraph();
	
	double GetTotalArea(int species) const
	{
		return Area[species]*F.x*F.y/(grid*grid);
	}

	void printAreaVector(std::ostream& os = std::cout, char sep = ' ');

	size_t num_species() const { return Area.size(); }
	
	Point ContinuumToGrid(const Point& P) const
	{
		return (P-O).Scaled(grid/F);
	}
	
	Point GridToContinuum(const Point& Q) const
	{
		return O + Q.Scaled(F/grid);
	}
protected:
	
	virtual void ResetFullMatrix(Mat& A) = 0;
	
	virtual void PreInitialize() = 0; //before calling normalize
	virtual void PostInitialize() = 0; //after calling normalize

	virtual Point CalculateBorder() const = 0;

	Point MaxAffectedArea() const
	{
		return CalculateBorder().Scaled(grid/F);
	}
	
	Interval GetXInterval(const Point& p, int block, int Acols) const
	{
		long N = grid;
	
		Point DAA = MaxAffectedArea();
		double dx = DAA.x;
		
		long dxi = long(dx+2);
		long minX = std::max(0L,long(p.x)-dxi);
		long maxX = std::min(N, long(p.x)+dxi);
		
		
		long offset = block*num_cols_per_block;
		
		long mxX = offset/N - 1;
		if (mxX > minX)
			minX = mxX;
		
		long MxX = (Acols+offset)/N + 1;
		if (MxX < maxX)
			maxX = MxX;
		
		return {minX,maxX};
	}
	
	Interval GetYInterval(const Point& p, int block, int Acols, long x) const
	{
		long N = grid;
	
		Point DAA = MaxAffectedArea();
		double dy = DAA.y;
		
		long dyi = long(dy+2);
		
		long minY = std::max(0L,long(p.y)-dyi);
		long maxY = std::min(N, long(p.y)+dyi);
		long my = minY;
		long My = maxY;
		
		long offset = block*num_cols_per_block;
		long myY = offset-x*N;
		if (myY > my)
			my = myY;
		
		long MyY = Acols+offset-x*N;
		if (MyY < My)
			My = MyY;
		return {my,My};
	}
	
private:
	void Normalize();
	void SetBlockSize(long memoryAvailable);
	virtual void Realize(Mat& A, long row, long block) = 0;
	
	virtual void UpdateArea(const Mat& A, size_t species);

	Mat DivideByArea(const Mat& M) const;

protected: // variables
	std::vector<double> Area;	
	size_t grid;

	double bx {0}; //border in continuum
	double by {0}; 
	
	Point O {0,0}; //lower-left corner
	Point W {0,0}; //upper-right corner
	Point F {0,0}; // F = W-O. Yeah, I use F often.

	std::vector<std::vector<Point>> E;
	
	size_t num_cols_per_block;
	size_t num_full_blocks;
	size_t num_partial_blocks {0}; //0 or 1
	size_t num_cols_partial_block {0};
};

template <class Mat>
void FuzzyNetworkBase<Mat>::SetBlockSize(long memoryAvailable)
{
	std::cout << std::endl << "***************************** System Information *****************************" << std::endl;

	auto numspecies = E.size();
	size_t memoryPerColumn = numspecies*sizeof(double)  + 24; //24 is just a number I came up with. Not based on reality or anything really. It's probably less, but whatever.
	
	size_t numberOfColumns = grid*grid;
	size_t memoryNeeded = numberOfColumns*memoryPerColumn;
	size_t total_columns = grid*grid;
	
	num_cols_per_block = std::min(memoryAvailable/memoryPerColumn,total_columns);
	num_full_blocks = total_columns/num_cols_per_block;
	num_cols_partial_block = total_columns%num_cols_per_block;
	if (num_cols_partial_block != 0)
		num_partial_blocks = 1;
	
	std::cout << "Total memory (if everything was put on memory): " << double(memoryNeeded)/GB << "GB\n";
	std::cout << "Available memory: " << double(memoryAvailable)/GB << "GB\n";
	std::cout << "Total number of columns: " << total_columns << '\n'; 
	std::cout << "So I need " << num_full_blocks << " full blocks and " << num_partial_blocks << " partial blocks.\n";
	
	std::cout << "Full blocks have " << num_cols_per_block << " columns and consume " 
		 << double(num_cols_per_block*memoryPerColumn)/GB << "GB of memory.\n";
		 
	std::cout << "Partial block has " << num_cols_partial_block << " columns and consumes " 
		 << double(num_cols_partial_block*memoryPerColumn)/GB << "GB of memory.\n";
		 
	std::cout << "******************************************************************\n\n" << std::flush;
}

template <class Mat>
void FuzzyNetworkBase<Mat>::Normalize()
{
	Point B = CalculateBorder();
	bx = B.x;
	by = B.y;
	
	double minX = inf;
	double maxX = -inf;
	double minY = inf;
	double maxY = -inf;
	
	for (const auto& u : E)
	{
		for (const Point& p : u)
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
	
	for (auto& u : E)
	{
		for (auto& P : u)
		{
			P = ContinuumToGrid(P);
		}
	}
}

template <class Mat>
void FuzzyNetworkBase<Mat>::printAreaVector(std::ostream& os, char sep)
{
	os << GetTotalArea(0);
	for (size_t i = 1; i < num_species(); ++i)
	{
		os << sep << GetTotalArea(i);
	}
	os << std::endl;
}


template <class Mat>
Mat FuzzyNetworkBase<Mat>::CalculateGraph()
{
	PreInitialize();
	
	Normalize();
	
	PostInitialize();
	Chronometer FromStart;
	
	size_t numspecies = E.size();
	size_t num_blocks = num_full_blocks + num_partial_blocks;

	Mat A(numspecies,num_cols_per_block);
	
	Mat M(numspecies,numspecies);
	
	for (size_t block = 0; block < num_full_blocks + num_partial_blocks; ++block)
	{
		Chronometer BlockTimer;
		std::cout << "\nStarting block " << block+1 << std::endl;
		if (block == num_full_blocks)
			A.resize(numspecies,num_cols_partial_block);
		
		ResetFullMatrix(A);
		
		std::cout << "\tTook " << BlockTimer.Reset() << "s to initialize matrix" << std::endl;
		
		#pragma omp parallel for
		for (size_t species = 0; species < numspecies; ++species)
		{
			Realize(A,species, block);
			UpdateArea(A,species);
		}
		
// 		for (size_t species = 0; species < numspecies; ++species)
// 		{
// 			std::cout << species << " matrix is: \n";
// 			printRowAsMatrix(A,species, grid);
// 			std::cout << std::endl;
// 		}
		
		std::cout << "\t Block " << block+1 << " took " << BlockTimer.Reset() << " to realize." << std::endl;
		
		// M += A*A^T
		AddTransposeProduct(M,A);

		std::cout << "\t And " << BlockTimer.Reset() << "s to multiply the matrices.\n";
		
		double running_time = FromStart.Peek();
		double total_time = running_time*double(num_blocks)/(block+1);
		
		std::cout << "Done with block " << block+1 << " of " << num_blocks 
			<< ". Expected remaining time: " << total_time - running_time << 's' << std::endl;
	}
	
	std::cout << "Total Time taken: " << FromStart.Reset() << std::endl;
	

	return DivideByArea(M);
		
}

template <class Mat>
void FuzzyNetworkBase<Mat>::UpdateArea(const Mat& A, size_t species)
{
	for (long col = 0; col < A.cols(); ++col)
	{
		Area[species] += A(species,col);
	}
// 	std::cout << "After area update, Area = " << Area << std::endl;
}

template <class Mat>
Mat FuzzyNetworkBase<Mat>::DivideByArea(const Mat& M) const
{
	auto numspecies = num_species();
	
	Mat R(numspecies,numspecies);
	for (size_t x = 0; x < numspecies; ++x)
	{
		if (Area[x] <= tolerance)
		{
			std::cerr << "Area of X = 0! Increase grid size!" << std::endl;
		} else
		{
			for (size_t y = 0; y < numspecies; ++y)
			{
				if (Area[x] > tolerance)
				{
					R(x,y) = M(x,y)/Area[x];
				}
				else
				{
					R(x,y) = M(x,y);
				}
			}
		}
	}

	return R;
}

template <class Mat>
void FuzzyNetworkBase<Mat>::PrintEverything(const std::vector<std::string>& names, const std::string& sageoutfile)
{
	auto M = CalculateGraph();
			
	std::cout << std::endl << "Adjacency Matrix: " << std::endl;
	std::cout << M << std::endl;

	DiGraph D = DiGraph::FromAdjacencyMatrix(M);
	
	auto edges = D.edges();


	auto by_weight = [](const Edge & a, const Edge & b)
	{
		return a.weight() < b.weight();
	};
	
	sort(edges.begin(), edges.end(), by_weight);
			
	for (const auto& e : edges)
	{
		std::cout << '\"' << names[e.from] << "\" ---> \"" << names[e.to]  << '\"' << " with weight " << e.weight() << std::endl;
	}
	
	if (sageoutfile != "")
	{
		std::ofstream out(sageoutfile);
		out << "G = DiGraph()\n";
		for (auto e : edges)
		{
			out << "G.add_edge(\"" << names[e.from] << "\",\"" << names[e.to]  << '\"' << "," << e.weight() << ")\n";
		}
	}

	
	std::cout << "Areas: ";
	printAreaVector(std::cout, ',');
}
