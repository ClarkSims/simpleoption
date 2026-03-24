/// \file simpleoption.cpp
#include <iomanip>
#include "simpleoption/simpleoption.h"
#include "simpleoption/normal.h"
#include "simpleoption/Secant.h"

void european_option_pair::init_calc_derived_attributes() const
{
  option_pair::init_calc_derived_attributes();

  if (_S==0 || _tau==0) return;

  _mu = _alpha - _sigma * _sigma * .5;
  _sqrt_tau = sqrt( _tau);
  _sigma_sqrt_tau = _sigma * _sqrt_tau;

  _log_S_K = log( _S / _K);
  _Zk = (_log_S_K + _mu * _tau) / _sigma_sqrt_tau;
  _h  = _Zk + _sigma_sqrt_tau;  

  _P_h = fast_normal_distribution_function( _h);

  //_dh_dS = 1 / (_S * _sigma_sqrt_tau);

  _dZk_dtau = -.5 * _Zk / _tau  + _mu / _sigma_sqrt_tau;
  _dh_dtau = _dZk_dtau + .5 * _sigma_sqrt_tau / _tau; 

  _N_h  = cumulative_normal_distribution_function( _h);
  _N_Zk = cumulative_normal_distribution_function( _Zk);

  _exp_alpha_r_t = exp( (_alpha - _r)*_tau);
  _exp_r_t = exp( -_r * _tau);
  
  _S_exp_alpha_r_t = _S * _exp_alpha_r_t;
  _K_exp_r_t = _K * _exp_r_t;

  _C = _S_exp_alpha_r_t * _N_h - _K_exp_r_t * _N_Zk;
  _P  = _C - _S * _exp_alpha_r_t + _K * _exp_r_t;

  _dC_dS = _N_h * _exp_alpha_r_t;
  _dP_dS = _dC_dS -  _exp_alpha_r_t;

  _d2C_dS_dtau =  (_alpha - _r ) * _dC_dS  +   _P_h * (_dh_dtau) * _exp_alpha_r_t;

  _d2P_dS_dtau = _d2C_dS_dtau - (_alpha - _r) * _exp_alpha_r_t;

  _d2C_dS2 = _exp_alpha_r_t *  _P_h / (_S * _sigma_sqrt_tau);
  _d2P_dS2 = _d2C_dS2;

  _d3C_dS2_dtau = (_alpha - _r) * _d2C_dS2 +   _P_h * (-_h * _dh_dtau) / (_S * _sigma_sqrt_tau) - .5 / _tau * _d2C_dS2;

  //_d3C_dS3 = - (_exp_alpha_r_t / _sigma_sqrt_tau) *(  1 / _S  * h * _dh_ds * _P_h  + 1 / (_S * _S) * _P_h);
  
  _dh_dsigma = - (_log_S_K + _alpha * _tau) / ( _sigma * _sigma_sqrt_tau);
  _dZ_dsigma = _dh_dsigma - _sqrt_tau;

  _dC_dsigma = _S * _sqrt_tau * _exp_alpha_r_t * _P_h;
  _dP_dsigma = _dC_dsigma;

  _d2C_dsigma2 = - _h *_dC_dsigma;
  _d2P_dsigma2 = _d2C_dsigma2;

  _dC_dtau = -_r * _C + _alpha * _S * _dC_dS + _sigma * _sigma * _S * _S * .5 * _d2C_dS2;
  _dP_dtau = _dC_dtau - (_alpha - _r) * _exp_alpha_r_t * _S - _r * _exp_r_t * _K;

  _d2C_dtau2 = -_r * _dC_dtau  +  _alpha * _S * _d2C_dS_dtau  +  _sigma * _sigma * _S * _S * .5 * _d3C_dS2_dtau;

  _d2P_dtau2 = _d2C_dtau2 - (_alpha - _r) * (_alpha - _r) *_exp_alpha_r_t * _S - _r * _r * _exp_r_t * _K;
}

SIMPOPTFNC ostream& STDCALL operator << ( ostream& os, const european_option_pair& rhs) {
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
  return os;
}

double european_option_pair::dC_dt_tau( double tau) {
  if (tau != _tau) put_tau( tau);
  return _dC_dtau;
}

double european_option_pair::d2C_dt2_tau( double tau) {
  if (tau != _tau) put_tau( tau);
  return _d2C_dtau2;
}

double european_option_pair::dP_dt_tau( double tau) {
  if (tau != _tau) put_tau( tau);
  return _dP_dtau;
}

double european_option_pair::d2P_dt2_tau( double tau) {
  if (tau != _tau) put_tau( tau);
  return _d2P_dtau2;
}

double european_option_pair::C_tau( double tau) {
  if (tau != _tau) put_tau( tau);
  return _C;
}

double european_option_pair::P_tau( double tau) {
  if (tau != _tau) put_tau( tau);
  return _P;
}
double european_option_pair::dC_dsigma( double sigma) {
  if (sigma != _sigma) put_sigma( sigma);
  return _dC_dsigma;
}

double european_option_pair::d2C_dsigma2( double sigma) {
  if (sigma != _sigma) put_sigma( sigma);
  return _d2C_dsigma2;
}

double european_option_pair::dP_dsigma( double sigma) {
  if (sigma != _sigma) put_sigma( sigma);
  return _dP_dsigma;
}

double european_option_pair::d2P_dsigma2( double sigma) {
  if (sigma != _sigma) put_sigma( sigma);
  return _d2P_dsigma2;
}

void european_option_pair::set_call_immediate_exercise() const
{
  _C = _S - _K;
  if (_C > 0) {
    _dC_dS = 1.0;
  } else {
    _C = 0;
    _dC_dS = 0;
  }

  _d2C_dS_dtau = 0;
  _d2C_dS2 = 0;
  _d3C_dS2_dtau = 0;
  _dC_dsigma = 0;
  _d2C_dsigma2 = 0;
  _dC_dtau = 0;
  _d2C_dtau2 = 0;
}

void european_option_pair::set_put_immediate_exercise() const
{
  _P = _K - _S;
  if (_P > 0) {
    _dP_dS = -1.0;
  } else {
    _P = 0;
    _dP_dS = 0;
  }

  _d2P_dS_dtau = 0;
  _d2P_dS2 = 0;
  _d3P_dS2_dtau = 0;
  _dP_dsigma = 0;
  _d2P_dsigma2 = 0;
  _dP_dtau = 0;
  _d2P_dtau2 = 0;
}

