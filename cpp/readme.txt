Project: Nilakantha C++
Author: Luke Paradis
Date: September, 2017
License: MIT

Description:
  Multithreaded C++ project to sum/print finite number of terms of the Nilakantha series which approximates Pi. High level design is described in doxygen commenting in src/sum_series.hpp and implementation details commented line by line in src/sum_series.cpp.

Tested on:
  Mac OS 10.12.3 with clang-800.0.42.1
  Ubuntu 16.04 with g++ 

Files:
  src/
    sum_series.cpp/hpp - functions for multithreaded computing of approximations of an infinite series.
    nilakantha.cpp - source for executable using sum_series to compute/print approximations of Nilakantha series.
    CMakeLists.txt - cmake project file if you choose to use cmake to build.
  doc/Doxyfile - doxygen project file for building documentation
  build.sh - script to build with standard compiler (g++/clang)
  cmake-build.sh - script to build using cross platform make (cmake).

Build: 
  ./build.sh or ./cmake-build.sh

Run:
  ./build/nilakantha num_terms [precision]

Documentation:
  doc/html/index.html
