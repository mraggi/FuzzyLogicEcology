#pragma once
#include "Utility.hpp"
#include <fstream>
#include <string>
#include <vector>

const int value_not_set = -1;

struct ArgumentParser
{
	ArgumentParser(int argc, char* argv[]);
	int latitude{value_not_set};
	int longitude{value_not_set};
	double visibility {2};
	int grid {2000};
	std::string outfile { "" };
	std::vector<int> NamedColumns {0};
	std::vector<std::string> ImageSpecies {};
	bool should_exit { false };
	long memoryAvailable{0};
	std::istream* is{&std::cin};
	std::ifstream file;
	void printMessage() const;
	bool fuzzy_min {false};
	bool propincuity {false};
};
