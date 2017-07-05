#include "GraphCalculator.hpp"

bool Test11()
{
	int grid = 5000;
	vector<vector<Point>> P;
	P.push_back({Point(0,0)});
	P.push_back({Point(0,1)});
	
	double C = 0.2;
	double sigma = sqrt(1/(2*C));
	
	GraphCalculator GC(grid, sigma, P, 4*GB);
	
	auto M = GC.CalculateGraph();
	
	if (abs(M[0][0] - 0.5) > 0.001)
		return false;
	double expectedvalue = exp(-C/2)/2;

	if (abs(M[0][1] - expectedvalue) > 0.001)
		return false;
	
	if (abs(GC.GetTotalArea(0)-pi/C) > 0.001)
		return false;
	
	return true;
	
}

int main()
{
	if (Test11())
	{
		cout << "All tests passed!" << endl;
		return 0;
	}
	cout << "Some tests failed!" << endl;
	return 1;
}
