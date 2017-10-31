#include <iomanip>
#include <cassert>
#include <fstream>

#include "GraphCalculator.hpp"

GraphCalculator::GraphCalculator(size_t _grid, double VisibilityRangeInKm, const vector<vector<Point>>& U, size_t memoryAvailable) : grid(_grid), Area(U.size(),0.0)
{
	double sigma = VisibilityRangeInKm;

	Cx = 1/(2*sigma*sigma);
	Cy = Cx;
	bx = MaxNonZeroDistance(Cx);
	by = MaxNonZeroDistance(Cy);
	
	E = U;
		
	Normalize(E);
	
	SetBlockSize(memoryAvailable);
	
}

void GraphCalculator::SetBlockSize(long memoryAvailable)
{
	cout << endl << "***************************** System Information *****************************" << endl;

#if USE_BLAZE
	blaze::setNumThreads( blaze::getNumThreads() );
	cout << "Using: " << blaze::getNumThreads() << " threads" << endl;
#endif
	
	auto numspecies = E.size();
	size_t memoryPerColumn = numspecies*sizeof(double)  + 24; //24 is just a number I came up with. Not based on reality or anything really. It's probably less, but whatever.
	
	size_t numberOfColumns = grid*grid;
	size_t memoryNeeded = numberOfColumns*memoryPerColumn;
	size_t total_columns = grid*grid;
	
	num_cols_per_block = min(memoryAvailable/memoryPerColumn,total_columns);
	num_full_blocks = total_columns/num_cols_per_block;
	num_cols_partial_block = total_columns%num_cols_per_block;
	if (num_cols_partial_block != 0)
		num_partial_blocks = 1;
	
	cout << "Total memory (if everything was put on memory): " << double(memoryNeeded)/GB << "GB" << '\n';
	cout << "Available memory: " << double(memoryAvailable)/GB << "GB\n";
	cout << "Total number of columns: " << total_columns << '\n'; 
	cout << "So I need " << num_full_blocks << " full blocks and " << num_partial_blocks << " partial blocks.\n";
	
	cout << "Full blocks have " << num_cols_per_block << " columns and consume " 
		 << double(num_cols_per_block*memoryPerColumn)/GB << "GB of memory.\n";
		 
	cout << "Partial block has " << num_cols_partial_block << " columns and consumes " 
		 << double(num_cols_partial_block*memoryPerColumn)/GB << "GB of memory.\n";
		 
	cout << "******************************************************************\n\n" << std::flush;
}



inline long num_columns(const MatrixXd& A)
{
#if USE_BLAZE
	return A.columns();
#elif USE_EIGEN
	return A.cols();
#endif
}

inline long num_rows(const MatrixXd& A)
{
	return A.rows(); 
	// I know it's the same in both libraries. Only here to be consistent with cols
}

void GraphCalculator::Realize(MatrixXd& A, long species, long block)
{
	const vector<Point>& P = E[species];
	long N = grid;
	double dx = MaxNonZeroDistance(Cmx);
	double dy = MaxNonZeroDistance(Cmy);
	long dxi = long(dx+2);
	long dyi = long(dy+2);
	
// 	cout << "di = " << di << endl;
	
// 	cout << "Número de pixeles: " << 2*(dxi) << "*" << 2*dyi << " = " << 4*dxi*dyi << endl;
	
	for (const Point& p : P)
	{
		long minX = max(0L,long(p.x)-dxi);
		long maxX = min(N, long(p.x)+dxi);
		long minY = max(0L,long(p.y)-dyi);
		long maxY = min(N, long(p.y)+dyi);
		assert(maxX-minX > 0);
		assert(maxY-minY > 0);
		
		long offset = block*num_cols_per_block;
		
		long mxX = offset/N - 1;
		if (mxX > minX)
			minX = mxX;
		
		long MxX = (num_columns(A)+offset)/N + 1;
		if (MxX < maxX)
			maxX = MxX;
		
		
		
		for (long x = minX; x < maxX; ++x)
		{
			long my = minY;
			long My = maxY;
			
			long myY = offset-x*N;
			if (myY > my)
				my = myY;
			
			long MyY = num_columns(A)+offset-x*N;
			if (MyY < My)
				My = MyY;
			
			for (long y = my; y < My; ++y)
			{
				long index = x*N+y - offset;
				
				UpdateFunction(p,x,y,A(species,index),species);
				
			}
		}
	}
	
#if (!FUZZY_MIN)
	for (size_t col = 0; col < num_columns(A); ++col)
		A(species,col) += 1.0;
#endif
}

inline
void UpdateArea(vector<double>& Area, const MatrixXd& A, size_t species)
{
	for (size_t col = 0; col < num_columns(A); ++col)
	{
#if FUZZY_MIN
		Area[species] += A(species,col);
#else		
		Area[species] += A(species,col);
// 		Area[species] += A(species,col)*A(species,col);
#endif 
	}
}

