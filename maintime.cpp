#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

#include "Point.hpp"
#include "ReadFile.hpp"
#include "TimeHelpers.hpp"
#include "Utility.hpp"

#include "FuzzyNetworkMin.hpp"
#include "FuzzyNetworkProduct.hpp"
#include "FuzzyNetworkPromiscuity.hpp"

#include "Graph.hpp"
#include "GraphMeasures.hpp"
#include "argumentparser.hpp"

int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
	srand(time(nullptr));
	
	Chronometer chrono;
	try
	{
		ArgumentParser AP(argc,argv);
		
		if (AP.should_exit)
			return 0;
		
		auto Table = ReadTable(*AP.is);
		auto SpeciesMap = ExtractLocations(Table, AP.NamedColumns, AP.x, AP.y);
		int numspecies = SpeciesMap.size();

		using SpeciesRegisters = std::pair<std::string, std::vector<Point>>;
		std::vector< SpeciesRegisters > V(SpeciesMap.begin(), SpeciesMap.end());
		sort(V.begin(), V.end(), [](const auto & L, const auto & R)
		{
			//Decreasing order by number of data points
			return L.second.size() > R.second.size();
		});
		
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
		
		std::vector<int> tests = {100,500, 1000,2000,2500,3000,4000,5000,7500,10000};
// 		std::vector<int> tests = {100,500, 1000};
		
		Chronometer W;
		for (int n : tests)
		{
			FuzzyNetworkProduct GC(n, Points, AP.memoryAvailable, AP.influence);
			GC.CalculateGraph();
			
			std::cout << "GGG: (" << n << "," << W.Reset() << ")" << std::endl;
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
