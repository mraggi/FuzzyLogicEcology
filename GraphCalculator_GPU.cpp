#include <iomanip>
#include "GraphCalculator.hpp"
#include <arrayfire.h>

GraphCalculator::GraphCalculator(size_t _grid, double VisibilityRangeInKm, const vector<vector<Point>>& U, size_t memoryAvailable) : grid(_grid)
{
	double sigma = VisibilityRangeInKm;

	Cx = 1/(2*sigma*sigma);
	Cy = Cx;
	bx = MaxNonZeroDistance(Cx);
	by = MaxNonZeroDistance(Cy);
	
	E = U;
	for (auto& conjDePuntos : E)
	{
		for (auto& punto : conjDePuntos)
		{
			punto *= KmInADegree;
		}
	}
		
	Normalize(E);
	
	SetBlockSize(memoryAvailable);
	
}

void GraphCalculator::SetBlockSize(long memoryAvailable)
{
	cout << endl << "***************************** System Information *****************************" << endl;
	blaze::setNumThreads( blaze::getNumThreads() );
	cout << "Using: " << blaze::getNumThreads() << " threads" << endl;
	
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
	
	cout << "Total memory (if everything was put on memory): " << double(memoryNeeded)/GB << "GB" << endl;
	cout << "Available memory: " << double(memoryAvailable)/GB << "GB" << endl;
	cout << "Total number of columns: " << total_columns << endl; 
	cout << "So I need " << num_full_blocks << " full blocks and " << num_partial_blocks << " partial blocks." << endl;
	
	cout << "Full blocks have " << num_cols_per_block << " columns and consume " 
		 << double(num_cols_per_block*memoryPerColumn)/GB << "GB of memory." << endl;
		 
	cout << "Partial blocks have " << num_cols_partial_block << " columns and consume " 
		 << double(num_cols_partial_block*memoryPerColumn)/GB << "GB of memory." << endl;
		 
	cout << "******************************************************************\n" << endl;
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
		
		long MxX = (A.columns()+offset)/N + 1;
		if (MxX < maxX)
			maxX = MxX;
		
		
		
		for (long x = minX; x < maxX; ++x)
		{
			long my = minY;
			long My = maxY;
			
			long myY = offset-x*N;
			if (myY > my)
				my = myY;
			
			long MyY = A.columns()+offset-x*N;
			if (MyY < My)
				My = MyY;
			
			for (long y = my; y < My; ++y)
			{
				long index = x*N+y - offset;
                
				double XX = (p.x-x)*(p.x-x);
				double YY = (p.y-y)*(p.y-y);
				
				A(species,index) *= (1.0-exp(-Cmx*XX - Cmy*YY));
			}
		}
	}
	
	for (size_t col = 0; col < A.columns(); ++col)
		A(species,col) += 1.0;
}

inline
void UpdateArea(vector<double>& Area, const MatrixXd& A, size_t species)
{
	for (size_t col = 0; col < A.columns(); ++col)
		Area[species] += A(species,col)*A(species,col);
}

Matrix GraphCalculator::CalculateGraph()
{
	Chronometer T;
	size_t numspecies = E.size();
	vector<double> Area(numspecies,0.0);
	
// #ifdef USE_GPU
// 	MatrixXd M(numspecies,numspecies,0.0);
	MatrixXd A(numspecies,num_cols_per_block);
	
	af::array AA(A.rows(), A.columns(),f64);
	af::array M(numspecies,numspecies);
	
	for (size_t block = 0; block < num_full_blocks; ++block)
	{
		Chronometer C;
		cout << endl << "Starting block " << block+1 << endl;
		A = -1.0;
		#pragma omp parallel for
		for (size_t species = 0; species < numspecies; ++species)
		{
			Realize(A,species, block);
			UpdateArea(Area,A,species);
		}
		cout << "\t Block " << block+1 << " took " << C.Reset() << " to realize." << endl;
// 		M += blaze::declsym( A * blaze::trans(A) );
		af::array AA(A.rows(), A.columns(), A.data());
		M += af::matmulNT(AA,AA);
		cout << "\t And " << C.Reset() << "s to multiply the matrices." << endl;
        double time = T.Peek();
		cout << "Done with block " << block+1 << " of " << num_full_blocks+num_partial_blocks 
             << ". Expected remaining time: " << time*double(num_full_blocks+num_partial_blocks)/(block+1) - time << 's' << endl; 
	}
	A.resize(numspecies,num_cols_partial_block,false);
	if (num_partial_blocks)
	{
		A = -1.0;
		#pragma omp parallel for 
		for (size_t species = 0; species < numspecies; ++species)
		{
			Realize(A,species,num_full_blocks);
			UpdateArea(Area,A,species);
		}
		
// 		M += blaze::declsym( A * blaze::trans(A) );
		
		af::array AA(A.rows(), A.columns(), A.data());
		M += af::matmulNT(AA,AA);
		
		cout << "Done with block " << num_full_blocks+1 << " of " << num_full_blocks+num_partial_blocks << '!' << endl; 
	}
	
	cout << "Total Time taken: " << T.Reset() << endl;
	
	double* MM = M.host<double>();
	
	Matrix R(numspecies,Row(numspecies));
	for (size_t x = 0; x < numspecies; ++x)
	{
		if (Area[x] <= tolerance)
		{
			cerr << "Area of X = 0! Increase grid size!!" << endl;
		}
		for (size_t y = 0; y < numspecies; ++y)
		{
			int index = x*grid + y;
			if (Area[x] > tolerance)
				R[x][y] = MM[index]/Area[x];
			else
				R[x][y] = MM[index];
		}
	}
	af::freeHost(MM);
	return R;
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
	Cny = Cx*F.y*F.y;
	
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
