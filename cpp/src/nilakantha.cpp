#include "series_summer.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

double evaluate_nilakantha( int n ) {
  if (n == 0)
    return 3.0;
  else
    return pow(-1,n+1) * 4.0 / (2*n*(2*n+1)*(2*n+2));
};

int main( int argc, char ** argv ) {
 
  if( argc != 2 ) {
    std::cerr << "Invalid input: wrong number of arguments." << std::endl;
    std::cerr << "Usage: nilakantha <num_terms>" << std::endl;
    exit(1);
  };

  int num_terms;
 
  try {
    num_terms = std::stoi(argv[1]);
  }
  catch (...){
    std::cerr << "Invalide input: requires positive integer <num_terms>." << std::endl;
    std::cerr << "Usage: nilakantha <num_terms>" << std::endl;
    exit(1);
  }

  if( num_terms < 0 ) {
    std::cerr << "Invalide input: requires positive integer <num_terms>." << std::endl;
    std::cerr << "Usage: nilakantha <num_terms>" << std::endl;
  }

  int precision = 6;

  SumSeries( evaluate_nilakantha, num_terms, precision );

}
