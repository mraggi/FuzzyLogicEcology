#include "ReadFile.hpp"
#include <fstream>
#include <istream>
#include <sstream>
#include <locale>

vector<vector<string>> ReadTable(istream& is)
{
	vector<vector<string>> R;
	string line;
	

	int i = 0;
	while (getline(is, line))
	{
		std::istringstream iss(line);
// 			cout << "line = " << line << endl;
		R.push_back({});
		do
		{
			string sub;
			iss >> sub;
			if (!sub.empty())
			{
				R[i].emplace_back(sub);
// 					cout << "Substring: |" << sub << "| of length " << sub.size() << std::endl;
			}
		} while (iss);
		if (i > 0 && R[i].size() != R[i-1].size())
		{
			cerr << "Error in file: Some lines have a different number of space-separated inputs" << endl;
			cerr << "Error in line: " <<i << ":\n" << R[i] << "\n(which has " << R[i].size() << " inputs instead of " << R[i].size() << ")" << endl;
			throw;
		}
		++i;
	}
	
	return R;
	
}

unordered_map<string,vector<Point>> QuercusExtractLocations(const vector<vector<string>>& U)
{
	unordered_map<string,vector<Point>> R;
	
	
	
	for (int i = 1; i < U.size(); ++i)
	{
		string name = U[i][1] + ' ' + U[i][2];
		double x = stod(U[i][3]);
		double y = stod(U[i][4]);
		
		
		
		R[name].emplace_back(x,y);
	}
	
	
	
	return R;
}

unordered_map<string,vector<Point>> LobataeExtractLocations(const vector<vector<string>>& U)
{
	unordered_map<string,vector<Point>> R;
	
	
	
	for (int i = 1; i < U.size(); ++i)
	{
		string name = U[i][1];
		double x = stod(U[i][2]);
		double y = stod(U[i][3]);
		
		R[name].emplace_back(x,y);
	}
	
	
	
	return R;
}

unordered_map<string,vector<Point>> CentroExtractLocations(const vector<vector<string>>& U)
{
	unordered_map<string,vector<Point>> R;
	
	
	
	for (int i = 1; i < U.size(); ++i)
	{
		string name = U[i][0]+ ' ' +U[i][1]+ ' ' +U[i][2]+ ' ' +U[i][3]+ ' ' +U[i][4]+ ' ' +U[i][5]+ ' ' +U[i][6];
		double x = stod(U[i][7]);
		double y = stod(U[i][8]);
		
		R[name].emplace_back(x,y);
	}
	
	
	
	return R;
}

unordered_map<string,vector<Point>> ExtractLocations(const vector<vector<string>>& U, const vector<int>& names, int longitude, int latitude)
{
	unordered_map<string,vector<Point>> R;
	
	if (longitude == -1 || latitude == -1)
	{
		for (int i = 0; i < U[0].size(); ++i)
		{
			string colname = U[0][i];
			std::transform(colname.begin(), colname.end(), colname.begin(), ::tolower);
			
			if (colname == "longitud" || colname == "longitude")
			{
				longitude = i;
				cout << "Longitude deduced to be in column " << i << endl;
			}
			
			if (colname == "latitud" || colname == "latitude")
			{
				latitude = i;
				cout << "Longitude deduced to be in column " << i << endl;
			}
		}
	}
	if (longitude < 0 || latitude < 0)
	{
		throw "Latitude or Longitude not specified, and could not deduce from file";
	}
	
	for (int i = 1; i < U.size(); ++i)
	{
		string name;
		for (auto j : names)
		{
			name += U[i][j];
			name += ' ';
		}
		name.pop_back();
		try
		{
			double x = stod(U[i][latitude]);
			double y = stod(U[i][longitude]);
			R[name].emplace_back(x,y);

		} catch(...)
		{
			cerr << "Error: number not in a correct format: " << U[i][longitude] << ' ' << U[i][latitude] << endl;
		}
	}
	
	
	
	return R;
}
