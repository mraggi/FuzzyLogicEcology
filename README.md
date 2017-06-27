# FuzzyLogicEcology
Construction of a digraph based on data points from observations of ecological distributions to calculate overlaps using fuzzy logic.

## Installing

First, you need to make sure you have the following dependencies:
```
g++ or clang++
blaze
boost
cmake
```

For example, to install in debian/ubuntu, do `sudo apt-get install build-essentials libboost-all-dev cmake git`

Then you have to install blaze manually. See [https://bitbucket.org/blaze-lib/blaze/wiki/Configuration%20and%20Installation]


To use, just do the usual:
```bash
	cd /path/to/wherever
	git clone https://github.com/mraggi/FuzzyLogicEcology.git
	cd FuzzyLogicEcology
	mkdir build
	cd build
	cmake ..
	make
```


To build the Quercus Oaxaca Graph using default options (i.e. for an example), see:
```bash
	./quercus_build_graph.sh
```

Of course, more detailed options can be found by typing
```bash
	./fuzzylogic --help
```
