# FuzzyLogicEcology
Construction of a digraph based on data points from observations of ecological distributions to calculate overlaps using fuzzy logic.

## Installing

First, you need to make sure you have the following dependencies:
```
g++ (or clang++)
eigen3
boost
cmake
```

### Installing dependencies in debian/ubuntu/mint
For example, to install in debian/ubuntu, do `sudo apt-get install build-essentials libboost-all-dev cmake git eigen3`

### Installing dependencies in arch/chakra/manjaro
In a terminal, type:
`sudo pacman -S cmake boost git eigen3`

### Installing
After the dependencies are satisfied, just do the usual:
```bash
	cd /path/to/wherever
	git clone https://github.com/mraggi/FuzzyLogicEcology.git
	cd FuzzyLogicEcology
	mkdir build
	cd build
	cmake ..
	make
```

The file that the program reads needs to be something like this (for a real example, see lobatae.txt or QuercusOaxaca.txt)
```txt
	Species SomeInfo Latitude Longitude MoreInfo
	Lobatae  99	50.3 58.8 blah
	Quercus  99	50.3 58.8 blah
```
Suppose the above file is saved as `myspecies.txt`


Then run the program as follows:
```bash
	./fuzzylogic -I1 --grid=5000 --memory=1GB myspecies.txt
```
The I1 means that the name with which the species will be identified is in column 1. If there is more than one column with the name, just pass all the columns as arguments (i.e. -I1 -I2)


### A more complete example

To build the Quercus Oaxaca Graph using default options (i.e. for an example), see:
```bash
	./quercus_build_graph.sh
```

Of course, more detailed options can be found by typing
```bash
	./fuzzylogic --help
```



### Usage and Tutorial

Let's write this.
