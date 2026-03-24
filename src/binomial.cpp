/// \file binomial.cpp
/// \brief This file contains the non trivial member functions of class binomial_option. Almost all of the functions are virtual, so that child classes can implement similar algorithms.

#include "simpleoption/americanfudge.h"
#include "simpleoption/american_option_approximation.h"
#include "simpleoption/binomial.h"
#include "simpleoption/Bisection.h"

/// \brief: This function calculates the values of the American Put and the American Call. It calls several virtual function. The function are virtual, so that child classes can implement different versions of the algorithm. The final value of the American call is stored as C, and the final value of the American put is stored as P.
///
/// assumptions: alpha, r, tau, NumberIncrement, S, and K all have reasonable values<br />
/// changes: the values of all derived attributes, including C the call price, and P the put price<br />
/// description:  This function calculates the values of the American Put and the American Call. It calls several virtual function. The function are virtual, so that child classes can implement different versions of the algorithm. The final value of the American call is stored as C, and the final value of the American put is stored as P.
void binomial_option::init_calc_derived_attributes() const {
  _dt = _tau / _NumberIterations;
  _u = _alpha * _dt + log( 1 + sqrt( exp( _sigma * _sigma * _dt) - 1));
  _v = _alpha * _dt + log( 1 - sqrt( exp( _sigma * _sigma * _dt) - 1));
  _Scap = _u - _v;
  _Ccap = _v;

  //cout << .5*(exp(_u) + exp(_v)) << " " << exp( _alpha * _dt) << endl;
  
  allocate_price_vectors();

  solve_call_put();
}

/// \brief This function calculates the price at a given point of the price grid. It is a virtual function so that it can be overloaded for other similar solutions for American option pricing.
///
/// assumptions: i>=0, i<=NumberIncrement  j>=get_min_j() and j<=get_max_j()<br />
/// changes: nothing<br />
/// returns: The price of the stock at element, (i,j) of the price grid</br />
/// description:This function calculates the price at a given point of the price grid. It is a virtual function so that it can be overloaded for other similar solutions for American option pricing.
double binomial_option::S( int i, int j) const {
  return _S * exp( i * _Ccap + j * _Scap);
}
    
