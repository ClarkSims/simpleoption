/// \file
/// \brief This file contains the bisection algorithm and the bisection_secant algorithm
#ifndef Bisection_hpp
#define Bisection_hpp

#include <iostream>
#include <vector>

#include <stdexcept>

#include "Secant.h"

using namespace std;

/// \brief This function solves to find a pair of X values, such that <ol>
///  <li>_F(X_a)<0 for some X_a in the interval [X0 , X1]</li>
///  <li>_F(X_b)>0 for some X_b in the interval [X0 , X1]</li>
///  </ol>
///
/// <b>Assumes:</b><ol>
/// <li> _f is assumed to be monotonic. _f can be either monotonically increasing, or decreasing. </li>
/// <li> _f is defined on the range 0 - infinity. </li>
/// </ol>
/// <b>Changes:</b> X0, X1, FX0, FX1 <br />
/// <b>Throws: domain error if X0==X1  or range error if _f violates the monotonic condition<br />
/// <b>Description:</b> This template function solves for X0 and X1 so that f(X0) < Fsolve < f(X1)<br />
template< class functor, class real>
void stradle_value( 
		   real& X0,
		   real& X1,
		   real& FX0,
		   real& FX1,
		   real Fsolve,
		   functor& obj, 
		   real (functor::*f)(real),
		   bool initialize_fx0 = true,
		   bool initialize_fx1 = true,
		   int direction = 0,
		   real MinX = 0,
		   real MaxX = 0)
{
  bool is_increasing, check_bounds;

  if (initialize_fx0) FX0 =(obj.*f)( X0);
  if (initialize_fx1) FX1 =(obj.*f)( X1);

  check_bounds = MinX!=0 || MaxX!=0;

  if (X0==X1) throw domain_error( "X0==X1 in stradle function");

  if (X0 > X1) {
    real tmp;
    tmp = X0;    X0 = X1;  X1 = tmp;
    tmp = FX0; FX0 = FX1; FX1 = tmp;
  }

  if (FX0 < Fsolve && Fsolve < FX1) return;

  if (direction == 0) {
    is_increasing = FX1 > FX0;
  } else {
    is_increasing = direction > 0;
  }

  if (check_bounds && (X1 > MaxX || X0 < MinX)) {
    throw domain_error( "X1 > MaxX || X0 < MinX in stradle_value");
  }

  if (is_increasing) {    // solve fx0 < fsolve < fx1
    while (FX0 > Fsolve) {
      if (FX1 < FX0) throw range_error( "FX1 < FX0 for monotonic increasing function  at label A in stradle_value");
      FX1 = FX0;
      X0 *= .5;
      if (check_bounds && X0 < MinX) {
	throw domain_error( "X0 < MinX in stradle_value");
      }
      FX0 = (obj.*f)( X0);
    }
    while (Fsolve > FX1) {
      if (FX1 < FX0) throw range_error( "FX1 < FX0 for monotonic increasing function  at label B in stradle_value");
      FX0 = FX1;
      X1 *= 2;
      if (check_bounds && X1 > MaxX) {
	throw domain_error( "X1 > MaxX in stradle_value");
      }
      FX1 = (obj.*f)( X1);
    }
  } else {  // solve fx0 > fsolve > fx1
    while (FX0 < Fsolve) {
      if (FX1 > FX0) throw range_error( "FX1 > FX0 for monotonic decreasing function at label C in stradle_value");
      FX1 = FX0;
      X0 *= .5;
      if (check_bounds && X0 < MinX) {
	throw domain_error( "X0 < MinX in stradle_value");
      }
      FX0 = (obj.*f)( X0);
    }
    while (Fsolve < FX1) {
      if (FX1 > FX0) throw range_error( "FX1 > FX0 for monotonic decreasing function at label D in stradle_value");
      FX0 = FX1;
      X1 *= 2;
      if (check_bounds && X1 > MaxX) {
	throw domain_error( "X1 > MaxX in stradle_value");
      }
      FX1 = (obj.*f)( X1);
    }
  }
}

/// \brief: This is an encapsulation of the Bisection algorithm. 
///
/// <b>assumption:</b> _f is monotonic. _f maybe be either monotonically increasing or decreasing.  _f is well defined on the interval _x0 to _x1.
/// <b>description:</b> This is an encapsulation of the Bisection algorithm.
template<class functor, class real>
class Bisection {
 protected:

  /// starting value of xlower, it must satisfy 
  real _x0;

  /// starting value of xupper
  real _x1;

  /// final value of solution
  real _fsolve;

