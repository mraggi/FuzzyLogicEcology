#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Utility.hpp"
#include "Point.hpp"
#include "TimeHelpers.hpp"
#include "ReadFile.hpp"
// #include "GraphCalculator.hpp"

#include "FuzzyNetworkProduct.hpp"
#include "FuzzyNetworkMin.hpp"
#include "FuzzyNetworkPromiscuity.hpp"

#include "argumentparser.hpp"
#include "Graph.hpp"
#include "GraphMeasures.hpp"

int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
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
		

		
		if (AP.propincuity)
		{
			FuzzyNetworkPromiscuity GC(AP.grid, Points, AP.memoryAvailable);
			std::cout << "Done pre-processing in " << chrono.Peek() << "s. Starting calculation..." << std::endl;
			GC.PrintEverything(names, AP.outfile);
		}
		else if (AP.fuzzy_min)
		{
			FuzzyNetworkMin GC(AP.grid, Points, AP.memoryAvailable, AP.visibility);
			std::cout << "Done pre-processing in " << chrono.Peek() << "s. Starting calculation..." << std::endl;
			GC.PrintEverything(names, AP.outfile);
		}
		else
		{	
			FuzzyNetworkProduct GC(AP.grid, Points, AP.memoryAvailable, AP.visibility);
			std::cout << "Done pre-processing in " << chrono.Peek() << "s. Starting calculation..." << std::endl;
			GC.PrintEverything(names, AP.outfile);
		}
		
		AP.printMessage();

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	std::cout << "\n------------------------\nTotal time: " << chrono.Peek() << "s\n";
	return 0;
}
