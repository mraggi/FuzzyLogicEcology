#include <iomanip>
#include "GraphCalculator.hpp"
#include <blaze/Math.h>
#include <blaze/config/BLAS.h>

using MatrixXd = blaze::DynamicMatrix<double>;

void Realize(MatrixXd& A, const vector<Point>& P, long row, long N, double Cmx, double Cmy, long block, long num_cols_per_block)
{
	double dx = MaxNonZeroDistance(Cmx);
	double dy = MaxNonZeroDistance(Cmy);
	long dxi = long(dx+1);
	long dyi = long(dy+1);
	
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
		for (long x = minX; x < maxX; ++x)
		{
			for (long y = minY; y < maxY; ++y)
			{
				long index = x*N+y;
				index -= num_cols_per_block*block;
                
				if (index >= 0 && index < A.columns())
				{
					double XX = (p.x-x)*(p.x-x);
					double YY = (p.y-y)*(p.y-y);
                    
					A(row,index) *= (1.0-exp(-Cmx*XX - Cmy*YY));
				}
			}
		}
	}
	
	for (size_t col = 0; col < A.columns(); ++col)
	{
		A(row,col) += 1.0;
	}
}


Matrix GraphCalculator::CalculateGraph()
{
	cout << "Using: " << blaze::getNumThreads() << " threads" << endl;
	blaze::setNumThreads( blaze::getNumThreads() );
	Chronometer T;
	size_t numspecies = E.size();
	vector<double> Area(numspecies,0.0);
	MatrixXd M(numspecies,numspecies,0.0);
	
	
	MatrixXd A(numspecies,num_cols_per_block,-1.0);
	for (size_t block = 0; block < num_full_blocks; ++block)
	{
		A = -1.0;
		#pragma omp parallel for
		for (size_t species = 0; species < numspecies; ++species)
		{
			Realize(A,E[species],species,grid,Cmx,Cmy, block, num_cols_per_block);
			
			for (size_t col = 0; col < A.columns(); ++col)
				Area[species] += A(species,col);
		}
		
		M += blaze::declsym( A * blaze::trans(A) );
        double time = T.Peek();
        
		cout << "Done with block " << block+1 << " of " << num_full_blocks+num_partial_blocks 
             << ". Expected remaining time: " << time*double(num_full_blocks+num_partial_blocks)/(block+1) - time << 's' << endl; 
	}
	
	if (num_partial_blocks)
	{
		MatrixXd A(numspecies,num_cols_partial_block,-1.0);
		#pragma omp parallel for
		for (size_t species = 0; species < numspecies; ++species)
		{
			Realize(A,E[species],species,grid,Cmx,Cmy, num_full_blocks, num_cols_per_block);
			
			for (size_t col = 0; col < A.columns(); ++col)
				Area[species] += A(species,col);
		}
		
		M += blaze::declsym( A * blaze::trans(A) );
		cout << "Done with block " << num_full_blocks+1 << " of " << num_full_blocks+num_partial_blocks << '!' << endl; 
	}
	
	cout << "Total Time taken: " << T.Reset() << endl;
	
	Matrix R(numspecies,Row(numspecies));
	for (size_t x = 0; x < numspecies; ++x)
	{
		if (Area[x] <= tolerance)
		{
			cerr << "Area of X = 0! Increase grid size!!" << endl;
		}
		for (size_t y = 0; y < numspecies; ++y)
		{
			if (Area[x] > tolerance)
				R[x][y] = sqrt(M(x,y)/Area[x]);
			else
				R[x][y] = M(x,y);
		}
	}
	
	return R;
}
