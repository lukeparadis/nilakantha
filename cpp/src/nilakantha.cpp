#include "sum_series.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

double EvaluateNilakantha( int n ) {
  if (n == 0)
    return 3.0;
  else
    return pow(-1,n+1) * 4.0 / (2*n*(2*n+1)*(2*n+2));
};

int main( int argc, char ** argv ) {

  int num_terms = std::stoi(argv[1]);
 
  int precision = 6;
  if( argc == 3 )
    precision = std::stoi(argv[2]);

  SumSeries( EvaluateNilakantha, num_terms, precision );

}
