/// \file
/// This file contains the secant algorithm, encoded in the SecantSolve0 template class.
#ifndef Secant_hpp
#define Secant_hpp

#include <iostream>
#include <vector>

using namespace std;

/// This is an encapsulation of the one dimensional Secant iteration algorithm. It can be used in situations with boundaries
template<class functor, class real>
class SecantSolve0 {
 protected:
  // key attributes
  /// \brief the maximum number of iterations of the routine
  int _max_iter;

  /// \brief the first seed value
  real _x0;
  /// \brief the second seed value
  real _x1;

  /// \brief the value of F to be solved. Usually this is 0. It is almost always 0, in the academic literature.
  real _Fsolve;
  
  /// \brief the convergence parameter. The calculations stop when F(x) < epsilon
  real _epsilon;

  /// \brief this is the function whose 0 is being solved
  real (functor::*_f)(real);

  /// \brief this is the object used to calculate the functions
  functor& _obj;

  // functional attributes
  /// \brief if true, then the search for the zero, has boundaries
  bool _check_boundary;
  /// \brief this is the minimum value of the acceptible range
  real _min_x;
  /// \brief this is the maximum value of the acceptible range
  real _max_x;


  // derived attributes
  /// \brief the value of x such that f(x) = 0 +/- epsilon
  mutable real _x_converge;
  /// \brief the last value of f
  mutable real _f_converge;
  /// \brief the sequence of x's 
  mutable vector<real> _x;
  /// \brief the sequence of f_x's
  mutable vector<real> _F_x;
  /// \brief the sequence of abs(f_x)'s
  mutable vector<real> _abs_Err_F_x;
  /// \brief the sequence of df_x's
  mutable vector<real> _dF_x;
  /// \brief a boolean which is true, if convergance occurs
  mutable bool _converged;
  /// \brief a boolean which is true if the sequence diverged
  mutable bool _diverged;
  /// \brief true if the final x is on the bounday
  mutable bool _boundary_solution;
  /// \brief the number of times a boundary has been hit
  mutable int _number_boundary_hits;
  /// \brief the number of times the lower boundary has been hit
  mutable bool _lower_boundary_hit;
  /// \brief the number of times the uper boundary has been hit
  mutable bool _upper_boundary_hit;
  /// \brief true if the last iteration hit the boundary
  mutable bool _boundary_hit;

  /// \brief this returns the number of times the bounaries have been hit. If boundaries are hit more than twice, then the loop is stoped.
  int check_boundary( real & x, real df_dx) {

    if (_check_boundary) {
      _lower_boundary_hit = _upper_boundary_hit = false;
      if (x < _min_x) {
	x = _min_x + 2 * _epsilon / df_dx;
	_lower_boundary_hit = true;
	_number_boundary_hits++;      
      } else if (x > _max_x) {
	x = _max_x - 2 * _epsilon / df_dx;
	_upper_boundary_hit = true;
	_number_boundary_hits++;
      }
      _boundary_hit = _lower_boundary_hit || _upper_boundary_hit;
    }
    
    return _number_boundary_hits;
  }

 public:  

  /// returns _boundary_solution
  bool get_boundary_solution() const {
    return _boundary_solution;
  }


  /// used to turn boundary checking on or off
  void set_check_boundary( bool check_boundary) {
    _check_boundary = check_boundary;
  }

  /// used to set the minimum of the solution set
  void set_min_x( real min_x) {
    _min_x = min_x;
  }

  /// used to set the maximum of the solution set
  void set_max_x( real max_x) {
    _max_x = max_x;
  }

  /// used to retrieve the convergance value
  real get_x_converge() const {
    return _x_converge;
  }

  /// used to retrieve the final value of f
  real get_f_converge() const {
    return _f_converge;
  }

  /// this returns a reference to the parent object with the hidden variables
  functor& get_functor() { return _obj;}

  /// returns if converged
  bool get_converged() const { return _converged;}

