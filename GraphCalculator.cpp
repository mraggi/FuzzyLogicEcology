#include "GraphCalculator.hpp"
#include <iomanip>

Matrix GraphCalculator::CalculateGraph()
{
	int numespecies = E.size();
	Matrix M(numespecies,Row(numespecies,1));

	vector<Mu> X;
	Chronometer T;

	for (int i = 0; i < numespecies; ++i)
	{
		X.emplace_back(malla);
		X[i].Realize(E[i],Cmx,Cmy);
// 		cout << "Para la mu_" << i << " tardé " << T.Reset() << 's' << endl;
	}
	
	Row Area(numespecies,0);
	for (int i = 0; i < numespecies; ++i)
	{
		Area[i] = X[i].Integrate();
		cout << setprecision(12);
		cout << "\tArea " << i << " = " << Area[i] << endl;
	}
	
	for (int i = 0; i < numespecies; ++i)
	{
// 		cout << i << ": " << X[i] << endl;
		real areai = Area[i];
		for (int j = i+1; j < numespecies; ++j)
		{
			real overlap = (X[i]*X[j]).Integrate();
			real areaj = Area[j];
			if (areai != 0.0)
				M[i][j] = overlap/areai;
			if (areaj != 0.0)
				M[j][i] = overlap/areaj;
			if (M[i][j] < 0.001)
				M[i][j] = 0.0;
			if (M[j][i] < 0.001)
				M[j][i] = 0.0;
		}
	}
	return M;
}

vector<vector<Point>> GraphCalculator::Normalized(const vector<vector<Point>>& U)
{
	real minX = 9999999999;
	real maxX = -9999999999;
	real minY = 9999999999;
	real maxY = -9999999999;
	
	for (auto& u : U)
	{
		
		for (auto p : u)
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
	O = Point(minX-bx,minY-by);
	W = Point(maxX+bx,maxY+by);
	F = W-O;
	cout << "O = " << O << endl;
	cout << "W = " << W << endl;
	cout << "En km, F = " << F << endl;
	
	Cnx = Cx*F.x*F.x;
	Cny = Cx*F.y*F.y;
	
	cout << "Cnx = " << Cnx << endl;
	
	Cmx = Cnx/(malla*malla);
	Cmy = Cny/(malla*malla);
	
	cout << "Cmx = " << Cmx << endl;
// 	cout << "O = " << O << " y W = " << W << endl;
	
// 	cout << "bordeEnContinuo = " << bordeEnContinuo << endl;
// 	cout << "b = " << b << endl;
	
	vector<vector<Point>> Q(U.size());
	int i = 0;
	for (auto& u : U)
	{
		for (auto& P : u)
		{
			
			Point S = (P-O);
			
			cout << S << endl;
			
			S.Scale(1.0/F);
			S *= malla;
			Q[i].emplace_back(S);
		}
		++i;
		cout << "---------------------------------" << endl;
	}
	return Q;
}