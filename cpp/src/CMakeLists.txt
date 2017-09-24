cmake_minimum_required(VERSION 2.8)
project(Nilakantha)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

set( SOURCE nilakantha.cpp sum_series.cpp )
set( HEADERS sum_series.hpp )

add_executable( nilakantha ${SOURCE} ${HEADERS} )
