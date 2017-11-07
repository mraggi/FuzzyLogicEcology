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
	./fuzzylogic -I0 --grid=5000 --memory=1GB myspecies.txt
```
The I1 means that the name with which the species will be identified is in column 1. If there is more than one column with the name, just pass all the columns as arguments (i.e. -I0 -I2). **Note that the columns start at 0, not 1**


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

```tex
Basic options:
  -h [ --help ]                  produce help message
  -m [ --memory ] arg            Maximum amount of memory (in bytes) to use. 
                                 Leave blank or at 0 to use all available 
                                 memory (not recommended!). Can use KB, MB, GB.
  -I [ --namecolumns ] arg       name columns
  -x [ --latitude ] arg (=-1)    latitude
  -y [ --longitude ] arg (=-1)   longitude
  -g [ --grid ] arg (=2000)      grid size. A larger grid means more accurate 
                                 the calculations (but slower).
  -i [ --input-file ] arg        input file. If not given, the program will 
                                 read from STDIN
  -v [ --visibility ] arg (=0.5) visibility in km. When doing the exponential 
                                 decay model, the coefficient (in e^(-Cx^2)
  -f [ --fuzzy_min ]             Use minimum instead of product as the fuzzy 
                                 logic model of intersection (warning: SLOW)
  -p [ --propincuity ]           Use Propincuity to calculate areas (warning: 
                                 SLOW. Not recommended!)
  -o [ --output-file ] arg       sage output file.
 ```