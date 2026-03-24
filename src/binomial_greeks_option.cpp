/// \file binomial_greeks_option.cpp
/// This file contains the non trivial member functions of class binomial_option. Almost all of the functions are virtual, so that child classes can implement similar algorithms.
#include "simpleoption/binomial_greeks_option.h"

/// \brief: This function calculates the values of the American Put and the American Call. It calls several virtual function. The function are virtual, so that child classes can implement different versions of the algorithm. The final value of the American call is stored as C, and the final value of the American put is stored as P.
///
/// assumptions: alpha, r, tau, NumberIncrement, S, and K all have reasonable values<br />
/// changes: the values of all derived attributes, including C the call price, and P the put price<br />
//  description:  This function does most of its work by calling binomial_option::init_calc_derived_attributes(), which in turn calls the
///   virtual functions allocate_price_vectors, get_grid_offset, get_max_j, and get_min_j. After the price grid is solved, the derivatives
///   are calculated by calling solve_derivatives()
void binomial_greeks_option::init_calc_derived_attributes() const {
  if (!_skip_calculations) {
    binomial_option::init_calc_derived_attributes();

    solve_derivatives();
  }
}
    
/// \brief This function allocates the arrays for storing call prices, put prices, and S prices. It is organized as a C-style square matrix, of dimensions (NumberIterations+1). The smaller index, is for price, so _S_grid[i*_Numberiterations + j] represents the price at time of, of the j'th element of price. This function is virtual, so that it can be overloaded for other pricing algorithms, such as grid relaxation.
///
/// assumptions: _NumberIterations > 0<br />
/// changes: _call_grid, _put_grid, _S_grid.<br />
/// returns: nothing.<br />
/// description: This function allocates the arrays for storing call prices, put prices, and S prices. It is organized as a C-style square matrix, of dimensions (NumberIterations+1). The smaller index, is for price, so _S_grid[i*_Numberiterations + j] represents the price at time of, of the j'th element of price. This function is virtual, so that it can be overloaded for other pricing algorithms, such as grid relaxation.
void binomial_greeks_option::allocate_price_vectors() const {
  unsigned int sz = (_NumberIterations+3) * (_NumberIterations+1);
  if (_call_grid.size() != sz) {
    std::vector<double> foo( sz); foo.swap( _call_grid);
  }
  if (_put_grid.size() != sz) {
    std::vector<double> foo( sz); foo.swap( _put_grid);
  }
  if (_S_grid.size() != sz) {
    std::vector<double> foo( sz); foo.swap( _S_grid);
  }
}

/// \brief This function takes two subscripts, and maps it to a single subscript. It enables a two dimensional matrix to be stored as a one dimensional array. This is a non standard matrix, in that j has a minimum value of -1. This function has the same sceme as binomial_option::get_grid_offset except there is an extra row of data at each end.
///
/// assumption: NumberIterations > 0 <br />
/// returns: the offset of the tuple (i, j)<br />
/// changes: nothing<br />
/// description: This function takes two subscripts, and maps it to a single subscript. It enables a two dimensional matrix to be stored 
/// as a one dimensional array. It uses the non standard C-style storage. K = i * (NumberIncrement+3) + j + 1, where i is the offset 
/// in time, j is the price offset, and NumberIncrement+3 * NumberIncremetn+1, is the dimension of the rectangular matrix. Note that 
/// the get_grid_offset( 0, 0),  returns 1, and get_grid_offset( 0, -1), returns 0.  This allows the first and second derivatives of
/// price to be calculated.
int binomial_greeks_option::get_grid_offset( int i, int j) const {
  return (i*(_NumberIterations+3)+j + 1);
}

/// \brief This function overrides the function binomial_option::get_min_j. This allows 2 extra rows of data to be calculated. The 
///   address scheme is described in more detail in binomial_greeks_option::get_grid_offset
///
///  Assumptions: NumberIterations > 0<br />
///  Returns: the minimum allowed of the price subscript, j, for a given time subscript, i. <br />
///  Changes: nothing<br />
///  Description:This function returns the minimum value of j, at time i.This function overrides the function binomial_option::get_min_j.
///  This allows 2 extra rows of data to be calculated. The address scheme is described in more detail in 
///  binomial_greeks_option::get_grid_offset
int binomial_greeks_option::get_min_j( int i) const {
  return -1;
}


