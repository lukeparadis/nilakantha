#!/usr/bin/env python

import math
import argparse
from sum_series import SumSeries

def evaluate_nilakantha(n):
    return 3 if n == 0 else math.pow(-1,n+1) * 4.0 / (2*n*(2*n+1)*(2*n+2))

if __name__ == '__main__':

    parser = argparse.ArgumentParser('Calculate Nilkantha series summation.')
    parser.add_argument('num_terms', type=int)
    parser.add_argument('precision', type=int, default=6)
    
    args = parser.parse_args()
    
    SumSeries(evaluate_nilakantha, args.num_terms, args.precision)

