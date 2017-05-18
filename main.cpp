#include <iostream>
#include <iomanip>

#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"

const real tolerance = 0.00001; //en el continuo original


inline real MaximaLongitudQueNoEs0(real C)
{
	return sqrt(-log(tolerance)/C);
}



const size_t malla = 80;
const real ConstanteEnContinuo = 2400000;

real bordeEnContinuo = MaximaLongitudQueNoEs0(ConstanteEnContinuo);
	
	
Point O;
Point W;
Point F;

real ConstanteEnNormalizado;
real b; // borde en normalizado
Point B;

real ConstanteEnMalla;
real bordeEnMalla;

template <class T> 
std::ostream& operator<<(std::ostream& os, const vector<T>& rhs)
{
	for (const auto& x : rhs)
		os << x << " ";
	return os;
}


using Row = vector<real>;
using Matrix = vector<Row>;

class Mu
{
public:
	Mu(size_t n) : m_M(n,Row(n,0))
	{ 
	}
	
	real operator()(size_t x, size_t y) const
	{
		return m_M[x][y];
	}
	real& operator()(size_t x, size_t y)
	{
		return m_M[x][y];
	}
	size_t numcols() const { return m_M.size(); }
	size_t numrows() const { return m_M.size(); }
	
	void Realize(const vector<Point>& P, real C);
	
	real Integrate() const;
	
private:
	
	
	Matrix m_M;
};

Mu operator+(const Mu& A, const Mu& B)
{
	Mu C(A.numrows());
	for (size_t x = 0; x < C.numcols(); ++x)
	{
		for (size_t y = 0; y < C.numrows(); ++y)
		{
			C(x,y) = A(x,y) + B(x,y) - A(x,y)*B(x,y);
		}
	}
	return C;
}

Mu operator*(const Mu& A, const Mu& B)
{
	Mu C(A.numrows());
	for (size_t x = 0; x < C.numcols(); ++x)
	{
		for (size_t y = 0; y < C.numrows(); ++y)
		{
			C(x,y) = A(x,y)*B(x,y);
		}
	}
	return C;
}

void Mu::Realize(const vector<Point>& P, real C)
{
	// TODO: Pasar lo de los puntos para afuera y dependiendo de C, ver qué debo modificar para cada punto.
	// Seguramente búsuqeda binaria
	
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			m_M[x][y] = -1;
		}
	}
	
	real d = MaximaLongitudQueNoEs0(C);
	long di = long(d+1);
// 	cout << "di = " << di << endl;
	for (const Point& p : P)
	{
		size_t minX = max(long(0),long(p.x)-di);
		size_t maxX = min(long(numcols()), long(p.x)+di);
		size_t minY = max(long(0),long(p.y)-di);
		size_t maxY = min(long(numrows()), long(p.y)+di);
		
		for (size_t x = minX; x < maxX; ++x)
		{
			for (size_t y = minY; y < maxY; ++y)
			{
				real d2 = p.DistanceSq(Point(x,y));
				m_M[x][y] *= (1-exp(-C*d2));
			}
		}
		
	}
	
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			m_M[x][y] += 1;
			if (abs(m_M[x][y]) < tolerance)
				m_M[x][y] = 0.0;
		}
	}
	
}

real Mu::Integrate() const
{
	real result = 0;
	for (size_t x = 0; x < numcols(); ++x)
	{
		for (size_t y = 0; y < numrows(); ++y)
		{
			result += m_M[x][y];
		}
	}
	return result/(malla*malla);
}

std::ostream& operator<<(std::ostream& os, const Mu& C)
{
	const size_t maxtoprint = 30;
	os << '\n';
	os << setprecision(3);
	for (size_t x = 0; x < min(C.numcols(),maxtoprint); ++x)
	{
		for (size_t y = 0; y < min(C.numrows(),maxtoprint); ++y)
		{
			os << C(x,y) << '\t';
		}
		os << '\n';
	}
	return os;
}

real random_real()
{
	int r = rand();
	return real(r)/RAND_MAX;
}

