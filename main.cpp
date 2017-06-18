#include <iostream>
#include <iomanip>

#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Mu.hpp"
#include "GraphCalculator.hpp"
#include "DiGraph.hpp"

int grid = 2000;
double visibilidad = 0.5;

int main() 
{
#ifdef USE_GPU
	cout << "Using following GPU: " << endl;
	af::info();
#else
	cout << "Using the CPU!" << endl;
#endif
	
	auto R = ReadTableFromSTDIN("QuercusOaxaca.txt");
// 	auto R = ReadTableFromSTDIN("lobatae.txt");
// 	auto R = ReadTableFromSTDIN("redes_centro.txt");
	
	auto U = QuercusExtractLocations(R);
// 	auto U = LobataeExtractLocations(R);
// 	auto U = CentroExtractLocations(R);
	
	
	vector< pair< string, vector<Point> > > V(U.begin(),U.end());
	sort(V.begin(),V.end(),[](const auto& R, const auto& L)
	{
		return R.second.size() > L.second.size();
	});
	int i = 0;
	for (auto v : V)
	{
		cout << "# " << i << ' ' << v.first << ": " << v.second.size() << endl;
		++i;
	}
	
	vector<vector<Point>> E;
	vector<string> names;
	E.reserve(V.size());
	for (auto& v : V)
	{
		E.emplace_back(v.second);
		names.emplace_back(v.first);
	}
	
	GraphCalculator GC(grid,visibilidad,E);
	int numspecies = U.size();
	
	Chronometer total;
	srand(time(NULL));
	Chronometer T;
	
	cout << setprecision(3) << std::fixed;
	
	auto M = GC.CalculateGraph();
	cout << M << endl;
	
	cout << "En calcular la matriz me tardé " << T.Reset() << endl;
	cout  << endl << "------------------------------" << endl << endl;

	DiGraph D = DiGraph::FromAdjacencyMatrix(M,0.005);
	D.set_names(names);
	
	auto edges = D.edges();
	
	sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b)
	{
		return a.weight() < b.weight();
	});
	for (auto e : edges)
		cout << '\"' << D.get_name(e.from) << "\" ---> \"" << D.get_name(e.to)  << '\"' << " with weight " << e.weight() << endl;

	
	cout << "Malla: " << grid << endl;
	cout << "Visibilidad: " << visibilidad << endl;

	cout << "Total time: " << total.Peek() << endl;
    return 0;
}
