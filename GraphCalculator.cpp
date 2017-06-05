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
		cout << "Para doubleizar la mu_" << i << " tardé " << T.Reset() << 's' << endl;
	}
	
	Row Area(numespecies,0);
	for (int i = 0; i < numespecies; ++i)
	{
		Area[i] = X[i].Integrate();
		cout << setprecision(2);
		cout << "\tArea " << i << " = " << Area[i] << endl;
		cout << "Para integrar la mu_" << i << " tardé " << T.Reset() << 's' << endl;

	}
	
	Chronometer C;
	
	int total = (numespecies*(numespecies-1))/2;
	int num = 0;
	for (int i = 0; i < numespecies; ++i)
	{
// 		cout << i << ": " << X[i] << endl;
		double areai = Area[i];
		for (int j = i+1; j < numespecies; ++j)
		{
			
			double overlap = (X[i]*X[j]).Integrate();
			double areaj = Area[j];
			if (areai != 0.0)
				M[i][j] = overlap/areai;
			if (areaj != 0.0)
				M[j][i] = overlap/areaj;
			if (M[i][j] < 0.001)
				M[i][j] = 0.0;
			if (M[j][i] < 0.001)
				M[j][i] = 0.0;
			
			++num;
			double t = C.Peek();
			cout << "Tiempo estimado: " << t*total/num - t << "s" << endl;
			
		}
	}
	return M;
}

vector<vector<Point>> GraphCalculator::Normalized(const vector<vector<Point>>& U)
{
	double minX = 9999999999;
	double maxX = -9999999999;
	double minY = 9999999999;
	double maxY = -9999999999;
	
	for (auto& u : U)
	{
		
		for (auto p : u)
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
// 	cout << "O = " << O << endl;
// 	cout << "W = " << W << endl;
// 	cout << "En km, F = " << F << endl;
	
	Cnx = Cx*F.x*F.x;
	Cny = Cx*F.y*F.y;
	
// 	cout << "Cnx = " << Cnx << endl;
	
	Cmx = Cnx/(malla*malla);
	Cmy = Cny/(malla*malla);
	
// 	cout << "Cmx = " << Cmx << endl;
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
			
// 			cout << S << endl;
			
			S.Scale(1.0/F);
			S *= malla;
			Q[i].emplace_back(S);
		}
		++i;
// 		cout << "---------------------------------" << endl;
	}
	return Q;
}
