/// \file normal.cpp
// to build debug case, to run stand alone:
// g++ -o normal -I../include -DDEBUG normal.cpp
#ifndef DEBUG
#define DEBUG 0
#endif

#include <math.h>

#ifdef __CPLUSPLUS__
extern "C" {
#endif

double normal_distribution_function( double x) {
  return exp( -x*x / 2) / sqrt( 2 * 3.141592653589793238462643383);
}

double fast_normal_distribution_function( double x) {
  double Z;
  if (x < 0) {
    Z = fast_normal_distribution_function( -x);
  } else {
    double b[6] = { 2.5052367, 1.2831204, .2264718, .1306469, -.0202490, .0039132};

    double t = x * x , tpoly;

    double poly = b[0];

    int i;

    tpoly = t;

    for (i=1; i<=5; i++) {
      poly += b[i] * tpoly;
      tpoly *= t;
    }

    Z = 1.0/poly;
  }

  return Z;
}

double cumulative_normal_distribution_function( double x) {
  double cdf;
  if (x < 0) {
    cdf = 1 - cumulative_normal_distribution_function( -x);
  } else {
    double b[6] = { 0, .319381530, -.356563782, 1.781477937, -1.821255978, 1.330274429};

    double p = .2316419;

    double t = 1 / (1+p*x), tpoly;

    double poly = 0, Z;

    int i;

    tpoly = t;

    for (i=1; i<=5; i++) {
      poly += b[i] * tpoly;
      tpoly *= t;
    }

    Z = normal_distribution_function( x);
      
    cdf = 1 - Z * poly;
  }

  return cdf;
}

#ifdef __CPLUSPLUS__
}
#endif


#if DEBUG
#include <stdio.h>

int main() {
  int I;
  double x, z, fast_z, P;
  for (I=0;I<20;I++) {
    x = I * .1;
    z = normal_distribution_function( x);
    fast_z = fast_normal_distribution_function( x);
    P = cumulative_normal_distribution_function( x);
    printf( "%lf %lf %lf %lf\n", x, z, fast_z, P);
  }

  return 0;
}
#endif
