#pragma once

#include "FuzzyNetworkBase.hpp"

template <class Mat, class Derived>
class FuzzyNetworkExpDecay : public FuzzyNetworkBase<Mat, FuzzyNetworkExpDecay<Mat,Derived>>
{
    using Base = FuzzyNetworkBase<Mat,FuzzyNetworkExpDecay<Mat,Derived>>;
public:
	Point CalculateBorder() const
	{
        Point B;
        B.x = MaxNonZeroDistance(Cx);
        B.y = MaxNonZeroDistance(Cy);
        return B;
    }

	void PreInitialize()
    {
        Cx = 1/(2*sigma*sigma);
        Cy = Cx;
    }
    
	void PostInitialize()
    {
        // fucking c++!
        const auto& F = Base::F;
        const auto& grid = Base::grid;
        
        Cnx = Cx*F.x*F.x;
        Cny = Cy*F.y*F.y;
        
        Cmx = Cnx/(grid*grid);
        Cmy = Cny/(grid*grid);
    }
	
	void Realize(Mat& A, long species, long block)
    {
        //fucking c++
        const auto& E = Base::E;
        const auto& grid = Base::grid;
        const auto& num_cols_per_block = Base::num_cols_per_block;
        
        const std::vector<Point>& P = E[species];
        long N = grid;
        
        for (const Point& p : P)
        {
            auto X = Base::GetXInterval(p,block, A.cols());
            
            long offset = block*num_cols_per_block;
            
            for (long x = X.L; x < X.R; ++x)
            {
                auto Y = Base::GetYInterval(p,block,A.cols(),x);
                
                for (long y = Y.L; y < Y.R; ++y)
                {
                    long index = x*N+y - offset;
                    
                    UpdateFunction(A(species,index),p,x,y,species);
                }
            }
        }
        PostRealizeAction(A,species);
    }

	inline void UpdateFunction(double& a, const Point& p, long x, long y, int species)
    {
        this->underlying().UpdateFunction(a,p,x,y,species);
    }
    
	void PostRealizeAction(Mat& A, long species) 
    {
        this->underlying().PostRealizeAction(A,species);
    }

    void ResetFullMatrix(Mat& A) const
	{
		this->underlying().ResetFullMatrix(A);
	}
    
protected:
	double Cx {1}; //in continuum
	double Cy {1};
	
	double Cnx {1}; // in normalized continuum
	double Cny {1};
	
	double Cmx {1}; //in grid
	double Cmy {1};
	double sigma;
private:
    
    //CRPT trick
    FuzzyNetworkExpDecay(size_t _grid, 
							const std::vector< std::vector< Point > >& U, 
							size_t memoryAvailable, 
							double VisibilityRangeInKm)
    	:	Base(_grid,U,memoryAvailable),
            sigma(VisibilityRangeInKm)
    {
        
    }
    friend Derived;
    
    inline Derived& underlying() { return static_cast<Derived&>(*this); }
    inline Derived const& underlying() const { return static_cast<Derived const&>(*this); }
};
