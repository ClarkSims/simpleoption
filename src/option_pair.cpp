/// \file option_pair.cpp
/// \brief This file contains the non trivial member functions for class option_pair
#include "simpleoption/option_pair.h"

#include <iomanip>
#include <limits>

#include "simpleoption/Bisection.h"
#include "simpleoption/Secant.h"

const double option_pair::NA = std::numeric_limits<double>::quiet_NaN();

void option_pair::init_calc_derived_attributes() const
{
  if (_S==0) return;

  if (_tau == 0) {
    set_call_immediate_exercise();
    set_put_immediate_exercise();
    return;
  }
}

OPTIONPAIRFNC ostream& STDCALL operator << ( ostream& os, const option_pair& rhs) {
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
  return os;
}

double option_pair::C_sigma( double sigma) {
  if (sigma != _sigma) put_sigma( sigma);
  return _C;
}

double option_pair::P_sigma( double sigma) {
  if (sigma != _sigma) put_sigma( sigma);
  return _P;
}

void option_pair::set_call_immediate_exercise() const
{
  _C = _S - _K;
  if (_C > 0) {
    _dC_dS = 1.0;
  } else {
    _C = 0;
    _dC_dS = 0;
  }

  _d2C_dS2 = 0;
  _dC_dtau = 0;
  _dC_dsigma = 0;
}

void option_pair::set_put_immediate_exercise() const
{
  _P = _K - _S;
  if (_P > 0) {
    _dP_dS = -1.0;
  } else {
    _P = 0;
    _dP_dS = 0;
  }

  _d2P_dS2 = 0;
  _dP_dtau = 0;
  _dP_dsigma = 0;
}

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

int option_pair::strictly_check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma)
{
  #define Funcname  "option_pair::check_attributes"

  _error_msg = "";
  _erno = 0;

  CHECK(S <=  0, __LINE__, Funcname);
  CHECK(K <=  0, __LINE__, Funcname);
  CHECK(Tau <=  0, __LINE__, Funcname);
  CHECK(R <  0, __LINE__, Funcname);
  CHECK(Sigma <=  0, __LINE__, Funcname);
  CHECK(is_NA(Sigma), __LINE__, Funcname);

  return 0;
}

///\brief This is a wrapper for the secant algortihm. This function returns the implied volatility of a call. If there is an error, _C_implied_sigma is set to a NA = not a number, and an exception is thrown.
///
/// assumes: These key attributes have been defined:<ul>
/// <li> S, underlying price  </li>
/// <li> K, strike price  </li>
/// <li> alpha, the return of the underlying  </li>
/// <li> r, interest rate  </li>
/// <li> tau, the amount of time till expiration  </li>
/// </ul>
/// if the seed volatility is not provided, than a seed of .3 = 30% is used<br />
/// the call_price is assumed to be greater than the intrinsic value of the call. If not then an exception of calss domain_error is called<br />
/// throws: domain_error if a key attribute or parameter is illegal.<br />
///         range_error if the value doesn't converge.<br />
/// changes: all derived attributes<br />
double option_pair::call_implied_sigma(
				       double call_price, 
				       bool show_iterations) 
{
  int erno;

  erno = strictly_check_attributes(
      _S,
      _K,
      _tau,
      _alpha,
      _r,
      _sigma);

  if (erno) {
    _C_implied_sigma = NA;
    throw std::domain_error( _error_msg);
  }

  if (call_price < call_intrinsic_value()) {
    _C_implied_sigma = NA;
    throw std::domain_error( "call_price < call_intrinsic_value in option_pair::call_implied_sigma");
  }

  _input_call_price = call_price;

  double sigma0, sigma1, price_sigma0, price_sigma1;

  sigma0 = .01;
  sigma1 = .50;
  
  stradle_value(
		sigma0,
		sigma1,
		price_sigma0,
		price_sigma1,
		call_price,
		*this, 
		&option_pair::C_sigma,
		true,
		true,
		1,
		1e-5,
		1e5);
  
  Bisection_Secant< option_pair, double > 
    solution( 
	   sigma0, 
	   sigma1, 
	   price_sigma0,
	   price_sigma1,
	   call_price, 
	   .0001, 
	   .0001,
	   100,
	   *this, 
	   &option_pair::C_sigma);

  solution.do_iteration( show_iterations);

  if (!solution.get_converged()) {
    _C_implied_sigma = NA;
    throw std::domain_error( "sigma did not converge in option_pair::call_implied_sigma");
  }

  return solution.get_x_mid();
}



