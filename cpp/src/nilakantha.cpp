/*! \file nilakantha.cpp
 *  \Author Luke Paradis
 *  \date September, 2017
 *  \brief Main function to compute and print approximations of an Nilakantha series.
 */
#include "sum_series.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>

long double EvaluateNilakantha( int n ) {
  
  if (n == 0)
    // First term is 3.0
    return 3.0;
  else
    // Subsequent terms given by this expression
    return pow(-1,n+1) * 4.0 / (2.0*n*(2.0*n+1)*(2.0*n+2.0));
};

int main( int argc, char ** argv ) {
 
  // Number of terms in the approximation
  int num_terms = std::stoi(argv[1]);
  
  // Default precision set to 6 digits
  int precision = 6;

  long double sum = 0;
  for( int k = 0; k < num_terms; k++ )
    sum += EvaluateNilakantha(k);

  std::cout << std::setprecision(20) << sum << std::endl;

  // Optionally input number of digits for precision
  if( argc == 3 )
    precision = std::stoi(argv[2]);
  
  // Print out first num_terms approximations of Nilakantha series
  //SumSeries( EvaluateNilakantha, num_terms, precision );

  return 0;
}
