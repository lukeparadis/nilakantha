#!/usr/bin/env python
"""Runnable script for computing Nilakantha approximations of PI."""

import argparse
import math
from sum_series import SumSeries 

def evaluate_nilakantha(n):
    """Evaluate the Nilakantha series at `n`"""
    return 3 if n == 0 else math.pow(-1,n+1) * 4.0 / (2*n*(2*n+1)*(2*n+2))

if __name__ == '__main__':

    # Create parser with description
    parser = argparse.ArgumentParser('Calculate Nilkantha series summation.')

    # Add int argument for number of terms
    parser.add_argument('num_terms', type=int)

    # Add optional int argument for printing precision, defaulting to 6
    parser.add_argument('precision', type=int, default=6)
    
    # Parse command line input, and store in `args`
    args = parser.parse_args()
    
    # Print approximations of Nilkantha series
    SumSeries(evaluate_nilakantha, args.num_terms, args.precision)