  /// This constructor sets everything needed by the algorithm.
  SecantSolve0( 
    real X0, 
    real X1,
    real Fsolve,
    real Epsilon, 
    unsigned int max_iter, 
    functor& obj, 
    real (functor::*f)(real), 
    bool check_boundary = false,
    real min_x = 0,
    real max_x = 0)
    :  _x0( X0), _x1( X1), _Fsolve( Fsolve), _epsilon( Epsilon),  _max_iter(max_iter), _obj(obj), _f(f),  _check_boundary(check_boundary), _min_x(min_x), _max_x(max_x), 
    _x_converge(0), _f_converge(0), 
    _x(max_iter), _F_x(max_iter), _abs_Err_F_x(max_iter), _dF_x(max_iter), _converged(false), _diverged(false), 
    _lower_boundary_hit(false), _upper_boundary_hit(false), _boundary_hit(false)
  {

  }

  /// This function does the calculations.
  void do_iteration( bool print_results) {
    int i;
    real dx, abs_dx, err;

    _converged = _diverged = _boundary_solution = false;

    _x[0] = _x0;
    _x[1] = _x1;
    _F_x[0] = (_obj.*_f)( _x[0]);
    _F_x[1] = (_obj.*_f)( _x[1]);
    _abs_Err_F_x[1] = (_F_x[1] > 0)? _F_x[1] : -_F_x[1];
    _dF_x[0] = 0;
    _dF_x[1] = (_F_x[1] - _F_x[0]) / (_x[1] - _x[0]);

    if (print_results) {
      cout << 0 << " " << _x[0] << " " << _F_x[0] << " not calculated" << endl;
      cout << 1 << " " << _x[1] << " " << _F_x[1] << " " << _dF_x[1] << endl;
    } 

    _number_boundary_hits=0;
    for (i=2; i<_max_iter; i++) {

      _x[i] = _x[i-1] - (_F_x[i-1] - _Fsolve) / _dF_x[i-1];

      if (check_boundary( _x[i], _dF_x[i-1])>1) {
	if (_lower_boundary_hit) {
	  _boundary_solution = true;
	  _x_converge = _min_x;
	  _f_converge = (_obj.*_f)( _min_x);
	  break;
	}
	if (_upper_boundary_hit) {
	  _boundary_solution = true;
	  _x_converge = _max_x;
	  _f_converge = (_obj.*_f)( _max_x);
	  break;
	}
      } else {
	if (_lower_boundary_hit) {
	  _x[i] = _min_x + _epsilon;
	}
	if (_upper_boundary_hit) {
	  _x[i] = _max_x - _epsilon;
	}
      }

      _F_x[i] = (_obj.*_f)( _x[i]);
      err = _F_x[i] - _Fsolve;
      _abs_Err_F_x[i] = (err > 0)? err : -err;
      dx = (_x[i] - _x[i-1]);
      abs_dx = (dx > 0)? dx : -dx;
	
      if (_abs_Err_F_x[i] < _epsilon || abs_dx < _epsilon) {
	_x_converge = _x[i];
	_f_converge = _F_x[i];
	_converged = true;
	break;
      }
      if (_abs_Err_F_x[i] > _abs_Err_F_x[i-1]) {
	_diverged = true;
      }

      _dF_x[i] = (_F_x[i] - _F_x[i-1]) / (_x[i] - _x[i-1]);

      if (print_results) {
	cout << i << " " << _x[i] << " " << _F_x[i] << " " << _dF_x[i] << endl;
      } 
    }

    if (_boundary_solution && print_results) {
      cout << i << " " << _x_converge << " " << _f_converge << " boundary_solution" << endl;
    } else if (_converged && print_results) {
      cout << i << " " << _x[i] << " " << _F_x[i] << " answer converged" << endl;
    } else if (_diverged && print_results) {
      cout << "answer diverged" << endl;
    }
  }
};


#endif
