/// \file normal.h
#ifndef normal_h
#define normal_h

#ifdef __CPLUSPLUS__
extern "C" {
#endif

double normal_distribution_function( double x);

double fast_normal_distribution_function( double x);

double cumulative_normal_distribution_function( double x);

#ifdef __CPLUSPLUS__
}
#endif

#endif