void printRowAsMatrix(const MatrixXd& A, int row, int grid)
{
	for (int index = 0; index < num_columns(A); ++index)
	{
// 		int x = index/grid;
		int y = index%grid;
		if (y == 0)
			cout << endl;
		cout << A(row,index) << ' ';
	}
	cout << endl;
}

void printNonZeros(const MatrixXd& A)
{
	for (int x = 0; x < num_rows(A); ++x)
	{
		for (int y = 0; y < num_columns(A); ++y)
		{
			if (A(x,y) > scalar_min_t(0.001))
			{
				cout << "(" << x << "," << y << "): " << A(x,y) << '\n';
			}
		}
	}
}

template <class Mat>
inline void ResetMatrix(Mat& A)
{
#if FUZZY_MIN
	scalar_min_t startvalue = 0.0;
#else
	double startvalue = -1.0;
#endif

#if USE_BLAZE
	A = -1.0;
#endif
		
#if USE_EIGEN
	A = MatrixXd::Constant(A.rows(), A.cols(), startvalue);
#endif
}

// This does M += A*A^T
template <class Mat>
inline void AddTransposeProduct(Mat& M, Mat& A)
{
	#if FUZZY_MIN //Using Eigen, but in fuzzy-min mode
		M += A.lazyProduct(A.transpose());
	#elif USE_EIGEN 
		M += A*A.transpose();
	#elif USE_BLAZE 
		M += blaze::declsym( A * blaze::trans(A) );
	#endif
}

Matrix GraphCalculator::CalculateGraph()
{
	Chronometer FromStart;
	
	size_t numspecies = E.size();
	size_t num_blocks = num_full_blocks + num_partial_blocks;

	MatrixXd A(numspecies,num_cols_per_block);
	
	MatrixXd M(numspecies,numspecies);
	
	for (size_t block = 0; block < num_full_blocks + num_partial_blocks; ++block)
	{
		Chronometer BlockTimer;
		cout << "\nStarting block " << block+1 << endl;
		if (block == num_full_blocks)
			A.resize(numspecies,num_cols_partial_block);
		
		ResetMatrix(A);
		
		cout << "\tTook " << BlockTimer.Reset() << "s to initialize matrix to -1" << endl;
		
		#pragma omp parallel for
		for (size_t species = 0; species < numspecies; ++species)
		{
			Realize(A,species, block);
			UpdateArea(Area,A,species);
		}
		cout << "\t Block " << block+1 << " took " << BlockTimer.Reset() << " to realize." << endl;
		
		// M += A*A^T
		AddTransposeProduct(M,A);

		cout << "\t And " << BlockTimer.Reset() << "s to multiply the matrices.\n";
		
		double running_time = FromStart.Peek();
		double total_time = running_time*double(num_blocks)/(block+1);
		
		cout << "Done with block " << block+1 << " of " << num_blocks 
			 << ". Expected remaining time: " << total_time - running_time << 's' << endl;
	}
	
	cout << "Total Time taken: " << FromStart.Reset() << endl;
	

	return DivideByArea(M);
}

template<class Mat>
Matrix GraphCalculator::DivideByArea(const Mat& M) const
{
	auto numspecies = num_species();
	
	Matrix R(numspecies,Row(numspecies));
	for (size_t x = 0; x < numspecies; ++x)
	{
		if (Area[x] <= tolerance)
		{
			cerr << "Area of X = 0! Increase grid size!" << endl;
		}
		for (size_t y = 0; y < numspecies; ++y)
		{
			if (Area[x] > tolerance)
			{
				R[x][y] = M(x,y)/Area[x];
			}
			else
			{
				R[x][y] = 0.0;
			}
		}
	}

	return R;
}

double GraphCalculator::GetTotalArea(int species) const
{
	return Area[species]*F.x*F.y/(grid*grid);
}

void GraphCalculator::Normalize(vector<vector<Point>>& U)
{
	double minX = inf;
	double maxX = -inf;
	double minY = inf;
	double maxY = -inf;
	
	for (const auto& u : U)
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
	
	Cnx = Cx*F.x*F.x;
	Cny = Cy*F.y*F.y;
	
	Cmx = Cnx/(grid*grid);
	Cmy = Cny/(grid*grid);
	
	for (auto& u : U)
	{
		for (auto& P : u)
		{
			P -= O;
			P.Scale(1.0/F);
			P *= grid;
		}
	}
}

void GraphCalculator::printAreaVector(std::ostream& os, char sep)
{
	os << GetTotalArea(0);
	for (size_t i = 1; i < num_species(); ++i)
	{
		os << sep << GetTotalArea(i);
	}
	os << endl;
}


// void GraphCalculator::WriteArcGis(const string& prefix)
// {
//	 for (int sp = 0; sp < num_species(); ++sp)
//	 {
//		 string fn = filename + to_string(sp) + ".txt";
//		 
//		 std::ofstream ofs(fn, std::ifstream::out);
//		 ofs << "ncols " << grid << '\n';
//		 ofs << "nrows " << grid << '\n';
//		 ofs << "xllcorner " << O.x << '\n';
//		 ofs << "yllcorner " << O.y << '\n';
//	 }
// }

