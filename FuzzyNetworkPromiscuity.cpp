#include "FuzzyNetworkPromiscuity.hpp"
#include "PointGraphConverter.hpp"

FuzzyNetworkPromiscuity::FuzzyNetworkPromiscuity(size_t _grid, 
					const std::vector<std::vector<Point>>& U, 
					size_t memoryAvailable) :
			FuzzyNetworkBase<Matrix>(_grid,U,memoryAvailable),
			m_radius(U.size(),0.0),
			m_spanningTree(U.size())
{
	
}
	

void FuzzyNetworkPromiscuity::PreInitialize()
{
	for (size_t s = 0; s < num_species(); ++s)
	{
		m_spanningTree[s] = primm(CreateGraphFromPoints(E[s]));
		
		assert(m_spanningTree[s].size()+1 == E[s].size());
		
		double avglength = 0.0;
		for (const Edge& e : m_spanningTree[s])
		{
			avglength += e.weight();
		}
		avglength /= m_spanningTree[s].size();
		
		m_radius[s] = avglength;
	}
	
	max_radius = *std::max_element(m_radius.begin(), m_radius.end());
// 	std::cout << "radiuses: " << m_radius << std::endl;
// 	std::cout << "max radius: " << max_radius << std::endl;
}

void FuzzyNetworkPromiscuity::PostInitialize()
{
	//empty because all tree calculations should be done before normalizing
}

Point FuzzyNetworkPromiscuity::CalculateBorder() const
{
	return Point(max_radius,max_radius);
}

struct Segment
{
	Segment(const Point& a, const Point& b) : A(a), B(b) {}
	
	bool IsInBox(const Point& P) const
	{
		double minX = std::min(A.x,B.x);
		double maxX = std::max(A.x,B.x);
		double minY = std::min(A.y,B.y);
		double maxY = std::max(A.y,B.y);
		return P.x >= minX && P.x <= maxX && P.y >= minY && P.y <= maxY;
	}
	
	Point A;
	Point B;
};

double distance_squared(const Point& P, const Segment& S)
{
	auto Q = P.ProjectionToLine(S.A,S.B);
	
	if (S.IsInBox(Q))
		return distance_squared(P,Q);
	
	return std::min(distance_squared(P,S.A),distance_squared(P,S.B));
}

double FuzzyNetworkPromiscuity::get_distancesq_to_spanning_tree(const Point& P, long species)
{
	double d = 9999999999.0;
	
	Point p = GridToContinuum(P);
	
	for (const auto& t : m_spanningTree[species])
	{
		Point A = E[species][t.from];
		Point B = E[species][t.to];
		
		double r = distance_squared(p,Segment(A,B));
		if (r < d)
			d = r;
	}
	
	return d;
}

void FuzzyNetworkPromiscuity::Realize(Matrix& A, long species, long block)
{
	long N = grid;
	long offset = block*num_cols_per_block;
	
	for (long index = 0; index < A.cols(); ++index)
	{
		long x = (index+offset)/N;
		
		long y = index - x*N + offset;
		
		
		
		double d2 = get_distancesq_to_spanning_tree(Point(x,y),species);
// 		std::cout << "d2 = " << d2 << std::endl;
		double r = m_radius[species];
// 		r *= sqrt(grid/(F.x*F.y));
		if (d2 < r*r)
			A(species,index) = 1.0;
		
// 		for (auto p : E[species])
// 		{
// 			if (p.Distance(Point(x,y)) < 1)
// 			{
// 				A(species,index) = 5;
// 			}
// 		}
	}
	
	
}

