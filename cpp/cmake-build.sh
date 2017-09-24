#!/bin/bash

# If build dir exists, delete and create
if [ -d build ]; then rm -rf build; fi;
mkdir build

# Standard cmake out-of-source build
cd build
cmake ../src
make

# Make docs
cd ../doc
doxygen
