/// \file demo_inverse_cumulative_normal_distribution.cpp
/// \brief Shows how to generate pseudo random normally 
/// distributed variables.

#include "simpleoption/inverse_cumulative_normal_distribution.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>

using namespace std;

#define N 1000

int main() {
  double x, sum_x=0, sum_x2=0;
  int i;
  time_t t;

  srand( time( &t));

  for (i=0; i<N; ++i) {
    x = inverse_cumulative_normal_distribution::PseudoRandomNormalSample( rand);
    sum_x += x;
    sum_x2 += x*x;
    cout << x << endl;
  }

  double *z = new double[N];
  inverse_cumulative_normal_distribution::PseudoRandomNormalSample( rand, N, z);

  for (i=0; i<N; ++i) {
    sum_x += z[i];
    sum_x2 += z[i] * z[i];
    cout << z[i] << endl;
  }

  sum_x /= 2*N;
  sum_x2 /= 2*N;
  cout << "mean     = " << sum_x << endl;
  cout << "variance = " << sum_x2 << endl;
}


/// \mainpage
/// The class inverse_cumulative_normal_distribution contains a lookup table
/// which stores values of the inverse cumulative normal distribution function.
/// This table is generated compiling and running build_table_of_inversecumnorm_function.cpp.
/// This table can be used to quickly generate pseudo random normally distributed variables.
/// demo_inverse_cumulative_normal_distribution.cpp has an example of how to 
/// generate normally distributed variables.
///
/// Note that the source of the pseudo randomness, is a user defined functor,
/// which generates a uniformly distributed random variable.
///
/// Note also that these pseudo random normally distributed variables are an excellent
/// source for random variables in Monti Caro methods which need such variables.
