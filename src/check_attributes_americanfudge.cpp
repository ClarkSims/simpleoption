/// \file rote_americanfudge.cpp
#include <iostream>
#include "simpleoption/americanfudge.h"

const char *american_option_fudge::_error_msg;
int american_option_fudge::_erno;

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

int american_option_fudge::check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma)
{
  #define Funcname "american_option_fudge::check_attributes"

  _error_msg = "";
  _erno = 0;

  CHECK(S <  0, __LINE__, Funcname);
  CHECK(K <  0, __LINE__, Funcname);
  CHECK(Tau <  0, __LINE__, Funcname);
  CHECK(R <  0, __LINE__, Funcname);
  CHECK(Sigma <  0, __LINE__, Funcname);

  return 0;
}

