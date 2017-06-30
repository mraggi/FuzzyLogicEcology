#include <iomanip>
#include "GraphCalculator.hpp"
#include "Mu.hpp"

Matrix GraphCalculator::CalculateGraph()
{
	int numspecies = E.size();
	Matrix M(numspecies,Row(numspecies,1.0));

	vector<Mu> X;
	Chronometer T;
	for (int i = 0; i < numspecies; ++i)
	{
		X.emplace_back(grid);
		X[i].Realize(E[i],Cmx,Cmy);
		cout << "Realizing mu_" << i << " took " << T.Reset() << 's' << endl;
	}
	
	Row Area(numspecies,0);
	for (int i = 0; i < numspecies; ++i)
	{
		Area[i] = X[i].Integrate();
		cout << setprecision(2);
		cout << "\tArea " << i << " = " << Area[i] << endl;
		cout << "Integrating mu_" << i << " took " << T.Reset() << 's' << endl;

	}
	
	Chronometer C;
	
	int total = (numspecies*(numspecies-1))/2;
	int num = 0;
	for (int i = 0; i < numspecies; ++i)
	{
// 		cout << i << ": " << X[i] << endl;
		double areai = Area[i];
		for (int j = i+1; j < numspecies; ++j)
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
			cout << "Estimated time: " << t*total/num - t << "s" << endl;
			
		}
	}
	return M;
}