vector<Point> GenerateRandomPoints(int n, int resolution)
{
	vector<Point> result;
	
	result.reserve(n);
	
	Point Centro(random_real()*resolution, random_real()*resolution);
	cout << "point2d("<< Centro << ")+";
	while (result.size() < n)
	{
		Point P(random_real()*resolution, random_real()*resolution);
		if (P.Distance(Centro) < 1000)
			result.emplace_back(P);
	}
	
	return result;
}

Matrix CalculateGraph(const vector<vector<Point>>& E, real C)
{
	int numespecies = E.size();
	Matrix M(numespecies,Row(numespecies,1));

	vector<Mu> X;
	Chronometer T;
	for (int i = 0; i < numespecies; ++i)
	{
		X.emplace_back(malla);
		X[i].Realize(E[i],C);
		cout << "Para la mu_" << i << " tardé " << T.Reset() << 's' << endl;
// 		cout << E[i];
// 		cout << X[i] << endl;
	}
	
	Row Area(numespecies,0);
	for (int i = 0; i < numespecies; ++i)
		Area[i] = X[i].Integrate();
	
	for (int i = 0; i < numespecies; ++i)
	{
		real areai = Area[i];
		for (int j = i+1; j < numespecies; ++j)
		{
			real overlap = (X[i]*X[j]).Integrate();
			real areaj = Area[j];
			M[i][j] = overlap/areai;
			M[j][i] = overlap/areaj;
			if (M[i][j] < 0.01)
				M[i][j] = 0.0;
			if (M[j][i] < 0.01)
				M[j][i] = 0.0;
		}
	}
	return M;
}

vector<vector<Point>> Normalized(const vector<vector<Point>>& U)
{
	real minX = 9999999999;
	real maxX = -9999999999;
	real minY = 9999999999;
	real maxY = -9999999999;
	
	for (auto& u : U)
	{
		for (auto& p : u)
		{
			real x = p.x;
			real y = p.y;
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
	
	bordeEnContinuo = MaximaLongitudQueNoEs0(ConstanteEnContinuo);
	
	
	O = Point(minX,minY);
	W = Point(maxX,maxY);
	F = W-O;
	
	ConstanteEnNormalizado = ConstanteEnContinuo/F.LengthSq();
	
	b = bordeEnContinuo/F.Length();
	B = Point(b,b);
	
	ConstanteEnMalla = ConstanteEnNormalizado/(malla*malla);
	bordeEnMalla = b*malla;
	
	cout << "O = " << O << " y W = " << W << endl;
	
	cout << "bordeEnContinuo = " << bordeEnContinuo << endl;
	cout << "b = " << b << endl;
	
	vector<vector<Point>> E(U.size());
	int i = 0;
	for (auto& u : U)
	{
		for (auto& P : u)
		{
			Point S = (P-O);
			S.Scale(1.0/F);
			S += B;
			S *= malla;
			
			cout << "Entonces, " << P << "|->" << S << endl;
			
			E[i].emplace_back(S);
		}
		++i;
	}
	return E;
}

int main() 
{
// 	vector<Point> HH = {Point(10,10), Point(20,20)};
// 	vector<vector<Point>> RR = {HH};
// 	RR = Normalized(RR);
// 	Mu MM(malla);
// 	MM.Realize(RR[0],Constante);
// 	cout << MM << endl;
// 	
// 	return 0;
	
	auto R = ReadTableFromSTDIN("QuercusOaxaca.txt");
	
	auto U = QuercusExtractLocations(R);
	
	vector<vector<Point>> E;
	E.reserve(U.size());
	for (auto& u : U)
	{
		E.emplace_back(u.second);
	}
	
	E = Normalized(E);
	
	int i = 0;
	for (auto& u : U)
	{
		cout << u.first << " appears at " << E[i] << endl;
		++i;
	}
	
	int numespecies = U.size();
	
	Chronometer total;
	srand(time(NULL));
	Chronometer T;
	
	cout << setprecision(1) << std::fixed;
	
	
	
	auto M = CalculateGraph(E,ConstanteEnMalla);
	
	for (int i = 0; i < numespecies; ++i)
	{
		for (int j = 0; j < numespecies; ++j)
		{
			cout << 100*M[i][j] << '\t';
		}
		cout << endl;
	}
	
	cout << "En calcular áreas me tardé " << T.Reset() << endl;

	cout << "Total time: " << total.Peek() << endl;
    return 0;
}
