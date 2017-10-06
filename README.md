# FuzzyLogicEcology
Construction of a digraph based on data points from observations of ecological distributions to calculate overlaps using fuzzy logic.

## Installing

First, you need to make sure you have the following dependencies:
```
g++ (or clang++)
eigen3 (or blaze)
boost
cmake
```

### Installing dependencies in debian/ubuntu/mint
For example, to install in debian/ubuntu, do `sudo apt-get install build-essentials libboost-all-dev cmake git eigen3`

Then, if you wish, you can install blaze manually. See [https://bitbucket.org/blaze-lib/blaze/wiki/Configuration%20and%20Installation]. On some machines, blaze is faster than eigen, but in some it isn't. If unsure, or if you don't need the absolute fastest, just use eigen.

### Installing dependencies in arch/chakra/manjaro
In a terminal, type:
`sudo pacman -S cmake boost git eigen3`

We provide a PKGBUILD for blaze. So download the provided PKGBUILD (inside "blaze" directory) to some directory, either with git or through the web interface.

Then, `cd` to said directory and type
```bash
    makepkg
    sudo pacman -U blaze-*
```

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

If you wish to use blaze instead of eigen, you can replace `cmake ..` by `cmake .. -DBUILD_USING_EIGEN=OFF` or `cmake .. -DBUILD_USING_BLAZE=ON`. 

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
