#include <iostream>
#include <iomanip>
#include <fstream>
#include <boost/program_options.hpp>

// using namespace boost;
namespace po = boost::program_options;

#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
#include "Mu.hpp"
#include "GraphCalculator.hpp"
#include "DiGraph.hpp"
#include "argumentparser.hpp"

int main(int argc, char* argv[])
{
	try
	{
		Chronometer chrono;
		srand(time(NULL));

		cout << setprecision(3) << std::fixed;
		
		ArgumentParser AP(argc,argv);
		
		if (AP.should_exit)
			return 0;
		
		auto Table = ReadTableFromSTDIN(AP.filename);
		auto SpeciesMap = ExtractLocations(Table,AP.NamedColumns,AP.latitude,AP.longitude);
		int numspecies = SpeciesMap.size();

		using SpeciesRegisters = pair<string, vector<Point>>;
		vector< SpeciesRegisters > V(SpeciesMap.begin(), SpeciesMap.end());
		sort(V.begin(), V.end(), [](const auto & L, const auto & R)
		{
			return L.second.size() > R.second.size();
		});
		
		for (const auto& v : V)
		{
			cout << "# " << v.first << " has " << v.second.size() << " observations \n";
		}

		vector<vector<Point>> Points;
		vector<string> names;
		Points.reserve(numspecies);
		names.reserve(numspecies);
		
		for (const auto& v : V)
		{
			Points.emplace_back(v.second);
			names.emplace_back(v.first);
		}

		GraphCalculator GC(AP.grid, AP.visibility, Points);

		Matrix M = GC.CalculateGraph();
		cout << M << endl;

		cout << "Calculating the matrix took " << chrono.Peek() << endl;
		cout  << endl << "------------------------------" << endl << endl;

		DiGraph D = DiGraph::FromAdjacencyMatrix(M,0.0001);
		D.set_names(names);

		auto edges = D.edges();

		sort(edges.begin(), edges.end(), [](const Edge & a, const Edge & b)
		{
			return a.weight() < b.weight();
		});
				
		for (const auto& e : edges)
		{
			cout << '\"' << D.get_name(e.from) << "\" ---> \"" << D.get_name(e.to)  << '\"' << " with weight " << e.weight() << endl;
		}
		
		if (AP.outfile != "")
		{
			ofstream out(AP.outfile);
			out << "G = DiGraph()\n";
			for (auto e : edges)
			{
				out << "G.add_edge(\"" << D.get_name(e.from) << "\",\"" << D.get_name(e.to)  << '\"' << "," << e.weight() << ")\n";
			}
		}

		cout << "Grid: " << AP.grid << endl;
		cout << "Visibility: " << AP.visibility << endl;

		cout << "Total time: " << chrono.Peek() << endl;
	}
	catch (std::exception& e)
	{
		cout << e.what() << '\n';
		return 1;
	}

	return 0;
}
