#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include "argumentparser.hpp"

using namespace std;
namespace po = boost::program_options;

ArgumentParser::ArgumentParser(int argc, char* argv[])
{
	string usage = string("Usage") + argv[0] + " [OPTION]... FILE";

	if (argc == 1)
	{
		cout << usage << endl;
		cout << "Option -h or --help prints options" << endl;
		should_exit = true;
		return;
	}

	
	po::options_description desc("Allowed options");
	
	desc.add_options()
	("help,h", "produce help message")
	("memory,m", po::value<string>(), "Maximum amount of memory in bytes to use. Leave blank or at 0 to autodetect. Can use KB, MB, GB")
	("latitude,x", po::value<int>(&latitude)->default_value(value_not_set), "latitude")
	("longitude,y", po::value<int>(&longitude)->default_value(value_not_set), "longitude")
	("grid,g", po::value<int>(&grid)->default_value(grid), "grid size (The larger grid is, the more accurate the calculations will be, but it's also slower.)")
	("visibility,v", po::value<double>(&visibility)->default_value(visibility), "visibility in km")
	("namecolumns,I", po::value< vector<int> >(), "name columns")
	("input-file,i", po::value< vector<string> >(), "input file. If left blank, the program will read from STDIN")
	("output-file,o", po::value< string >(), "sage output file.")	
	;

	po::positional_options_description p;
	p.add("input-file", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		cout << usage << '\n';
		cout << desc;
		should_exit = true;
	}

	if (vm.count("namecolumns"))
	{
		cout << "Name columns are: " << vm["namecolumns"].as< vector<int> >().size() << ": " << vm["namecolumns"].as< vector<int> >() << '\n';
	}

	if (vm.count("input-file"))
	{
		string filename = vm["input-file"].as< vector<string> >()[0];
		cout << "Input file is: " << filename << '\n';
		file.open(filename);
		is = &file;
	}
	
	if (vm.count("output-file"))
	{
		cout << "Sage output file is: " << vm["output-file"].as< string >() << '\n';
	}

	if (vm.count("namecolumns"))
	{
		NamedColumns = vm["namecolumns"].as< vector<int> >();
	}
	
	// If there is an output file, do this
	if (vm.count("output-file"))
	{
		string outfile = vm["output-file"].as<string>();
		
	}
	
	if (!vm.count("memory"))
	{
		cout << "Memory option not set. Attempting to use all system memory" << endl;
#ifdef USE_GPU
		memoryAvailable = 1*GB;
#else
		memoryAvailable = getTotalSystemMemory();
        
		memoryAvailable -= 1*GB; // Leave at least 1 GB for the OS
		memoryAvailable *= 0.9; // Don't use more than 90% of memory!
		memoryAvailable = max(memoryAvailable,long(20L*MB)); // at least use 20MB
#endif
	} else
	{
		size_t units = 1;
		string val = vm["memory"].as<string>();
		if (toupper(val.back()) == 'B')
		{
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
				cerr << "Memory amount is in an invalid format. Use for example --memory=500MB" << endl;
				throw e;
			}
			
		}
		
		
		
	}
}
