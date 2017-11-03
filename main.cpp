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
#include "Graph.hpp"
#include "GraphMeasures.hpp"

void printLibraryMessage();

int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(3) << std::fixed;
	srand(time(NULL));

	
	Chronometer chrono;
	try
	{
		ArgumentParser AP(argc,argv);
		
		if (AP.should_exit)
			return 0;
		
		auto Table = ReadTable(*AP.is);
		auto SpeciesMap = ExtractLocations(Table,AP.NamedColumns,AP.latitude,AP.longitude);
		int numspecies = SpeciesMap.size();

		using SpeciesRegisters = std::pair<std::string, std::vector<Point>>;
		std::vector< SpeciesRegisters > V(SpeciesMap.begin(), SpeciesMap.end());
// 		sort(V.begin(), V.end(), [](const auto & L, const auto & R)
// 		{
// 			//Decreasing order by number of data points
// 			return L.second.size() > R.second.size();
// 		});
		
		std::cout << std::endl << "********************* Species Found *******************" << std::endl;
		int num = 0;
		for (const auto& v : V)
		{
			std::cout << "# "  << num << ' ' << v.first << " has " << v.second.size() << " observations \n";
			++num;
		}
		std::cout << "***********************************************" << std::endl;
		
		std::vector<std::vector<Point>> Points;
		std::vector<std::string> names;
		Points.reserve(numspecies);
		names.reserve(numspecies);
		
		for (const auto& v : V)
		{
			Points.emplace_back(v.second);
			names.emplace_back(v.first);
		}
		
		GraphCalculator GC(AP.grid, AP.visibility, Points, AP.memoryAvailable);

		std::cout << "Done pre-processing in " << chrono.Peek() << "s. Starting calculation..." << std::endl;
		Matrix M = GC.CalculateGraph();
		
		for (int i = 0; i < numspecies; ++i)
		{
			std::cout << "  " << names[i] << " has area " << GC.GetTotalArea(i) << std::endl;
		}
		
		std::cout << std::endl << "Adjacency Matrix: " << std::endl;
		std::cout << M << std::endl;

		std::cout << "Done Calculating Graph in " << chrono.Peek() << "s." << std::endl;
		std::cout  << std::endl << "------------------------------" << std::endl << std::endl;

		DiGraph D = DiGraph::FromAdjacencyMatrix(M);
		
		auto edges = D.edges();


		auto by_weight = [](const Edge & a, const Edge & b)
		{
			return a.weight() < b.weight();
		};
		
		sort(edges.begin(), edges.end(), by_weight);
				
		for (const auto& e : edges)
		{
			std::cout << '\"' << names[e.from] << "\" ---> \"" << names[e.to]  << '\"' << " with weight " << e.weight() << std::endl;
		}
		
		if (AP.outfile != "")
		{
			std::ofstream out(AP.outfile);
			out << "G = DiGraph()\n";
			for (auto e : edges)
			{
				out << "G.add_edge(\"" << names[e.from] << "\",\"" << names[e.to]  << '\"' << "," << e.weight() << ")\n";
			}
		}

		std::cout << "Grid: " << AP.grid << std::endl;
		std::cout << "Visibility: " << AP.visibility << std::endl;

		printLibraryMessage();
		
		
		std::cout << "Areas: ";
		GC.printAreaVector(std::cout, ',');
		
// 		if (AP.arcgisfile != "")
// 		{
// //			 GC.WriteArcGis(AP.arcgisfile);
// 			std::cout << "Operation writing to arcgis file not supported yet :(" << std::endl;
// 		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	std::cout << std::endl << "Total time: " << chrono.Peek() << std::endl;
	return 0;
}


void printLibraryMessage()
{
	#if FUZZY_MIN
		std::cout << "USING EIGEN (FUZZY MIN mode)" << std::endl;
	#elif USE_BLAZE
		std::cout << "USING BLAZE (FUZZY PRODUCT mode)" << std::endl;
	#elif USE_EIGEN
		std::cout << "USING EIGEN (FUZZY PRODUCT mode)" << std::endl;
	#else
		std::cout << "Error: You must use either Eigen or Blaze!" << std::endl;
	#endif
}
