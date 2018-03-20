# SpaDiMo-F

Short for SPAcial DIstribution MOdeling with Fuzzy logic.

Construction of a digraph based on data points from observations of ecological distributions to calculate overlaps using fuzzy logic.

## Some (brief) theory
 
 See paper (upcoming!) for more details. 
 
 In short, for each species we produce a matrix of probability. Each observation *strengthens* the entries of the matrix: each entry goes from 0.0 up to 1.0, depending on how close the observed data are to the points.
 
 Here is a visual representation of what we mean:
 
 ![Fuzzy Logic](https://github.com/mraggi/FuzzyLogicEcology/blob/master/Paper/fuzzyinverted.png "Fuzzy Logic Blobs")
 
 Then, for each pair of species, we calculate the area of overlap and produce a (directed) edge from species A to species B whose weight is the weight of the overlap area over the area of A (*i.e.* how much of A is inside B). This is done in a highly efficient manner.

### Types of fuzzy logic

See [wikipedia's entry on fuzzy logic](https://en.wikipedia.org/wiki/Fuzzy_logic) for a more in-depth explanation.

There are many models for fuzzy logic, depending on what one means when talking about the *intersection*. If property A is 70% true and property B is 50% true, how true is property (A and B)? Consider the following two:
	* (A and B) is 35% true (product). This mimics independent events in probability.
	* (A and B) is 50% true (minimum). Also called the Zadeh operators.

From either of those (and many more) we could create a model for fuzzy logic.

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
libpng
png++
```

Make sure you have a recent version of each. In particular, your `C++` compiler should have `C++14` support (gcc >= 5.2 and clang >= 3.9 should be fine), a recent version of eigen3 (at least 3.2) and a recent version of boost (at least 1.59). Let us know if we can help.

There is some evidence that this program performs much better with clang than GCC. It also performs better with clang 5.0 than 4.0.

We are in the process of making all this available as a web service. If you are interested, let us know. We'll work faster and we'll be more motivated if we see there is some interest.

#### Installing dependencies in debian/ubuntu/mint
For example, to install in debian/ubuntu, do `sudo apt-get install build-essentials libboost-all-dev cmake git eigen3 libpng`

#### Installing dependencies in arch/chakra/manjaro
In a terminal, type:
`sudo pacman -S cmake boost git eigen3 png++`

### Installing
After the dependencies are satisfied, just do the usual cmake dance:
```bash
	cd /path/to/wherever
	git clone https://github.com/mraggi/FuzzyLogicEcology.git
	cd FuzzyLogicEcology
	mkdir build
	cd build
	cmake ..
	make
```

Here is a dummy example text file that the program reads (for a real example, see lobatae.txt or QuercusOaxaca.txt or rojos.txt)
```txt
	Species SomeInfo Latitude Longitude MoreInfo
	rubra  99	50.3 58.8 blah
	palustris  99	50.3 58.8 blah
```
Suppose the above file is saved as `myspecies.txt`. The columns `SomeInfo` and `MoreInfo` will be ignored. Columns are separated by tabs, spaces, or a combination of the two. Columns cannot contain spaces, so for example, if you have a column called `Name` and then in that column an entry is `Abies balsamea`, this will be detected as two different columns (and header row should be adjusted accordingly, for example, by adding `Family` before `Name`).


Then run the program as follows:
```bash
	./fuzzylogic -I0 --grid=5000 --memory=2GB myspecies.txt
```
The I0 means that the name with which the species will be identified is in column 0. If there is more than one column with the name, just pass all the columns as arguments (i.e. -I0 -I2). **Note that the columns start at 0, not 1.**


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

	-o [ --output-file ] arg     sagemath output file. This is in order to analyze 
    							 the resulting DiGraph in sage mathematics software.
 ```

### Acknowledgements

This work was done by a group of researchers at *Escuela Nacional de Estudios Superiores unidad Morelia, Universidad Nacional Autónoma de México* with a research grant **PAPIIT IA106316**. For the complete list of authors, see the (upcoming) paper. All provided data was collected by César Andrés Torres Miranda *(need to fill this!)

If you find it useful, or if you have any comments, suggestions, feature requests, *etc.* feel free to email us at [mraggi@gmail.com](mailto:mraggi@gmail.com).