///\brief This is a wrapper for the secant algortihm. This function returns the implied volatility of a put. If there is an error, _P_implied_sigma is set to a NA = not a number, and an exception is thrown.
///
/// assumes: These key attributes have been defined:<ul>
/// <li> S, underlying price  </li>
/// <li> K, strike price  </li>
/// <li> alpha, the return of the underlying  </li>
/// <li> r, interest rate  </li>
/// <li> tau, the amount of time till expiration  </li>
/// </ul>
/// if the seed volatility is not provided, than a seed of .3 = 30% is used<br />
/// the put_price is assumed to be greater than the intrinsic value of the put. If not then an exception of calss domain_error is puted<br />
/// throws: domain_error if a key attribute or parameter is illegal.<br />
///         range_error if the value doesn't converge.<br />
/// changes: all derived attributes<br />
double option_pair::put_implied_sigma(
				       double put_price, 
				       bool show_iterations) 
{
  int erno;

  erno = strictly_check_attributes(
      _S,
      _K,
      _tau,
      _alpha,
      _r,
      _sigma);

  if (erno) {
    _P_implied_sigma = NA;
    throw std::domain_error( _error_msg);
  }

  if (put_price < put_intrinsic_value()) {
    _P_implied_sigma = NA;
    throw std::domain_error( "put_price < put_intrinsic_value in option_pair::put_implied_sigma");
  }

  _input_put_price = put_price;

  double sigma0, sigma1, price_sigma0, price_sigma1;

  sigma0 = .01;
  sigma1 = .50;
  
  stradle_value(
		sigma0,
		sigma1,
		price_sigma0,
		price_sigma1,
		put_price,
		*this, 
		&option_pair::P_sigma,
		true,
		true,
		1,
		1e-5,
		1e5);
  
  Bisection_Secant< option_pair, double > 
    solution( 
	   sigma0, 
	   sigma1, 
	   price_sigma0,
	   price_sigma1,
	   put_price, 
	   .0001, 
	   .0001,
	   100,
	   *this, 
	   &option_pair::P_sigma);

  solution.do_iteration( show_iterations);

  if (!solution.get_converged()) {
    _P_implied_sigma = NA;
    throw std::domain_error( "sigma did not converge in option_pair::put_implied_sigma");
  }

  return solution.get_x_mid();
}

void option_pair::solve_alpha_and_sigma(
				       double call_price, 
                                       double put_price, 
				       int ishow_iterations) 
{
  // aproxomate alpha from European put call parity
  double alpha = log(((call_price - put_price)*exp(-_r*_tau)+_K)/_S)/_tau;
  double dalpha_dC = 1/((call_price - put_price)*exp(-_r*_tau)+_K)/_tau*exp(-_r*_tau);
  double dalpha_dP = -dalpha_dC;

  bool show_iterations = (ishow_iterations>0);
  bool sub_show_iterations = (ishow_iterations>1);

  for (size_t i=0; i<10; ++i) {
    _alpha = alpha;
    if (show_iterations) {
      std::cout << i << " alpha = " << alpha << std::endl;
    }

    call_implied_sigma( call_price, sub_show_iterations);
    double call_sigma = _sigma;
    double dc_dsigma  = _dC_dsigma;

    put_implied_sigma( put_price, sub_show_iterations);
    double put_sigma = _sigma;
    double dp_dsigma  = _dP_dsigma;
  
    double dsigma = (call_sigma - put_sigma)*.5;
    
    double dC = -dc_dsigma * dsigma;
    double dP = dp_dsigma * dsigma;
  
    double dalpha = dC * dalpha_dC + dP * dalpha_dP;
    alpha += dalpha;
    if (dalpha < 1e-4) {
      break;
    }
  }
}

void option_pair::solve_forward_and_sigma(
				       double call_price, 
                                       double put_price, 
				       int ishow_iterations) 
{
  _S = (call_price - put_price) + _K;
  
  solve_alpha_and_sigma(
                        call_price, 
                        put_price, 
			ishow_iterations);

  _S *= exp( _alpha * _tau);
  _alpha = 0;
}
