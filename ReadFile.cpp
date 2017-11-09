#include "ReadFile.hpp"
#include <fstream>
#include <istream>
#include <locale>
#include <sstream>
#include <string>

std::vector<std::vector<std::string>> ReadTable(std::istream& is)
{
	std::vector<std::vector<std::string>> table;
	std::string line;
	

	int i = 0;
	while (getline(is, line))
	{
		std::stringstream iss(line);
// 			std::cout << "line = " << line << std::endl;
		table.emplace_back(std::vector<std::string>());
		do
		{
			std::string sub;
			iss >> sub;
			if (!sub.empty())
			{
				table[i].emplace_back(sub);
// 					std::cout << "Substd::string: |" << sub << "| of length " << sub.size() << std::endl;
			}
		} while (iss);
		if (i > 0 && table[i].size() != table[i-1].size())
		{
			std::cerr << "Error in file: Some lines have a different number of space-separated inputs" << std::endl;
			std::cerr << "Error in line: " <<i << ":\n" << table[i] << "\n(which has " << table[i].size() << " inputs instead of " << table[i].size() << ")" << std::endl;
			throw;
		}
		++i;
	}
	
	if (table.empty())
	{
		std::cerr << "Empty input stream!" << std::endl;
		throw;
	}
	
	return table;
	
}


std::unordered_map<std::string,std::vector<Point>> ExtractLocations(const std::vector<std::vector<std::string>>& U, const std::vector<int>& names, int longitude, int latitude)
{
	std::unordered_map<std::string,std::vector<Point>> result;
	
	if (longitude == -1 || latitude == -1)
	{
		for (size_t i = 0; i < U[0].size(); ++i)
		{
			std::string colname = U[0][i];
			std::transform(colname.begin(), colname.end(), colname.begin(), ::tolower);
			
			if (colname == "longitud" || colname == "longitude")
			{
				longitude = i;
				std::cout << "Longitude deduced to be in column " << i << std::endl;
			}
			
			if (colname == "latitud" || colname == "latitude")
			{
				latitude = i;
				std::cout << "Latitude deduced to be in column " << i << std::endl;
			}
		}
	}
	if (longitude < 0 || latitude < 0)
	{
		throw "Latitude or Longitude not specified, and could not deduce from file";
	}
	
	for (size_t i = 1; i < U.size(); ++i)
	{
		std::string name;
		for (auto j : names)
		{
			name += U[i][j];
			name += ' ';
		}
		name.pop_back();
		try
		{
			double x = stod(U[i][latitude])*KmInADegree;
			double y = stod(U[i][longitude])*KmInADegree;
			
			result[name].emplace_back(x,y);

		} catch(...)
		{
			std::cerr << "Error: number not in a correct format: " << U[i][longitude] << ' ' << U[i][latitude] << std::endl;
		}
	}
	
	
	
	return result;
}
