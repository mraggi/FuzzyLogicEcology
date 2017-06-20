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

int grid = 2000;
double visibility = 0.5;

const int value_not_set = -1;

int main(int argc, char* argv[])
{
	try
	{
		string usage = string("Usage") + argv[0] + " [OPTION]... FILE";

		if (argc == 1)
		{
			cout << usage << endl;
			cout << "Option -h or --help prints options" << endl;
			return 0;
		}

		int latitude;
		int longitude;
		po::options_description desc("Allowed options");
		desc.add_options()
		("help,h", "produce help message")
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
			return 0;
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

		

#ifdef USE_GPU
		cout << "Using following GPU: " << endl;
		af::info();
#else
		cout << "Using the CPU!" << endl;
#endif

		string filename = "QuercusOaxaca.txt";
		vector<int> NamedColumns = {{1,2}};
		
		
		if (vm.count("namecolumns"))
		{
			NamedColumns = vm["namecolumns"].as< vector<int> >();
		}
		
		if (vm.count("input-file"))
		{
			filename = vm["input-file"].as< vector<string> >()[0];
		}

		auto R = ReadTableFromSTDIN(filename);
		auto U = ExtractLocations(R,NamedColumns,latitude,longitude);
		
		// 	auto U = LobataeExtractLocations(R);
		// 	auto U = CentroExtractLocations(R);


		vector< pair< string, vector<Point> > > V(U.begin(), U.end());
		sort(V.begin(), V.end(), [](const auto & R, const auto & L)
		{
			return R.second.size() > L.second.size();
		});
		int i = 0;

		for (auto v : V)
		{
			cout << "# " << i << ' ' << v.first << ": " << v.second.size() << '\n';
			++i;
		}

		vector<vector<Point>> E;
		vector<string> names;
		E.reserve(V.size());

		for (auto& v : V)
		{
			E.emplace_back(v.second);
			names.emplace_back(v.first);
		}

		GraphCalculator GC(grid, visibility, E);
		int numspecies = U.size();

		Chronometer total;
		srand(time(NULL));
		Chronometer T;

		cout << setprecision(3) << std::fixed;

		auto M = GC.CalculateGraph();
		cout << M << endl;

		cout << "Calculating the matrix took " << T.Reset() << endl;
		cout  << endl << "------------------------------" << endl << endl;

		DiGraph D = DiGraph::FromAdjacencyMatrix(M,0.0001);
		D.set_names(names);

		auto edges = D.edges();

		sort(edges.begin(), edges.end(), [](const Edge & a, const Edge & b)
		{
			return a.weight() < b.weight();
		});
				
		for (auto e : edges)
		{
			cout << '\"' << D.get_name(e.from) << "\" ---> \"" << D.get_name(e.to)  << '\"' << " with weight " << e.weight() << endl;
			
			
			
		}
		
		// If there is an output file, do this
		if (vm.count("output-file"))
		{
			string outfile = vm["output-file"].as<string>();
			ofstream out(outfile);
			out << "G = DiGraph()\n";
			for (auto e : edges)
			{
				out << "G.add_edge(\"" << D.get_name(e.from) << "\",\"" << D.get_name(e.to)  << '\"' << "," << e.weight() << ")\n";
			}
		}


		cout << "Grid: " << grid << endl;
		cout << "Visibility: " << visibility << endl;

		cout << "Total time: " << total.Peek() << endl;
	}
	catch (std::exception& e)
	{
		cout << e.what() << '\n';
		return 1;
	}

	return 0;
}
