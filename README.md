# FuzzyLogicEcology
Construction of a digraph based on data points from observations of ecological distributions to calculate overlaps using fuzzy logic.

## Some (brief) theory
 
 See paper for more details. 
 
 In short, for each species we produce a matrix of probability. Each observation stenghthens the entries of the matrix: each entry goes from 0.0 up to 1.0, depending on how close other points are.
 
 Here is a visual representation:
 
 ![Fuzzy Logic](https://github.com/mraggi/FuzzyLogicEcology/blob/master/Paper/fuzzyinverted.png "Fuzzy Logic Blobs")
 
 Then, for each pair of species, we calculate the area of overlap and produce a (directed) edge from species A to species B whose weight is the weight of the overlap area over the area of A (*i.e.* how much of A is inside B).

### Types of fuzzy logic

See [wikipedia's entry on fuzzy logic](https://en.wikipedia.org/wiki/Fuzzy_logic) for a more in-depth explanation.

There are many models for fuzzy logic, depending on what one means when talking about the *intersection*. If property A is 70% true and property B is 50% true, how true is property (A and B)? Consider the following two:
	* (A and B) is 35% true (product)
	* (A and B) is 50% true (minimum)

From both of those (and many more) we could create a model for fuzzy logic (actually, from any monotone function which satisfies the same properties as crisp intersection and some more).
 
### Propincuity
 The standard way of defining the distribution area of a species is with *propincuity*, which is also available in this program (for comparison only. It's slow, and there are probably faster alternatives out there). 
 
 In propincuity, one takes a minimum spanning tree of the vertices, sets the *radius* to the average edge length, and then considers the area of distribution by moving a circle with said radius along all the edges of the spanning tree, like the following picture (radius adjusted for clarity).
 ![Propincuity](https://github.com/mraggi/FuzzyLogicEcology/blob/master/Paper/prop.png "Propincuity")

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
	rubra  99	50.3 58.8 blah
	palustris  99	50.3 58.8 blah
```
Suppose the above file is saved as `myspecies.txt`


Then run the program as follows:
```bash
	./fuzzylogic -I0 --grid=5000 --memory=2GB myspecies.txt
```
The I1 means that the name with which the species will be identified is in column 1. If there is more than one column with the name, just pass all the columns as arguments (i.e. -I0 -I2). **Note that the columns start at 0, not 1**


## A more realistic example

To build the Quercus Oaxaca Graph using default options (i.e. for an example), see:
```bash
	./quercus_build_graph.sh
```

Of course, more detailed options can be found by typing
```bash
	./fuzzylogic --help
```

## Usage and Tutorial

```tex
Basic options:

	-h [ --help ]                Produce this help message.

	-I [ --namecolumns ] arg     Name columns (start at 0).

	-g [ --grid ] arg (=2000)    Grid size. A larger grid means more accurate 
                                 the calculations (but slower).

	-m [ --memory ] arg          Maximum amount of memory (in bytes) to use. 
                                 Leave blank or at 0 to use all available 
                                 memory (not recommended!). Can use KB, MB, GB.
                                 A value of about 2GB is recommended, depending 
                                 on how much you have available.

	-i [ --input-file ] arg      Input file. If not given, the program will 
                                 read from STDIN.

	-v [ --visibility ] arg (=0.5) Visibility in km. When doing the exponential 
                                 decay model, coefficient C (in e^(-Cx^2))

	-f [ --fuzzy_min ]           Use minimum instead of product as the fuzzy 
                                 logic model of intersection (warning: SLOW).

	-p [ --propincuity ]         Use Propincuity to calculate areas (warning: 
                                 SLOW. Not recommended!).

	-o [ --output-file ] arg     sagemath output file.
 ```

 
