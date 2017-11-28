#pragma once

#include <cassert>
#include <fstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <vector>

#include <png++/png.hpp>

#include "Graph.hpp"
#include "MatrixUtils.hpp"
#include "Point.hpp"
#include "TimeHelpers.hpp"

struct Interval
{
	Interval(long l, long r) : L(l), R(r) {}
	long L;
	long R;
};

template <class Matrix_t, class Derived>
class FuzzyNetworkBase
{
public:
	
	
	void PrintEverything(const std::vector<std::string>& names, const std::string& sageoutfile);
	
	Matrix_t CalculateGraph();
	
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
	
	inline std::pair<long,long> IndexToMatrix(long index, long block = 0)
	{
		long offset = block*num_cols_per_block;
		long x = (index+offset)/grid;
		long y = index+offset-grid*x;
		return {x,y};
	}
	
	inline long MatrixToIndex(long x, long y, long block = 0)
	{
		long offset = block*num_cols_per_block;
		return x*grid+y - offset;
	}
	
	void SetImagesToSave(const std::map<int,std::string>& M);

protected:
	
	void ResetFullMatrix(Matrix_t& A) const { this->underlying().ResetFullMatrix(A);  }
	
	void PreInitialize() { this->underlying().PreInitialize(); }
    
    
	void PostInitialize() { this->underlying().PostInitialize(); }
    

	Point CalculateBorder() const { return this->underlying().CalculateBorder(); }

	Point MaxAffectedArea() const
	{
		return CalculateBorder().Scaled(grid/F);
	}
	
	Interval GetXInterval(const Point& p, int block, int Acols) const
	{
		long N = grid;
	
		Point DAA = MaxAffectedArea();
		double dx = DAA.x;
		
		auto dxi = static_cast<long>(dx+2);
		auto minX = std::max(0L,long(p.x)-dxi);
		auto maxX = std::min(N, long(p.x)+dxi);
		
		
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
		
		auto dyi = static_cast<long>(dy+2);
		auto minY = std::max(0L,long(p.y)-dyi);
		auto maxY = std::min(N, long(p.y)+dyi);
		auto my = minY;
		auto My = maxY;
		
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
    
    Derived& underlying() { return static_cast<Derived&>(*this); }
    Derived const& underlying() const { return static_cast<Derived const&>(*this); }

    
	void Normalize();
	void SetBlockSize(long memoryAvailable);
	void Realize(Matrix_t& A, long species, long block) { this->underlying().Realize(A,species,block); }
	
	virtual void UpdateArea(const Matrix_t& A, size_t species);

	Matrix_t DividedByArea(const Matrix_t& M) const;

	void SaveToImages(Matrix_t& A, long block);
	void SaveImagesToDisk();
	
protected: // variables
	std::vector<double> Area;	
	size_t grid;

	double bx {0}; //border in continuum
	double by {0}; 
	
	Point O {0,0}; //lower-left corner
	Point W {0,0}; //upper-right corner
	Point F {0,0}; // F = W-O. Yeah, I use F often.

	std::vector<std::vector<Point>> E;
	
	size_t num_cols_per_block {1};
	size_t num_full_blocks {1};
	size_t num_partial_blocks {0}; //0 or 1
	size_t num_cols_partial_block {0};
	
	using image = png::image<png::rgba_pixel>;
	
	std::map<int,std::string> m_speciestoimage{};
	std::vector<image> m_images{};
	
private:
    FuzzyNetworkBase(size_t _grid, const std::vector<std::vector<Point>>& U, size_t memoryAvailable) : 
		Area(U.size(),0.0)
		, grid(_grid)
		, E(U)
	{
		SetBlockSize(memoryAvailable);
// 		std::cout << "At beginning, Area = " << Area << std::endl;
	}
	friend Derived;
};

template <class Matrix_t, class Derived>
void FuzzyNetworkBase<Matrix_t,Derived>::SetImagesToSave(const std::map<int,std::string>& M)
{
	m_speciestoimage = M;
	m_images = std::move(std::vector<image>(M.size(), image(grid,grid)));
}

template <class Matrix_t, class Derived>
void FuzzyNetworkBase<Matrix_t,Derived>::SetBlockSize(long memoryAvailable)
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

template <class Matrix_t, class Derived>
void FuzzyNetworkBase<Matrix_t,Derived>::Normalize()
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
	
