#!/bin/bash

# If build dir exists, delete and create
if [ -d build ]; then rm -rf build; fi;
mkdir build

# Use clang for compilation
compiler=clang++

# Compile to objects and link to executable
$compiler -std=c++11 -c src/sum_series.cpp -o build/sum_series.o
$compiler -std=c++11 -c src/nilakantha.cpp -o build/nilakantha.o
$compiler -std=c++11 build/sum_series.o build/nilakantha.o -o build/nilakantha

# Build documentation
cd doc
#doxygen
