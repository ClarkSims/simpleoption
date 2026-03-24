/// \file rote_simpleoption.cpp
#include <iostream>
#include "simpleoption/simpleoption.h"

const char *european_option_pair::_error_msg;
int european_option_pair::_erno;

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

int european_option_pair::check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma)
{
  #define Funcname  "european_option_pair::check_attributes"

  _error_msg = "";
  _erno = 0;

  CHECK(S <  0, __LINE__, Funcname);
  CHECK(K <  0, __LINE__, Funcname);
  CHECK(Tau <  0, __LINE__, Funcname);
  CHECK(Alpha <  0, __LINE__, Funcname);
  CHECK(R <  0, __LINE__, Funcname);
  CHECK(Sigma <  0, __LINE__, Funcname);

  return 0;
}

void european_option_pair::init_call_values( const european_option_pair& rhs)
{
  _C            = rhs._C;
  _dC_dS        = rhs._dC_dS;
  _d2C_dS_dtau  = rhs._d2C_dS_dtau;
  _d2C_dS2      = rhs._d2C_dS2;
  _d3C_dS2_dtau = rhs._d3C_dS2_dtau;
  _dC_dsigma    = rhs._dC_dsigma;
  _d2C_dsigma2  = rhs._d2C_dsigma2;
  _dC_dtau      = rhs._dC_dtau;
  _d2C_dtau2    = rhs._d2C_dtau2;
}

void european_option_pair::init_put_values( const european_option_pair& rhs)
{
  _P            = rhs._P;
  _dP_dS        = rhs._dP_dS;
  _d2P_dS_dtau  = rhs._d2P_dS_dtau;
  _d2P_dS2      = rhs._d2P_dS2;
  _d3P_dS2_dtau = rhs._d3P_dS2_dtau;
  _dP_dsigma    = rhs._dP_dsigma;
  _d2P_dsigma2  = rhs._d2P_dsigma2;
  _dP_dtau      = rhs._dP_dtau;
  _d2P_dtau2    = rhs._d2P_dtau2;
}
