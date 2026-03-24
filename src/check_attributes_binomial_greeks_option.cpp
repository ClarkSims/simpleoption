/// \file rote_binomial_greeks_option.cpp
/// This file contains the rote member functions of class binomial_greeks_option. Almost all of the functions are virtual, so that child classes can implement similar algorithms.
#include <iostream>
#include "simpleoption/binomial_greeks_option.h"

const char *binomial_greeks_option::_error_msg;
int binomial_greeks_option::_erno;

#ifdef CHECK
#undef CHECK
#endif
#ifdef linux
#define CHECK(y,z,a) if (y) { _error_msg = __STRING(y)" at line "\
  __STRING(z)" in " a "::check_attributes"; \
  return (_erno = z);}
#else
#define CHECK(y,z,a) if (y) {_error_msg = #y" at line "\
  #z" in " #a"::check_attributes"; \
  return (_erno = z);}
#endif

int binomial_greeks_option::check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma,
      int NumberIterations,
      int vega_method,
      int skip_calculations)
{
  #define Funcname  "binomial_greeks_option::check_attributes"

  _error_msg = "";
  _erno = 0;

  CHECK(S <  0, __LINE__, Funcname);
  CHECK(K <  0, __LINE__, Funcname);
  CHECK(Tau <  0, __LINE__, Funcname);
  CHECK(Alpha <  0, __LINE__, Funcname);
  CHECK(R <  0, __LINE__, Funcname);
  CHECK(Sigma <  0, __LINE__, Funcname);
  CHECK(vega_method<0 && vega_method>2, __LINE__, Funcname);

  return 0;
}
