# SGraFuLo

Short for Sympatry Graphs with Fuzzy Logic.

This is software to automatically construct a sympatry network, which is a digraph based on data points from observations of ecological distributions to calculate overlaps using fuzzy logic.

## Some (brief) theory
 
 See paper (upcoming!) for more details. 
 
In short, for each species we produce a large matrix of numbers between 0 and 1, which denotes the observed *area of influence* of said species. Each observation *strengthens* the entries of the matrix: each entry goes from 0.0 up to 1.0, depending on how close the observed data are to the points.
 
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
 
 ![Propincuity](https://github.com/mraggi/FuzzyLogicEcology/blob/master/prop.png "Propincuity")

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

Make sure you have a recent version of each. In particular, your `C++` compiler should have `C++14` support (gcc >= 5.2 and clang >= 3.9 should be fine), a recent version of eigen3 (at least 3.2) and a recent version of boost (at least 1.59). Let us know if you encounter any issues.

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
	./fuzzylogic -I0 --grid=2500 --memory=2GB myspecies.txt
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

  -h [ --help ]               produce help message
  -I [ --name-columns ] arg   Name column indexes (starting at 0).
  -x [ --latitude ] arg       Latitude (or x) column index. Default: deduce 
                              from input.
  -y [ --longitude ] arg      Latitude (or y) column index. Default: deduce 
                              from input.
  -g [ --grid ] arg (=2500)   Grid size. A larger grid means more accurate the 
                              calculations (but slower).
  -m [ --memory ] arg         Maximum amount of memory (in bytes) to use. Leave
                              blank or at 0 to use all available memory (not 
                              recommended!). Can use KB, MB, GB.
  -i [ --input-file ] arg     Input file. If not given, the program will read 
                              from STDIN
  -v [ --influence ] arg (=1) Influence radius (in km). When doing the 
                              exponential decay model, C=1/(2*v^2) (in 
                              e^(-Cx^2))
  -s [ --save-image ] arg     In order to create an image, specify the (exact) 
                              name of the species.
  -f [ --fuzzy-min ]          Use minimum instead of product as the fuzzy logic
                              model of intersection (warning: SLOW)
  -p [ --propincuity ]        Use Propincuity to calculate areas instead of 
                              exponential decay (warning: SLOW. Not 
                              recommended!)
  -o [ --output-file ] arg    sagemath output file name.
  --matrix-file arg           matrix output file name.
 ```
 
### Incorporating environmental variables.

One way would be to use not only geographical information, but to apply a PCA reduction to your environmental variables in order to produce output of dimension 3. See the example notebook PCA.ipynb (requires scikit-learn) to convert your data before feeding it to the software.

### Acknowledgements

This work was done by a group of researchers at *Escuela Nacional de Estudios Superiores unidad Morelia, Universidad Nacional Autónoma de México* with a research grant **PAPIIT IA106316**. For the complete list of authors, see the (upcoming) paper. All provided data was provided by César Andrés Torres Miranda.

If you find it useful, or if you have any comments, suggestions, feature requests, *etc.* feel free to email us at [mraggi@gmail.com](mailto:mraggi@gmail.com).