  /// change in x which will terminate the loop
  real _delta;

  /// change in y which will terminate the loop
  real _epsilon;

  /// the lower bound for x
  real _x_lower;

  /// the upper bound for x
  real _x_upper;

  /// value of _f at the lower bound
  real _F_x_lower;

  /// this is a temporary variable which determins if F_x_lower is calculated prior to the first iteration
  bool _calc_F_x_lower;

  /// value of _f at the upper bound
  real _F_x_upper;

  /// this is a temporary variable which determins if F_x_lower is calculated prior to the first iteration
  bool _calc_F_x_upper;

  /// the next value of x to be valued with _f
  real _x_mid;

  /// the change in x
  real _dx;

  /// the value of _f evaluated at x_mid
  real _F_x_mid;

  /// absolute difference between _f(x) and _fsolve
  real _abs_error;

  /// \brief is true if f is increasing
  bool _is_increasing;

  /// \brief The maximum number of iterations. 
  int _max_iter;

  /// \brief  The iteration variable. After do_iteration is called, this is the last iteration before the loop was terminated.
  int i;

  /// \brief True if the function finished in less than max_iterations
  bool _converged;

  /// \brief This is a reference to the object which contains the hidden data for f
  functor& _obj;

  /// \brief This points to the function which is being solved.
  real (functor::*_f)(real);

 public:

  Bisection( 
    real X0, 
    real X1,
    real Fsolve,
    real Delta,
    real Epsilon, 
    unsigned int max_iter, 
    functor& obj, 
    real (functor::*f)(real)) :
    _x0(X0), _x1(X1), _fsolve(Fsolve), _delta(Delta), _epsilon(Epsilon), _max_iter(max_iter), _obj(obj), _f(f), _calc_F_x_lower(false), _calc_F_x_upper(false)
  {

  }

  Bisection( 
    real X0, 
    real X1,
    real FX0,
    real FX1,
    real Fsolve,
    real Delta,
    real Epsilon, 
    unsigned int max_iter, 
    functor& obj, 
    real (functor::*f)(real)) :
    _x0(X0), _x1(X1), _fsolve(Fsolve), _delta(Delta), _epsilon(Epsilon), 
      _F_x_lower(FX0), _calc_F_x_lower(true), 
      _F_x_upper(FX1),  _calc_F_x_upper(true), 
      _max_iter(max_iter), _obj(obj), _f(f)
  {

  }

  
  /// This function does the calculations.
  void do_iteration(  bool print_results)
    {

      _converged = false;

      _x_lower = _x0;
      _x_upper = _x1;
      if (_calc_F_x_lower) _F_x_lower =  (_obj.*_f)( _x_lower); 
      if (_calc_F_x_upper) _F_x_upper =  (_obj.*_f)( _x_upper); 

      _is_increasing = (_F_x_upper > _F_x_lower);

      if (print_results) {
	cout << "x_lower = " << _x_lower << endl;
	cout << "x_upper = " << _x_upper << endl;

	cout << "F_x_lower = " << _F_x_lower << endl;
	cout << "F_x_upper = " << _F_x_upper << endl;

	if (_is_increasing) {
	  cout << "F is increasing" << endl;
	} else {
	  cout << "F is decreasing" << endl;
	}
      }

      _converged = false;
    
      for (i=0; i<_max_iter; i++) {
	_x_mid = .5 * (_x_lower + _x_upper);
	_dx = _x_upper - _x_mid;
	_F_x_mid = (_obj.*_f)( _x_mid);
	_abs_error = _F_x_mid - _fsolve;
	if (_abs_error < 0) _abs_error = -_abs_error;

	if (print_results) {
	  cerr << "i = " << i 
	       << " x_lower = " << _x_lower 
	       << " x_upper = " << _x_upper 
	       << " x_mid = " << _x_mid 
	       << " _F_x_mid = " << _F_x_mid << endl;

	/*cout << "x_lower = " << _x_lower << endl;
	  cout << "x_upper = " << _x_upper << endl;
	  cout << "F_x_lower = " << _F_x_lower << endl;
	  cout << "F_x_mid   = " << _F_x_mid << endl;
	  cout << "F_x_upper = " << _F_x_upper << endl; */

	}

	if (_abs_error < _epsilon || _dx < _delta) {
	  _converged = true;
	  break;
	}

	if (_F_x_mid > _fsolve) { 
	  if (_is_increasing) {
	    _x_upper = _x_mid;
	    _F_x_upper = _F_x_mid;
	  } else {
	    _x_lower = _x_mid;
	    _F_x_lower = _F_x_mid;
	  }
	}

	if (_F_x_mid < _fsolve) {
	  if (_is_increasing) {
	    _x_lower = _x_mid;
	    _F_x_lower = _F_x_mid;
	  } else {
	    _x_upper = _x_mid;
	    _F_x_upper = _F_x_mid;
	  }
	}
      }
    }
				 

