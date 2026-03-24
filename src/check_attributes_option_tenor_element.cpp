/// \file rote_option_tenor_element.cpp
#include <iostream>
#include "simpleoption/option_tenor_element.h"

const char *option_tenor_element::_error_msg;
int option_tenor_element::_erno;

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

int option_tenor_element::check_attributes(
      double K,
      double call_price,
      double put_price)
{
  #define Funcname  "option_tenor_element::check_attributes"

  _error_msg = "";
  _erno = 0;

  CHECK( K <  0, __LINE__, Funcname);
  CHECK( call_price <  0 && call_price!=-1, __LINE__, Funcname);
  CHECK( put_price <  0 && put_price!=-1, __LINE__, Funcname);

  return 0;
}

void option_tenor_element::init_calc_derived_attributes() const {
  if (_call_price == -1 || _put_price == -1) {
    _straddle_price = -1;
  } else {
    _straddle_price = _call_price + _put_price;
  }
}
