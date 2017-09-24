Project: Nilakantha Python
Author: Luke Paradis
Date: September, 2017
License: MIT

Description:
  Multihreaded Python project to sum/print finite number of terms of the Nilakantha series which approximates Pi. High level design is described in python docstrings in sum_series.py and implementatin details commented line by line in the same file.

Tested on:
  Mac OS 10.12.3 with Python 2.7.10
  Ubuntu 16.04.4 with Python 2.7.12

Files:
  sum_series.py - Function and thread classes to sum/print approximations of an infinite series.
  nilakantha.py - Runnable script using sum_series to print approximations of the Nilakantha series.

Run:
  ./nilakantha.py num_terms [precision]

Documentation:
  pydoc sum_series
