#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Utility.hpp"
#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Mu.hpp"
#include "GraphCalculator.hpp"
#include "argumentparser.hpp"
#include "edge.hpp"

int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
    cout << setprecision(3) << std::fixed;
    srand(time(NULL));
	try
	{
		Chronometer chrono;
		ArgumentParser AP(argc,argv);
		
		if (AP.should_exit)
			return 0;
		
		auto Table = ReadTable(*AP.is);
		auto SpeciesMap = ExtractLocations(Table,AP.NamedColumns,AP.latitude,AP.longitude);
		int numspecies = SpeciesMap.size();

		using SpeciesRegisters = pair<string, vector<Point>>;
		vector< SpeciesRegisters > V(SpeciesMap.begin(), SpeciesMap.end());
// 		sort(V.begin(), V.end(), [](const auto & L, const auto & R)
// 		{
// 			//Decreasing order by number of data points
// 			return L.second.size() > R.second.size();
// 		});
		
		cout << endl << "********************* Species Found *******************" << endl;
		int num = 0;
		for (const auto& v : V)
		{
			cout << "# "  << num << ' ' << v.first << " has " << v.second.size() << " observations \n";
            ++num;
		}
		cout << "***********************************************" << endl;
		
		vector<vector<Point>> Points;
		vector<string> names;
		Points.reserve(numspecies);
		names.reserve(numspecies);
		
		for (const auto& v : V)
		{
			Points.emplace_back(v.second);
			names.emplace_back(v.first);
		}
		
		GraphCalculator GC(AP.grid, AP.visibility, Points, AP.memoryAvailable);

		cout << "Done pre-processing in " << chrono.Peek() << "s. Starting calculation..." << endl;
		Matrix M = GC.CalculateGraph();
		cout << M << endl;

		cout << "Done Calculating Graph in " << chrono.Peek() << "s." << endl;
		cout  << endl << "------------------------------" << endl << endl;

		auto edges = FromAdjacencyMatrix(M);


		sort(edges.begin(), edges.end(), [](const Edge & a, const Edge & b)
		{
			return a.weight < b.weight;
		});
				
		for (const auto& e : edges)
		{
			cout << '\"' << names[e.from] << "\" ---> \"" << names[e.to]  << '\"' << " with weight " << e.weight << endl;
		}
		
		if (AP.outfile != "")
		{
			ofstream out(AP.outfile);
			out << "G = DiGraph()\n";
			for (auto e : edges)
			{
				out << "G.add_edge(\"" << names[e.from] << "\",\"" << names[e.to]  << '\"' << "," << e.weight << ")\n";
			}
		}

		cout << "Grid: " << AP.grid << endl;
		cout << "Visibility: " << AP.visibility << endl;

#if FUZZY_MIN
		cout << "USING EIGEN (FUZZY MIN mode)" << endl;
#elif USE_BLAZE
		cout << "USING BLAZE (FUZZY PRODUCT mode)" << endl;
#else
		cout << "USING EIGEN (FUZZY PRODUCT mode)" << endl;
#endif
		
		cout << endl << "Total time: " << chrono.Peek() << endl;
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
