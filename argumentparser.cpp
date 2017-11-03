#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include "argumentparser.hpp"

namespace po = boost::program_options;

ArgumentParser::ArgumentParser(int argc, char* argv[])
{
	std::string usage = std::string("Usage") + argv[0] + " [OPTION]... FILE";

	if (argc == 1)
	{
		std::cout << usage << std::endl;
		std::cout << "Option -h or --help prints options" << std::endl;
		should_exit = true;
		return;
	}

	
	po::options_description desc("Allowed options");
	
	desc.add_options()
	("help,h", "produce help message")
	("memory,m", po::value<std::string>(), "Maximum amount of memory in bytes to use. Leave blank or at 0 to autodetect. Can use KB, MB, GB")
	("latitude,x", po::value<int>(&latitude)->default_value(value_not_set), "latitude")
	("longitude,y", po::value<int>(&longitude)->default_value(value_not_set), "longitude")
	("grid,g", po::value<int>(&grid)->default_value(grid), "grid size (The larger grid is, the more accurate the calculations will be, but it's also slower.)")
	("visibility,v", po::value<double>(&visibility)->default_value(visibility), "visibility in km")
	("namecolumns,I", po::value< std::vector<int> >(), "name columns")
	("input-file,i", po::value< std::vector<std::string> >(), "input file. If left blank, the program will read from STDIN")
	("output-file,o", po::value< std::string >(), "sage output file.")	
// 	("arcgis-outfile", po::value< std::vector<std::string> >(), "ArcGIS output files (see )http://webhelp.esri.com/arcgisdesktop/9.3/index.cfm?TopicName=ESRI\%20ASCII\%20Raster\%20format")
	;

	po::positional_options_description p;
	p.add("input-file", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		std::cout << usage << '\n';
		std::cout << desc;
		should_exit = true;
		return;
	}

	if (vm.count("namecolumns"))
	{
		std::cout << "Name columns are: " << vm["namecolumns"].as< std::vector<int> >().size() << ": " << vm["namecolumns"].as< std::vector<int> >() << '\n';
	}

// 	if (vm.count("arcgis-outfile"))
// 	{
// 		arcgisfile = vm["arcgis-outfile"].as< std::vector<std::string> >()[0];
// 		std::cout << "Arcgis file prefix is: " << arcgisfile << '\n';
// 	}
	
	if (vm.count("input-file"))
	{
		std::string filename = vm["input-file"].as< std::vector<std::string> >()[0];
		std::cout << "Input file is: " << filename << '\n';
		try
		{
		  file.open(filename);
		  is = &file;
		} catch (...)
		{
		  std::cout << "Error opening file: " << filename << std::endl;
		  is = &std::cin;
		}
	} else
	{
	  std::cout << "No input file given. Will read from STDIN" << std::endl;
	}
	
	if (vm.count("output-file"))
	{
		std::cout << "Sage output file is: " << vm["output-file"].as< std::string >() << '\n';
	}

	if (vm.count("namecolumns"))
	{
		NamedColumns = vm["namecolumns"].as< std::vector<int> >();
	}
	
	// If there is an output file, do this
	if (vm.count("output-file"))
	{
		std::string outfile = vm["output-file"].as<std::string>();
		
	}
	
	if (!vm.count("memory"))
	{
		std::cout << "Memory option not set. Attempting to use all system memory" << std::endl;
		memoryAvailable = getTotalSystemMemory();
		
		memoryAvailable -= 1*GB; // Leave at least 1 GB for the OS
		memoryAvailable *= 0.9; // Don't use more than 90% of memory!
		memoryAvailable = std::max(memoryAvailable,long(20L*MB)); // at least use 20MB
	} else
	{
		std::string val = vm["memory"].as<std::string>();
		if (toupper(val.back()) == 'B')
		{
			size_t units = 1;
			val.pop_back();
			char c = toupper(val.back());
			
			if (c == 'G')
				units = GB;
			if (c == 'M')
				units = MB;
			if (c == 'K')
				units = KB;
			
			if (units != 1)
			{
				val.pop_back();
			}
			try
			{
				memoryAvailable = stol(val)*units;
			}
			catch (std::exception& e)
			{
				std::cerr << "Memory amount is in an invalid format. Use for example --memory=500MB" << std::endl;
				throw;
			}
			
		}
		
		
		
	}
}
