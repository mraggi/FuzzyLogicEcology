#include "argumentparser.hpp"
#include <boost/program_options.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


ArgumentParser::ArgumentParser(int argc, char* argv[])
{
	namespace po = boost::program_options;

	std::string usage = std::string("Usage") + std::string(argv[0]) + " [OPTION]... FILE"; //NOLINT //REALLY??

	if (argc == 1)
	{
		std::cout << usage << std::endl;
		std::cout << "Option -h or --help prints full options" << std::endl;
		should_exit = true;
		return;
	}

	
	po::options_description basic_options("Allowed options");
	
	basic_options.add_options()
	("help,h", "produce help message")
	("name-columns,I", po::value< std::vector<int> >(), "Name column indexes (starting at 0).")
	("latitude,x", po::value< int >(), "Latitude (or x) column index. Default: deduce from input.")
	("longitude,y", po::value< int >(), "Latitude (or y) column index. Default: deduce from input.")
	("grid,g", po::value<int>(&grid)->default_value(grid), "Grid size. A larger grid means more accurate the calculations (but slower).")
	("memory,m", po::value<std::string>(), "Maximum amount of memory (in bytes) to use. Leave blank or at 0 to use all available memory (not recommended!). Can use KB, MB, GB.")
	("input-file,i", po::value< std::vector<std::string> >(), "Input file. If not given, the program will read from STDIN")
	("influence,v", po::value<double>(&influence)->default_value(influence), "Influence radius (in km). When doing the exponential decay model, C=1/(2*v^2) (in e^(-Cx^2))")
	("save-image,s", po::value< std::vector<std::string> >(), "In order to create an image, specify the (exact) name of the species.")
	("fuzzy-min,f", "Use minimum instead of product as the fuzzy logic model of intersection (warning: SLOW)")
	("propincuity,p", "Use Propincuity to calculate areas instead of exponential decay (warning: SLOW. Not recommended!)")
	("output-file,o", po::value< std::string >(), "sagemath output file name.")	
	("matrix-file", po::value< std::string >(), "matrix output file name.")	
	;

	po::positional_options_description p;
	p.add("input-file", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(basic_options).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help")) //NOLINT
	{
		std::cout << usage << '\n';
		std::cout << 
		"\nThis program takes input of the form:\n\n"
		"\tSpecies SomeInfo Latitude Longitude MoreInfo\n"
		"\trubra 99 50.3 58.8 blah\n"
		"\tpalustris 99 50.3 58.8 blah\n\n"
		"and produces an intersection matrix. See README.md for more details. The name can span multiple columns.\n\n";
		
		std::cout << basic_options;
		should_exit = true;
		return;
	}
	
	fuzzy_min = static_cast<bool>(vm.count("fuzzy-min"));
	propincuity = static_cast<bool>(vm.count("propincuity"));
	
	if (fuzzy_min && propincuity)
	{
		std::cout << "Cannot use both propincuity and fuzzy-min. Specify one or the other (or, preferably, neither), but not both.\n";
		should_exit = true;
		return;
	}
	
	

	if (vm.count("name-columns")) //NOLINT
	{
		NamedColumns = vm["name-columns"].as< std::vector<int> >();
		std::cout << "There are " << NamedColumns.size() << " name columns: " << NamedColumns << '\n';
	}
	
	if (vm.count("save-image")) //NOLINT
	{
		ImageSpecies = vm["save-image"].as< std::vector<std::string> >();
		std::cout << "Going to save " << ImageSpecies.size() << " images for the following species: " << ImageSpecies << '\n';
	}

	if (vm.count("latitude")) //NOLINT
	{
		x = vm["latitude"].as< int >();
		std::cout << "Latitude deduced to be at column with index " << x << '\n';
	}
	
	if (vm.count("longitude")) //NOLINT
	{
		y = vm["longitude"].as< int >();
        std::cout << "Longitude deduced to be at column with index " << y << '\n';
	}
	
// 	if (vm.count("arcgis-outfile"))
// 	{
// 		arcgisfile = vm["arcgis-outfile"].as< std::vector<std::string> >()[0];
// 		std::cout << "Arcgis file prefix is: " << arcgisfile << '\n';
// 	}
	
	if (vm.count("input-file")) //NOLINT
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
	  std::cout << "No input file given. Will read from STDIN... " << std::endl;
	}
	
// 	if (vm.count("output-file")) //NOLINT
// 	{
// 		std::string outfile = vm["output-file"].as<std::string>();
// 		std::cout << "Sage output file is: " << outfile << '\n';
// 	}
	
	if (vm.count("matrix-file")) //NOLINT
	{
		matrix_file = vm["matrix-file"].as<std::string>();
		std::cout << "Matrix output file is: " << matrix_file << '\n';
	}
	
	if (!vm.count("memory")) //NOLINT
	{
		memoryAvailable = getTotalSystemMemory();
		
		memoryAvailable -= 1*GB; // Leave at least 1 GB for the OS
		memoryAvailable *= 0.90;
		memoryAvailable = std::max(memoryAvailable,long(20L*MB)); // at least use 20MB
		std::cout << "Memory option not set. Using (almost) all available system memory: " << double(memoryAvailable)/MB << "MB" << std::endl;
	}
	else
	{
		std::string val = vm["memory"].as<std::string>();
		
		if (toupper(val.back()) == 'B')
			val.pop_back();
		
		size_t units = 1;
		
		char c = toupper(val.back());
		
		if (c == 'G')
			units = GB;
		if (c == 'M')
			units = MB;
		if (c == 'K')
			units = KB;
		
		if (units != 1)
			val.pop_back();
		
		try
		{
			memoryAvailable = stod(val)*units;
		}
		catch (...)
		{
			std::cerr << "Memory amount is in an invalid format. Use for example --memory=500MB" << std::endl;
			throw;
		}
		
		if (memoryAvailable < 20L*MB)
		{
			std::cerr << "You need at least 20 MB of memory. Setting it for you to 20MB." << std::endl;
			memoryAvailable = 20L*MB;
		}
	}
}

void ArgumentParser::printMessage() const
{
	std::cout << "Grid: " << grid << std::endl;
	if (!fuzzy_min && !propincuity)
		std::cout << "Visibility: " << influence << std::endl;
	if (fuzzy_min)
	{
		std::cout << "Using FUZZY MIN version of fuzzy logic.\n";
	}
	else if (propincuity)
	{
		std::cout << "Using PROPINCUITY (NOT FUZZY LOGIC!).\n";		
	}
	else
	{
		std::cout << "Using regular PRODUCT version of fuzzy logic.\n";		
	}
}
