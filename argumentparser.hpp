#pragma once
#include <vector>
#include <string>
#include "Utility.hpp"

const int value_not_set = -1;

struct ArgumentParser
{
	ArgumentParser(int argc, char* argv[]);
	int latitude{value_not_set};
	int longitude{value_not_set};
	double visibility {0.5};
	int grid {2000};
	std::string filename { "QuercusOaxaca.txt" };
	std::string outfile { "" };
	std::vector<int> NamedColumns {{1,2}};
	bool should_exit { false };
};