/// \brief This function allocates the arrays for storing call prices, put prices, and S prices. It is organized as a C-style square matrix, of dimensions (NumberIterations+1). The smaller index, is for price, so _S_grid[i*_Numberiterations + j] represents the price at time of, of the j'th element of price. This function is virtual, so that it can be overloaded for other pricing algorithms, such as grid relaxation.
///
/// assumptions: _NumberIterations > 0<br />
/// changes: _call_grid, _put_grid, _S_grid.<br />
/// returns: nothing.<br />
/// description: This function allocates the arrays for storing call prices, put prices, and S prices. It is organized as a C-style square matrix, of dimensions (NumberIterations+1). The smaller index, is for price, so _S_grid[i*_Numberiterations + j] represents the price at time of, of the j'th element of price. This function is virtual, so that it can be overloaded for other pricing algorithms, such as grid relaxation.
void binomial_option::allocate_price_vectors() const {
  unsigned int sz = (_NumberIterations+1) * (_NumberIterations+1);
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

/// \brief This function takes two subscripts, and maps it to a single subscript. It enables a two dimensional matrix to be stored as a one dimensional array. It uses the standard C-style storage. K = i * (NumberIncrement+1) + j, where i is the offset in time, j is the price offset, and NumberIncrement+1, is the dimension of the square matrix.
///
/// assumption: NumberIterations > 0 <br />
/// returns: the offset of the tuple (i, j)<br />
/// changes: nothing<br />
/// description: This function takes two subscripts, and maps it to a single subscript. It enables a two dimensional matrix to be stored as a one dimensional array. It uses the standard C-style storage. K = i * (NumberIncrement+1) + j, where i is the offset in time, j is the price offset, and NumberIncrement+1, is the dimension of the square matrix.
int binomial_option::get_grid_offset( int i, int j) const {
  return (i*(_NumberIterations+1)+j);
}

/// \brief This function returns the minimum value of j, at time i. This function allows loops of this form to execute: <pre>
///   for (i=NumberIncrement;i>=0;i==) { 
///      for (j=get_min_j(); j<=get_max_j(); j++) {
///        use an grid algorithm to solve for option price
///      }
///   }
///   </pre>
///  This function is virtual so that different grid relaxation algorithms can be implemented in child classes.
///
///  Assumptions: NumberIterations > 0<br />
///  Returns: the minimum allowed of the price subscript, j, for a given time subscript, i. <br />
///  Changes: nothing<br />
///  Description:This function returns the minimum value of j, at time i. This function allows loops of this form to execute: <pre>
///   for (i=NumberIncrement;i>=0;i==) { 
///      for (j=get_min_j(); j<=get_max_j(); j++) {
///        use an grid algorithm to solve for option price
///      }
///   }
///   </pre>
///  This function is virtual so that different grid relaxation algorithms can be implemented in child classes.
int binomial_option::get_min_j( int i) const {
  return 0;
}

/// \brief This function returns the maximum value of j, at time i. This function allows loops of this form to execute: <pre>
///   for (i=NumberIncrement;i>=0;i==) { 
///      for (j=get_min_j(); j<=get_max_j(); j++) {
///        use an grid algorithm to solve for option price
///      }
///   }
///   </pre>
///  This function is virtual so that different grid relaxation algorithms can be implemented in child classes.
///
///  Assumptions: NumberIterations > 0<br />
///  Returns: the maximum allowed of the price subscript, j, for a given time subscript, i. <br />
///  Changes: nothing<br />
///  Description:This function returns the maximum value of j, at time i. This function allows loops of this form to execute: <pre>
///   for (i=NumberIncrement;i>=0;i==) { 
///      for (j=get_min_j(); j<=get_max_j(); j++) {
///        use an grid algorithm to solve for option price
///      }
///   }
///   </pre>
///  This function is virtual so that different grid relaxation algorithms can be implemented in child classes.
int binomial_option::get_max_j( int i) const {
  return i;
}

/// \brief This function does the classic binomial options calculation.
///
/// assumptions: alpha, r, tau, NumberIncrement, S, and K all have reasonable values.<br />
/// changes: the values of the price grids, several temporary variables and the values of C and P.<br />
/// description: This function does the classic binomial options calculation.<br />
void binomial_option::solve_call_put() const {
  int i, j, k;
  int l, m;
  double intrinsic_value, tau, exp_alpha_r_tau, exp_r_tau;
   
  _exp_r_t = exp( -_r * _dt);

  i = _NumberIterations;
  for (j=get_min_j(i); j<=get_max_j(i); j++) {
    k = get_grid_offset( i, j);
    _S_grid[ k] = S( i, j);
    _call_grid[ k] = (_S_grid[k] > _K)? (_S_grid[k] - _K):0;
    _put_grid[ k] = (_S_grid[k] > _K)? 0:(_K - _S_grid[k]);
  }

  int jmin, jmax;
  std::vector<double>::iterator ps, pslead, ps_end;
  double ds;

  for (--i; i>=0; --i) {
    jmin = get_min_j(i);
    jmax = get_max_j(i);
    
    ps = _S_grid.begin() + get_grid_offset( i, jmin);
    pslead = ps + 1;
    ps_end = _S_grid.begin() + get_grid_offset( i, jmax);

    *ps = S(i, jmin);
    ds = exp( _Scap);

    while (ps != ps_end) {
      *pslead ++ = ds * *ps ++;
    }
  }

  for (i = _NumberIterations-1; i>=0; --i) {
    tau = (_NumberIterations - i) * _dt;
    exp_alpha_r_tau = exp( (_alpha - _r)*tau);
    exp_r_tau = exp( - _r * tau);

    for (j=get_min_j(i); j<=get_max_j(i); j++) {
      k = get_grid_offset( i, j);
      l = get_grid_offset( i+1, j);
      m = get_grid_offset( i+1, j+1);

      //_S_grid[ k] = S( i, j);
      
      if (_calc_call) {
	_call_grid[ k] = .5*(_call_grid[ l]+_call_grid[ m]) * _exp_r_t;
	intrinsic_value = call_intrinsic_value( _S_grid[k], exp_alpha_r_tau, exp_r_tau);
	if (_call_grid[ k] < intrinsic_value) {
	  _call_grid[ k] = intrinsic_value;
	}
      }

      if (_calc_put) {
	_put_grid[ k] = .5*(_put_grid[ l]+_put_grid[ m]) * _exp_r_t;
	intrinsic_value = put_intrinsic_value( _S_grid[k], exp_alpha_r_tau, exp_r_tau);
	if (_put_grid[ k] < intrinsic_value) {
	  _put_grid[ k] = intrinsic_value;
	}
      }
    }
  }

  k = get_grid_offset( 0, 0);
  _C = _call_grid[k];
  _P = _put_grid[k];
}

double binomial_option::C_sigma( double sigma) {
  _calc_call = true;
  _calc_put = false;
  if (sigma != _sigma) put_sigma( sigma);
  _calc_put = true;
  return _C;
}

double binomial_option::P_sigma( double sigma) {
  _calc_call = false;
  _calc_put = true;
  if (sigma != _sigma) put_sigma( sigma);
  _calc_call = true;
  return _P;
}

double binomial_option::call_intrinsic_value() const { 
  double tmp = _S - _K;
  tmp = (tmp>0)? tmp : 0;

  double tmp2 = _S*exp((_alpha - _r)*_tau) - _K * exp( -_r * _tau);
  return (tmp2 > tmp) ?  tmp2 : tmp;
}


double binomial_option::put_intrinsic_value() const { 
  double tmp = _K - _S;
  tmp = (tmp>0)? tmp : 0;

  double tmp2 =  _K * exp( -_r * _tau) - _S*exp((_alpha - _r)*_tau);
  return (tmp2 > tmp) ?  tmp2 : tmp;
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

int binomial_option::strictly_check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma,
      int NumberIterations)
{
  #define Funcname  "binomial_option::check_attributes"

  _error_msg = "";
  _erno = 0;

  CHECK(S <=  0, __LINE__, Funcname);
  CHECK(K <=  0, __LINE__, Funcname);
  CHECK(Tau <=  0, __LINE__, Funcname);
  CHECK(R <  0, __LINE__, Funcname);
  //CHECK(Sigma<=0, __LINE__, Funcname);
  //CHECK(is_NA( Sigma), __LINE__, Funcname);
  CHECK(NumberIterations<=0, __LINE__, Funcname);

  return 0;
}

double binomial_option::call_implied_sigma( double call_price, bool show_iterations, double eps, bool nothrow) 
{
  int erno;

  erno = strictly_check_attributes(
      _S,
      _K,
      _tau,
      _alpha,
      _r,
      _sigma,
      _NumberIterations);

  if (erno) {
    if (_sigma == 0) {
      _C = call_intrinsic_value();
      return NA;
    }

    _C_implied_sigma = NA;
    if (nothrow) {
      return NA;
    }
    throw std::domain_error( _error_msg);
  }

  if (call_price < call_intrinsic_value() + eps) {
    _C_implied_sigma = NA;
    if (nothrow) {
      return NA;
    }
    throw std::domain_error( "call_price < call_intrinsic_value in binomial_option::call_implied_volatility");
  }

  double sigma0, sigma1, price_sigma0, price_sigma1;

  american_option_fudge aof( _S, _K, _tau, _alpha, _r, _sigma); 
  american_option_approximation aop( _S, _K, _tau, _alpha, _r, _sigma); 

  sigma0 = aof.call_implied_sigma( call_price);

  sigma1 = aop.call_implied_sigma( call_price);

  if (sigma1 == sigma0) sigma1 *= 1.1;

  option_pair *op = dynamic_cast< option_pair * > ( this);

  stradle_value(
		sigma0,
		sigma1,
		price_sigma0,
		price_sigma1,
		call_price,
		*op, 
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
	   *op, 
	   &option_pair::C_sigma);

  solution.do_iteration( show_iterations);

  if (!solution.get_converged()) {
    _C_implied_sigma = NA;
    if (nothrow) {
      return NA;
    }
    throw std::domain_error( "sigma did not converge in option_pair::call_implied_sigma");
  }
  
  return solution.get_x_mid();
}

double binomial_option::put_implied_sigma( double put_price, bool show_iterations, double eps, bool nothrow) 
{
  int erno;

  erno = strictly_check_attributes(
      _S,
      _K,
      _tau,
      _alpha,
      _r,
      _sigma,
      _NumberIterations);

  if (erno) {
    if (_sigma == 0) {
      _P = put_intrinsic_value();
      return NA;
    }

    _P_implied_sigma = NA;
    if (nothrow) {
      return NA;
    }
    throw std::domain_error( _error_msg);
  }

  if (put_price < put_intrinsic_value()+eps) {
    _P_implied_sigma = NA;
    if (nothrow) {
      return NA;
    }
    throw std::domain_error( "put_price < put_intrinsic_value in binomial_option::put_implied_volatility");
  }

  double sigma0, sigma1, price_sigma0, price_sigma1;

  american_option_fudge aof( _S, _K, _tau, _alpha, _r, _sigma); 
  american_option_approximation aop( _S, _K, _tau, _alpha, _r, _sigma); 

  sigma0 = aof.put_implied_sigma( put_price);

  sigma1 = aop.put_implied_sigma( put_price);

  if (sigma1 == sigma0) sigma1 *= 1.1;

  option_pair *op = dynamic_cast< option_pair * > ( this);

  stradle_value(
		sigma0,
		sigma1,
		price_sigma0,
		price_sigma1,
		put_price,
		*op, 
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
	   *op, 
	   &option_pair::P_sigma);

  solution.do_iteration( show_iterations);

  if (!solution.get_converged()) {
    _P_implied_sigma = NA;
    if (nothrow) {
      return NA;
    }
    throw std::domain_error( "sigma did not converge in option_pair::put_implied_sigma");
  }
  
  return solution.get_x_mid();
}
