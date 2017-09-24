#!/bin/bash

# Build exectuable
if [ -d build ]; then rm -rf build; fi;
mkdir build
g++ -std=c++11 -c src/sum_series.cpp -o build/sum_series.o
g++ -std=c++11 -c src/nilakantha.cpp -o build/nilakantha.o
g++ -std=c++11 build/sum_series.o build/nilakantha.o -o build/nilakantha

# Build documentation
#cd doc
#doxygen