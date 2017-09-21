import math
import argparse
from series_summer import SeriesSummer

def evaluate_nilakantha(n):
    return math.pow(-1,n+1) * 4.0 / (2*n*(2*n+1)*(2*n+2))

if __name__ == '__main__':

    parser = argparse.ArgumentParser('Calculate Nilkantha series summation.')
    parser.add_argument('num_terms', type=int)
    parser.add_argument('--precision', type=int, default=6)
    
    args = parser.parse_args()
    
    data = { 0: 3.0 } 

    nilakantha_summer = SeriesSummer(evaluate_nilakantha, args.num_terms, args.precision, data)
    nilakantha_summer.run()

