/// \file americanfudge.cpp
#include <iostream>
#include <iomanip>

#include "simpleoption/americanfudge.h"
#include "simpleoption/GridSearch.h"
#include "simpleoption/NewtonRaphson.h"

inline double max2( double tmp1, double tmp2) {
  return (tmp1 > tmp2)? tmp1 : tmp2;
}

inline double max3( double tmp1, double tmp2, double tmp3) {
  return max2( tmp1, max2( tmp2, tmp3));
}

void american_option_fudge::init_calc_derived_attributes() const
{
  european_option_pair::init_calc_derived_attributes();
  
  _call_fudge = max3( _S - _K, _exp_alpha_r_t * _S - _exp_r_t * _K, 0) - max2( _exp_alpha_r_t * _S - _exp_r_t * _K, 0);
  _d_call_fudge_ds = (_call_fudge != 0)? (1-_exp_alpha_r_t) : 0;
  _d_call_fudge_dt = (_call_fudge != 0)? ( -(_alpha - _r)*_S*_exp_alpha_r_t + _r * _exp_r_t * _K) : 0;

  _put_fudge = max3( _K - _S, _exp_r_t * _K - _exp_alpha_r_t * _S, 0) - max2( _exp_r_t * _K - _exp_alpha_r_t * _S, 0);
  _d_put_fudge_ds = (_put_fudge != 0)? (_exp_alpha_r_t - 1) : 0;
  _d_put_fudge_dt = (_put_fudge != 0)? ( _r * _exp_r_t * _K - (_alpha - _r)*_S*_exp_alpha_r_t) : 0;

  _C       += _call_fudge;
  _dC_dS   += _d_call_fudge_ds;
  _dC_dtau += _d_call_fudge_dt;

  _P       += _put_fudge;
  _dP_dS   += _d_put_fudge_ds;
  _dP_dtau += _d_put_fudge_dt;
}

double american_option_fudge::call_intrinsic_value() const { 
  return max3( _S - _K, _exp_alpha_r_t * _S - _exp_r_t * _K, 0);
}

double american_option_fudge::put_intrinsic_value() const { 
  return max3( _K - _S, _exp_r_t * _K - _exp_alpha_r_t * _S, 0);
}

