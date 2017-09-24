#!/bin/bash

dst=luke_paradis_nilakantha
if [ -d $dst ]; then rm -rf $dst; fi;
mkdir $dst

cp README.txt $dst

mkdir $dst/python
cp python/nilakantha.py $dst/python
cp python/sum_series.py $dst/python
cp python/README.txt $dst/python

mkdir $dst/cpp
cp -r cpp/src $dst/cpp/
cp cpp/build.sh $dst/cpp
cp cpp/cmake-build.sh $dst/cpp
mkdir $dst/cpp/doc
cp cpp/doc/Doxyfile $dst/cpp/doc
