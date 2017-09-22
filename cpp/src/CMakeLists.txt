cmake_minimum_required(VERSION 2.8)
project(Nilakantha)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

set( SOURCE nilakantha.cpp series_summer.cpp )
set( HEADERS nilakantha.cpp series_summer.cpp )

add_executable( nilakantha ${SOURCE} ${HEADERS} )
