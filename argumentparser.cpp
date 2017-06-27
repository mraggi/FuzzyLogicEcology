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
	("grid,g", po::value<int>(&grid)->default_value(grid), "grid size")
	("visibility,v", po::value<double>(&visibility)->default_value(visibility), "visibility in km")
	("namecolumns,I", po::value< vector<int> >(), "name columns")
	("input-file,i", po::value< vector<string> >(), "input file")
	("output-file,o", po::value< string >(), "sage output file with graph description.")		
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
		cout << "Input file is: " << vm["input-file"].as< vector<string> >() << '\n';
	}
	
	if (vm.count("output-file"))
	{
		cout << "Sage output file is: " << vm["output-file"].as< string >() << '\n';
	}

	if (vm.count("namecolumns"))
	{
		NamedColumns = vm["namecolumns"].as< vector<int> >();
	}
	
	if (vm.count("input-file"))
	{
		filename = vm["input-file"].as< vector<string> >()[0];
	}
	
	// If there is an output file, do this
	if (vm.count("output-file"))
	{
		string outfile = vm["output-file"].as<string>();
		
	}
	
	if (!vm.count("memory"))
	{
		memoryAvailable = getTotalSystemMemory();
        
        if (memoryAvailable < 2*GB)
        {
            throw "Not enough memory to run this program!";
        }
		
		memoryAvailable -= 2*GB; // Leave at least 2 GB for the OS
		memoryAvailable *= 0.95; // Don't use more than 95% of memory!
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
				memoryAvailable = stoll(val)*units;
			}
			catch (std::exception& e)
			{
				cerr << "Memory is in an invalid format" << endl;
				throw e;
			}
			
		}
		
		
		
	}
}
