#include <iostream>
#include <iomanip>

#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Mu.hpp"
#include "GraphCalculator.hpp"



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
	E.reserve(U.size());
	for (auto& u : U)
	{
		E.emplace_back(u.second);
	}
	
// 	for (auto& e : E)
// 	{
// 		for (auto& p : e)
// 		{
// 			p *= ValorDe1Grado;
// 		}
// 	}
	
	GraphCalculator G(1000,0.1,E);
	
	int numespecies = U.size();
	
	Chronometer total;
	srand(time(NULL));
	Chronometer T;
	
	cout << setprecision(2) << std::fixed;
	
	auto M = G.CalculateGraph();
	
	cout << M << endl;
	
	cout << "En calcular áreas me tardé " << T.Reset() << endl;

	cout << "Total time: " << total.Peek() << endl;
    return 0;
}