/// \brief This function overrides the function binomial_option::get_max_j. This allows 2 extra rows of data to be calculated. The 
///   address scheme is described in more detail in binomial_greeks_option::get_grid_offset
///
///  Assumptions: NumberIterations > 0<br />
///  Returns: the maximum allowed of the price subscript, j, for a given time subscript, i. <br />
///  Changes: nothing<br />
///  Description:This function returns the maximum value of j, at time i.This function overrides the function binomial_option::get_max_j.
///  This allows 2 extra rows of data to be calculated. The address scheme is described in more detail in 
///  binomial_greeks_option::get_grid_offset
int binomial_greeks_option::get_max_j( int i) const {
  return i+1;
}

void binomial_greeks_option::solve_derivatives() const {
  if (_calc_call) {
    if (_C > call_intrinsic_value()) {
      _dC_dS = (_call_grid[2] - _call_grid[0]) / (_S_grid[2] - _S_grid[0]);
      _d2C_dS2 = (_call_grid[2] - 2*_call_grid[1] + _call_grid[0]) / (_S_grid[2] - _S_grid[1]) / (_S_grid[1] - _S_grid[0]);
      _dC_dtau =  -_r * _C + _alpha * _S * _dC_dS + _sigma * _sigma * _S * _S * .5 * _d2C_dS2;
    } else {
      if (_C == 0) {
	_dC_dS = 0;
      } else if (_C == (_S - _K)) {
	_dC_dS = 1;
      } else {
	_dC_dS = exp((_alpha - _r)*_tau);
      }
      _d2C_dS2 = 0;
      _dC_dtau = 0;
    }
  }

  if (_calc_put) {
    if (_P > put_intrinsic_value()) {
      _dP_dS = (_put_grid[2] - _put_grid[0]) / (_S_grid[2] - _S_grid[0]);
      _d2P_dS2 = (_put_grid[2] - 2*_put_grid[1] + _put_grid[0]) / (_S_grid[2] - _S_grid[1]) / (_S_grid[1] - _S_grid[0]);
      _dP_dtau =  -_r * _P + _alpha * _S * _dP_dS + _sigma * _sigma * _S * _S * .5 * _d2P_dS2;
    } else {
      if (_P == 0) {
	_dP_dS = 0;
      } else if (_P == (_K - _S)) {
	_dP_dS = -1;
      } else {
	_dP_dS = -exp((_alpha - _r)*_tau);
      }
      _d2P_dS2 = 0;
      _dP_dtau = 0;
    }
  }

  if (_vega_method == 1) {
    numerical_approximate_dX_dsigma();
  } else if (_vega_method == 2) {
    fast_approximate_dX_dsigma();
  }
}

void binomial_greeks_option::fast_approximate_dX_dsigma() const {
  _exp_alpha_r_t  = (_alpha == _r)? 1:exp( (_alpha - _r)*_tau);
  _sqrt_tau       = sqrt( _tau);
  _sigma_sqrt_tau = _sigma * _sqrt_tau;

  _P_h_call       = _d2C_dS2 * _S * _sigma_sqrt_tau / _exp_alpha_r_t;
  _dC_dsigma      = _S * sqrt( _tau) * _exp_alpha_r_t * _P_h_call;

  _P_h_put        =  _d2P_dS2 * _S * _sigma_sqrt_tau / _exp_alpha_r_t;
  _dP_dsigma      = _S * sqrt( _tau) * _exp_alpha_r_t * _P_h_put;
}

void binomial_greeks_option::numerical_approximate_dX_dsigma( double dsigma) const 
{
  if (dsigma == 0) {
    dsigma = _sigma / _NumberIterations;
  }
  _del_sigma = dsigma;
  _vega_temp.init( _S, _K, _tau, _alpha, _r, _sigma+dsigma, _NumberIterations);
  if (_calc_call) {
    _dC_dsigma = (_vega_temp.get_C() - _C)/_del_sigma;
  }
  if (_calc_put) {
    _dP_dsigma = (_vega_temp.get_P() - _P)/_del_sigma;
  }
}
