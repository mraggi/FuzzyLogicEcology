#include <iostream>
#include <iomanip>

#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Mu.hpp"
#include "GraphCalculator.hpp"
#include "DiGraph.hpp"

int malla = 5000;
double visibilidad = 1;

int main() 
{
	srand(time(NULL));

	auto R = ReadTableFromSTDIN("QuercusOaxaca.txt");
	
	auto U = QuercusExtractLocations(R);
	int i = 0;
	for (auto u : U)
	{
		cout << "# " << i << '|' << u.first << "|" << endl;
		++i;
	}
	
	vector<string> todas;
	for (auto& u : U)
	{
		todas.emplace_back(u.first);
	}
	
	string nombre1 = todas[rand()%todas.size()];
	string nombre2 = todas[rand()%todas.size()];
	
	cout << nombre1 << " vs " << nombre2 << endl;
	
	vector<vector<Point>> E;
	vector<string> names;
	E.reserve(U.size());
	for (auto& u : U)
	{
		
		if (u.first == nombre1 || u.first == nombre2)
		{
			E.emplace_back(u.second);
			names.emplace_back(u.first);
		}
	}
	
	Chronometer total;
	Chronometer T;
	
	for (double visibilidad = 0.05; visibilidad < 1.5; visibilidad += 0.01)
	{
		GraphCalculator GC(malla,visibilidad,E);
		int numespecies = U.size();
		
		
		
		cout << setprecision(4) << std::fixed;
		
		auto M = GC.CalculateGraph();
// 		cout << M << endl;

		DiGraph D = DiGraph::FromAdjacencyMatrix(M,0.0005);
		D.set_names(names);
		
		auto edges = D.edges();
		
		sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b)
		{
			return a.weight() < b.weight();
		});
// 		for (auto e : edges)
// 			cout << '\"' << D.get_name(e.from) << "\" ---> \"" << D.get_name(e.to)  << '\"' << " with weight " << e.weight() << endl;

		double w = 0.0;
		if (edges.size() > 0)
			w = edges[1].weight();
		
		
		cout << "(" << visibilidad << "," << w << ")" << endl;
	}
	
	cout << "Total time: " << T.Peek() << endl;
		
	cout << "Malla: " << malla << endl;
	
    return 0;
}
