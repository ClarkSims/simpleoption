/// \file rote_option_tenor.cpp
#include <iostream>
#include "simpleoption/option_tenor.h"

const char *option_tenor::_error_msg;
int option_tenor::_erno;

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

int option_tenor::check_attributes(
      double S,
      double Alpha,
      double R,
      double Tau,
      double K_min,
      double K_inc,
      size_t N_strikes,
      size_t N_binomial)
{
  #define Funcname  "option_tenor::check_attributes"

  _error_msg = "";
  _erno = 0;

  CHECK(S <  0, __LINE__, Funcname);
  CHECK(Tau <  0, __LINE__, Funcname);
  CHECK(R <  0, __LINE__, Funcname);
  CHECK(K_min < 0, __LINE__, Funcname);
  CHECK(K_inc < 0, __LINE__, Funcname);
  CHECK(N_strikes < 0, __LINE__, Funcname);

  return 0;
}
