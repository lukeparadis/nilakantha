#!/bin/bash

# If build dir exists, delete and create
if [ -d build ]; then rm -rf build; fi;
mkdir build

# Use clang for compilation
compiler=clang++

# Use c++11 standard and compile for multithreading
compiler_flags="-std=c++11 -pthread"

# Compile to objects and link to executable
$compiler $compiler_flags -c src/sum_series.cpp -o build/sum_series.o
$compiler $compiler_flags -c src/nilakantha.cpp -o build/nilakantha.o
$compiler $compiler_flags build/sum_series.o build/nilakantha.o -o build/nilakantha

# Build documentation
cd doc
#doxygen
