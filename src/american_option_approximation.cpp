/// \file american_option_approximation.cpp
#include <iostream>
#include <iomanip>

#include "simpleoption/american_option_approximation.h"
#include "simpleoption/GridSearch.h"

void american_option_approximation::init_calc_derived_attributes() const
{
  european_option_pair::init_calc_derived_attributes();

  if ((_alpha - _r) >= 0) {
    _call_early_exercise = false;
    _put_early_exercise = true;
  } else {
    _call_early_exercise = true;
    _put_early_exercise = true;
  }

  _optvar.init( dynamic_cast<const european_option_pair&>( *this));

  _put_tau = _call_tau = _tau;

  if (_call_early_exercise) {
    GridSearch< european_option_pair, double > path(
						    100,
						    0,
						    .01*_tau,
						    _optvar,
						    &european_option_pair::C_tau);


    path.do_iteration( false);
      
    _optvar.put_tau( _call_tau = path.get_x_max());
    set_call( _optvar); 
  }

  if (_put_early_exercise) {
    GridSearch< european_option_pair, double > path(
						    100,
						    0,
						    .01*_tau,
						    _optvar,
						    &european_option_pair::P_tau);


    path.do_iteration( false);
      
    _optvar.put_tau( _put_tau = path.get_x_max());
    set_put( _optvar); 
  }
}

void american_option_approximation::set_call( const european_option_pair& rhs) const
{
  _C = rhs._C;
  _dC_dS = rhs._dC_dS;
  _d2C_dS_dtau = rhs._d2C_dS_dtau;
  _d2C_dS2 = rhs._d2C_dS2;
  _d3C_dS2_dtau = rhs._d3C_dS2_dtau;
  _dC_dsigma = rhs._dC_dsigma;
  _d2C_dsigma2 = rhs._d2C_dsigma2;
  _dC_dtau = rhs._dC_dtau;
  _d2C_dtau2 = rhs._d2C_dtau2;
}

void american_option_approximation::set_put( const european_option_pair& rhs) const
{
  _P = rhs._P;
  _dP_dS = rhs._dP_dS;
  _d2P_dS_dtau = rhs._d2P_dS_dtau;
  _d2P_dS2 = rhs._d2P_dS2;
  _d3P_dS2_dtau = rhs._d3P_dS2_dtau;
  _dP_dsigma = rhs._dP_dsigma;
  _d2P_dsigma2 = rhs._d2P_dsigma2;
  _dP_dtau = rhs._dP_dtau;
  _d2P_dtau2 = rhs._d2P_dtau2;
}

AMERICANAPPROXFNC ostream& STDCALL operator << ( ostream& os, const american_option_approximation& rhs) {
  os << "S         = " << setprecision(3) << rhs._S << endl;
  os << "K         = " << setprecision(3) << rhs._K << endl;
  os << "Tau       = " << rhs._tau << endl;
  os << "alpha     = " << rhs._alpha << endl;
  os << "r         = " << rhs._r << endl;
  os << "sigma     = " << rhs._sigma << endl;
  os << "C         = " << rhs._C << endl;
  os << "P         = " << rhs._P << endl;
  os << "dC_dS     = " << rhs._dC_dS << endl;
  os << "dP_dS     = " << rhs._dP_dS << endl;
  os << "d2C_dS2   = " << rhs._d2C_dS2 << endl;
  os << "d2P_dS2   = " << rhs._d2P_dS2 << endl;
  os << "dC_dsigma = " << rhs._dC_dsigma << endl;
  os << "dP_dsigma = " << rhs._dP_dsigma << endl;
  os << "dC_dtau   = " << rhs._dC_dtau << endl;
  os << "dP_dtau   = " << rhs._dP_dtau << endl;
  os << "d2P_dtau2 = " << rhs._d2P_dtau2 << endl;
  os << "call_tau  = " << rhs._call_tau << endl;
  os << "put_tau   = " << rhs._put_tau << endl;

  return os;
}

double american_option_approximation::call_intrinsic_value() const { 
  double tmp = _S - _K;
  tmp = (tmp>0)? tmp : 0;

  double tmp2 = _S*exp((_alpha - _r)*_tau) - _K * exp( -_r * _tau);
  return (tmp2 > tmp) ?  tmp2 : tmp;
}

double american_option_approximation::put_intrinsic_value() const { 
  double tmp = _K - _S;
  tmp = (tmp>0)? tmp : 0;

  double tmp2 =  _K * exp( -_r * _tau) - _S*exp((_alpha - _r)*_tau);
  return (tmp2 > tmp) ?  tmp2 : tmp;
}
