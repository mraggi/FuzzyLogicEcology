#include "FuzzyNetworkProduct.hpp"

bool Test11()
{
    using std::vector;
	int grid = 5000;
	vector<vector<Point>> P;
	P.push_back({Point(0,0)});
	P.push_back({Point(0,1)});
	
	double C = 0.2;
	double sigma = sqrt(1/(2*C));
	
	FuzzyNetworkProduct GC(grid, sigma, P, 4*GB);
	
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
    using std::cout;
    using std::endl;
    
	Matrix A(2,2);
	A(0,0) = scalar_min_t(1);
	A(0,1) = scalar_min_t(2);
	A(1,0) = scalar_min_t(3);
	A(1,1) = scalar_min_t(4);
	cout << "A = " << endl << A << endl;

//	 cout << "A*A = " << endl << A.lazyProduct(A) << endl;
	return 0;
	
// 	if (Test11())
// 	{
// 		cout << "All tests passed!" << endl;
// 		return 0;
// 	}
// 	cout << "Some tests failed!" << endl;
// 	return 1;
}
