#include <iostream>
#include <iomanip>

#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Mu.hpp"
#include "GraphCalculator.hpp"
#include "DiGraph.hpp"

int malla = 2000;
double visibilidad = 0.5;

int main() 
{
	auto R = ReadTableFromSTDIN("QuercusOaxaca.txt");
// 	auto R = ReadTableFromSTDIN("lobatae.txt");
// 	auto R = ReadTableFromSTDIN("redes_centro.txt");
	
	auto U = QuercusExtractLocations(R);
// 	auto U = LobataeExtractLocations(R);
// 	auto U = CentroExtractLocations(R);
	int i = 0;
	for (auto u : U)
	{
		cout << "# " << i << ' ' << u.first << endl;
		++i;
	}
	
	vector<vector<Point>> E;
	vector<string> names;
	E.reserve(U.size());
	for (auto& u : U)
	{
		E.emplace_back(u.second);
		names.emplace_back(u.first);
	}
	
	GraphCalculator GC(malla,visibilidad,E);
	int numespecies = U.size();
	
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

	cout << "Total time: " << total.Peek() << endl;
	
	cout << "Malla: " << malla << endl;
	cout << "Visibilidad: " << visibilidad << endl;
    return 0;
}