	double maxrange = std::max(maxX - minX + 2*bx, maxY - minY + 2*by);
	
	O = Point(minX-bx,minY-by);
// 	W = Point(maxX+bx,maxY+by);
	W = O + Point(maxrange,maxrange);
	F = W-O;
	std::cout << std::setprecision(9) << std::endl;
	std::cout << "minx,miny = " << O/KmInADegree << std::endl;
	std::cout << "maxx,maxy = " << W/KmInADegree << std::endl;
	std::cout << "pixel size: " << (F.x/KmInADegree)/grid << std::endl;
	std::cout << std::setprecision(3) << std::endl;
	
	for (auto& u : E)
	{
		for (auto& P : u)
		{
			P = ContinuumToGrid(P);
		}
	}
}

template <class Matrix_t, class Derived>
void FuzzyNetworkBase<Matrix_t,Derived>::printAreaVector(std::ostream& os, char sep)
{
	if (num_species() == 0)
	{
		os << std::endl;
		return;
	}
	
	os << GetTotalArea(0);
	for (size_t i = 1; i < num_species(); ++i)
	{
		os << sep << GetTotalArea(i);
	}
	os << std::endl;
}

const long species_to_save = 0;

template <class Matrix_t, class Derived>
Matrix_t FuzzyNetworkBase<Matrix_t, Derived>::CalculateGraph()
{
	Chronometer FromStart;

	PreInitialize();
	
	Normalize();
	
	PostInitialize();
	
	size_t numspecies = num_species();
	
	size_t num_blocks = num_full_blocks + num_partial_blocks;

	Matrix_t A(numspecies,num_cols_per_block);
	
	Matrix_t M = Matrix_t::Zero(numspecies, numspecies);
	
	std::cout << "Took " << FromStart.Peek() << "s to initialize.\nStarting calculations.\n";
	
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

		SaveToImages(A,block);
		
		if (grid < 100)
		{
			for (size_t species = 0; species < numspecies; ++species)
			{
				std::cout << species << " matrix is: \n";
				printRowAsMatrix(A,species, grid);
				std::cout << std::endl;
			}
		}
		
		std::cout << "\tBlock " << block+1 << " took " << BlockTimer.Reset() << "s to realize," << std::endl;
		
		// M += A*A^T
		AddTransposeProduct(M,A);

		std::cout << "\tand " << BlockTimer.Reset() << "s to multiply the matrices.\n";
		
		double running_time = FromStart.Peek();
		double total_time = running_time*double(num_blocks)/(block+1);
		
		std::cout << "Done with block " << block+1 << " of " << num_blocks 
			<< ". Expected remaining time: " << total_time - running_time << 's' << std::endl;
	}
	
	SaveImagesToDisk();

	std::cout << "Total Time taken to calculate network: " << FromStart.Reset() << std::endl;
	

	return DividedByArea(M);
		
}

template <class Matrix_t, class Derived>
void FuzzyNetworkBase<Matrix_t, Derived>::SaveToImages(Matrix_t& A, long block)
{
	long i = 0;
	for (auto& m : m_speciestoimage)
	{
		long species = m.first;
		#pragma omp parallel for
		for (long index = 0; index < A.cols(); ++index)
		{
			auto p = IndexToMatrix(index,block);
			long x = p.first;
			long y = p.second;
			
			double a = A(species,index);
			auto c = static_cast<png::byte>(a*255.0);
			
			auto pixel = png::rgba_pixel(c,c,c);
			
			if (c == 4)
				pixel = png::rgba_pixel(c,150,c);
			if (c == 5)
				pixel = png::rgba_pixel(c,255,c);
			if (c == 6)
				pixel = png::rgba_pixel(c,200,c);
			
			m_images[i][x][y] = pixel;
		}
		++i;
	}
}

