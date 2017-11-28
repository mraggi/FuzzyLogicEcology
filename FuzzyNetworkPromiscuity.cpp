#include "FuzzyNetworkPromiscuity.hpp"
#include "PointGraphConverter.hpp"
	

void FuzzyNetworkPromiscuity::PreInitialize()
{
	for (size_t s = 0; s < num_species(); ++s)
	{
		m_spanningTree[s] = primm(CreateGraphFromPoints(E[s]));
		
		assert(m_spanningTree[s].size()+1 == E[s].size());
		
		double avglength = 0.0;
		for (const Edge& e : m_spanningTree[s])
		{
// 			std::cout << "Edge: " << e << std::endl;
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
// 	std::cout << "radiuses: " << m_radius << std::endl;
// 	std::cout << "max radius: " << max_radius << std::endl;
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

double FuzzyNetworkPromiscuity::get_distancesq_to_spanning_tree(const Point& P, long species) const
{
// 	if (E[species].size() < 2)
// 	{
// 		return 0;
// 	}
	double d = 999999999.0;
	
	Point p = GridToContinuum(P);
// 	Point p = P;
	
	for (const auto& t : m_spanningTree[species])
	{
		Point A = GridToContinuum(E[species][t.from]);
		Point B = GridToContinuum(E[species][t.to]);
		
		double r = distance_squared(p,Segment(A,B));
		
// 		std::cout << "Distance from " << p << " to segment " << A << "<->" << B << " is " << sqrt(r) << std::endl; 
		
// 		std::cout << "Distance from " << A << " to " << B << " is " << distance(A,B) << std::endl;
		
		if (r < d)
			d = r;
	}
	
	return d;
}

Interval FuzzyNetworkPromiscuity::GetXInterval(long species) const
{
	long m = grid;
	long M = 0;
	
	for (const auto& P : E[species])
	{
		if (P.x < m)
			m = P.x;
		if (P.x > M)
			M = P.x;
	}
	--m;
	++M;
	
// 	auto W = ContinuumToGrid(O+Point(m_radius[species],m_radius[species]));
// 	auto W = Point(m_radius[species],m_radius[species]);
	double r = grid*m_radius[species]/F.x;
	m -= r;
	M += r;
	
	if (m < 0)
		m = 0;
	
	if (M > static_cast<long>(grid))
		M = grid;
	
	return {m,M};
}

Interval FuzzyNetworkPromiscuity::GetYInterval(long species) const
{
	long m = grid;
	long M = 0;
	
	for (const auto& P : E[species])
	{
		if (P.y < m)
			m = P.y;
		if (P.y > M)
			M = P.y;
	}
	--m;
	++M;
	
// 	auto W = ContinuumToGrid(O+Point(m_radius[species],m_radius[species]));
// 	auto W = Point(m_radius[species],m_radius[species]);
	double r = grid*m_radius[species]/F.y;

	m -= r;
	M += r;
	
	if (m < 0)
		m = 0;
	
	if (M > static_cast<long>(grid))
		M = grid;
	
	return {m,M};
}

void FuzzyNetworkPromiscuity::Realize(Matrix& A, long species, long block)
{
	long N = grid;
	long offset = block*num_cols_per_block;
	
	
	auto X = GetXInterval(species);
	auto Y = GetYInterval(species);
		for (long x = X.L; x < X.R; ++x)
	{
		for (long y = Y.L; y < Y.R; ++y)
		{
			long index = x*N+y - offset;
			
			if (index < 0 || index >= A.cols())
				continue;
			
			double d2 = get_distancesq_to_spanning_tree(Point(x,y),species);
			
			double r = m_radius[species];

			if (d2 < r*r)
				A(species,index) = 1.0;
			else
				A(species,index) = 0.0;
		}
	}
	
// 	long r = MaxAffectedArea().x+1.0;
// 	
// 	for (const auto& t : m_spanningTree[species])
// 	{
// 		Point GA = E[species][t.from];
// 		Point GB = E[species][t.to];
// 		
// 		Segment S(GA,GB);
// 		
// 		Point CA = GridToContinuum(A);
// 		Point CB = GridToContinuum(B);
// 		
// 		for (long x = S.minX()-r+1; x < X.maxX()+r; ++x)
// 		{
// 			double Y;
// 			
// 			if (x > S.maxX())
// 				Y = , entonces Y = la y del vértice correspondiente (dpende de la inclinación? fuck)
// 			
// 			if x < S.minX, entonces Y 
// 			
// 			long y = Y;
// 			
// 			long index = x*N-offset;
// 
// 			while (y < N && distance(Point(x,y),segmento) < r)
// 			{
// 				A(species,index) = 1.0;
// 				++y;
// 			}
// 			
// 			y = Y;
// 			--y;
// 			
// 			while (y >= 0 &&  distance(Point(x,y),segmento) < r)
// 			{
// 				A(species,index) = 1.0;
// 				--y;
// 			}
// 		}
// 	}
	

	
}