  /// \brief This returns the final value of x
  real get_x_mid() const {
    return _x_mid;
  }

  /// \brief  This true if the final values of the series satisfy the convergence
  bool get_converged() const {
    return _converged;
  }

  real get_df_dx() const {
    return (_F_x_upper - _F_x_mid) / (_x_upper - _x_mid);
  }
};

/// this class is a child class of Bisection. The algorithm converges faster because it changes from the bisection to the secant algorithm /// on every other iteration. 
template<class functor, class real>
class Bisection_Secant : public Bisection< functor, real >  {
 public:

  Bisection_Secant( 
    real X0, 
    real X1,
    real Fsolve,
    real Delta,
    real Epsilon, 
    unsigned int max_iter, 
    functor& obj, 
    real (functor::*f)(real)) :
    Bisection< functor, real > ( X0, X1, Fsolve, Delta, Epsilon, max_iter, obj, f )
  {
  }

  Bisection_Secant( 
    real X0, 
    real X1,
    real FX0,
    real FX1,
    real Fsolve,
    real Delta,
    real Epsilon, 
    unsigned int max_iter, 
    functor& obj, 
    real (functor::*f)(real)) :
    Bisection< functor, real > ( X0, X1, FX0, FX1, Fsolve, Delta, Epsilon, max_iter, obj, f )
  {
  }

  /// This function does the calculations.
  void do_iteration( bool print_results) {
    unsigned int i;
    real df_dx;

    this->_converged = false;

    this->_x_lower = this->_x0;
    this->_x_upper = this->_x1;
    if (this->_calc_F_x_lower) this->_F_x_lower =  (this->_obj.*this->_f)( this->_x_lower); 
    if (this->_calc_F_x_upper) this->_F_x_upper =  (this->_obj.*this->_f)( this->_x_upper); 
    
    if (print_results) {
      cout << "x_lower = " << this->_x_lower << endl;
      cout << "x_upper = " << this->_x_upper << endl;

      cout << "F_x_lower = " << this->_F_x_lower << endl;
      cout << "F_x_upper = " << this->_F_x_upper << endl;
    }

    this->_is_increasing = this->_F_x_upper > this->_F_x_lower;
    
    for (i=0; i<this->_max_iter; i++) {
      if (i % 2 == 1) {
	df_dx = (this->_F_x_upper - this->_F_x_lower) / (this->_x_upper - this->_x_lower);
	this->_x_mid = this->_x_upper - (this->_F_x_upper - this->_fsolve) / df_dx;
	if (this->_x_mid < this->_x_lower || this->_x_mid > this->_x_upper) {
	  this->_x_mid = .5 * (this->_x_lower + this->_x_upper);
	}
      } else {
	this->_x_mid = .5 * (this->_x_lower + this->_x_upper);
      }

      this->_dx = this->_x_upper - this->_x_mid;
      this->_F_x_mid = (this->_obj.*this->_f)( this->_x_mid);
      this->_abs_error = this->_F_x_mid - this->_fsolve;
      if (this->_abs_error < 0) this->_abs_error = -this->_abs_error;

      if (print_results) {
	cerr << "i = " << i 
	     << " x_lower = " << this->_x_lower 
	     << " x_upper = " << this->_x_upper 
	     << " x_mid = " << this->_x_mid 
	     << " this->_F_x_mid = " << this->_F_x_mid << endl;
      }

      if (this->_abs_error < this->_epsilon || this->_dx < this->_delta) {
	this->_converged = true;
	break;
      }

      if (this->_F_x_mid > this->_fsolve) { 
	if (this->_is_increasing) {
	  this->_x_upper = this->_x_mid;
	  this->_F_x_upper = this->_F_x_mid;
	} else {
	  this->_x_lower = this->_x_mid;
	  this->_F_x_lower = this->_F_x_mid;
	}
      }

      if (this->_F_x_mid < this->_fsolve) {
	if (this->_is_increasing) {
	  this->_x_lower = this->_x_mid;
	  this->_F_x_lower = this->_F_x_mid;
	} else {
	  this->_x_upper = this->_x_mid;
	  this->_F_x_upper = this->_F_x_mid;
	}
      }
    }
  }
};

#endif