template <class Matrix_t, class Derived>
void FuzzyNetworkBase<Matrix_t, Derived>::SaveImagesToDisk()
{
	long num_image = 0; //needed because m_speciestoimage is a map and I need to traverse it! Hopefully not too large.

	for (const auto& m : m_speciestoimage)
	{
		long species = m.first;
		const auto& name = m.second;
		
		//Add points in red
		for (auto p : E[species])
		{
			long x = p.x;
			long y = p.y;
			long b = 6;

			for (int i = x-b; i <= x+b; ++i)
			{
				for (int j = y-b; j <= y+b; ++j)
				{
					double d = distance_squared(Point(i,j),p);
					double t = std::min(255.0,255.0*d/(b*b));
					auto c = static_cast<png::byte>(t);
					
					auto oldblue = m_images[num_image][i][j].blue;
					auto oldgreen = m_images[num_image][i][j].green;

					m_images[num_image][i][j].blue = std::min(c,oldblue);
					m_images[num_image][i][j].green = std::min(c,oldgreen);
					
				}
			}
		}
		
		//
		
		m_images[num_image].write(name + ".png");
		
		++num_image;
	}
}

template <class Matrix_t, class Derived>
void FuzzyNetworkBase<Matrix_t, Derived>::UpdateArea(const Matrix_t& A, size_t species)
{
	for (long col = 0; col < A.cols(); ++col)
	{
		Area[species] += A(species,col);
	}
// 	std::cout << "After area update, Area = " << Area << std::endl;
}

template <class Matrix_t, class Derived>
Matrix_t FuzzyNetworkBase<Matrix_t, Derived>::DividedByArea(const Matrix_t& M) const
{
// 	std::cout << M << std::endl;
	auto numspecies = num_species();
	
	Matrix_t R(numspecies,numspecies);
	for (size_t x = 0; x < numspecies; ++x)
	{
		
		if (Area[x] <= tolerance)
		{
			std::cerr << "Area of X = 0! Increase grid size!" << std::endl;
		} 
		
        for (size_t y = 0; y < numspecies; ++y)
        {
            if (Area[x] > tolerance)
            {
                R(x,y) = M(x,y)/Area[x];
            }
            else
            {
                R(x,y) = 0.0;
            }
        }
		
	}

	return R;
}

template <class Matrix_t, class Derived>
void FuzzyNetworkBase<Matrix_t, Derived>::PrintEverything(const std::vector<std::string>& names, const std::string& sageoutfile)
{
	Matrix_t M = CalculateGraph();
	
	std::cout << std::setprecision(3) << std::fixed;

	std::cout << std::endl << "Adjacency Matrix: " << std::endl;
	std::cout << M << std::endl;

	DiGraph D = DiGraph::FromAdjacencyMatrix(M);
	
	auto edges = D.edges();

	auto by_weight = [](const Edge & a, const Edge & b)
	{
		return a.weight() < b.weight();
	};
	
	std::sort(edges.begin(), edges.end(), by_weight);
	
	std::cout << "\nThese are the edges (sorted by weight):\n";
	
	for (const auto& e : edges)
	{
		std::cout << '\"' << names[e.from] << R"(" ---> ")" << names[e.to]  << '\"' << " with weight " << e.weight() << std::endl;
	}
	
	if (sageoutfile != "")
	{
		std::ofstream out(sageoutfile);
		out << "G = DiGraph()\n";
		for (auto e : edges)
		{
			out << R"(G.add_edge(")" << names[e.from] << R"(",")" << names[e.to]  << '\"' << "," << e.weight() << ")\n";
		}
	}

	
	std::cout << "\nAreas: ";
	printAreaVector(std::cout, ',');
	
	std::cout << std::defaultfloat;
	std::cout << std::endl;
}
