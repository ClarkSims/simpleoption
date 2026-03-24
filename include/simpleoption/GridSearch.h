/// \file GridSearch.h
#ifndef GridSearch_hpp
#define GridSearch_hpp

#include <iostream>
#include <vector>

using namespace std;

/// This is an encapsulation of the 1 dimensional grid search algorithm. It is used to find the maximum (or minimum) value of a function, on a grid.
template<class functor, class real>
class GridSearch {
 protected:
  // key attributes
  /// \brief the number of iterations of the routine
  int _max_number_increment;

  /// \brief the first value
  real _X0;
  /// \brief the second seed value
  real _dX;
  
  /// \brief this is the function which is being maximized / minimized
  real (functor::*_f)(real);

  /// \brief this is the object used to calculate the functions
  functor& _obj;

  // derived attributes

  /// \brief the value of x, st f(x) is maximum
  mutable real _x_max;
  /// \brief integer offset where f( _X + i*_dX)
  mutable int _i_max;
  /// \brief the maximum value of f on the grid
  mutable real _f_x_max;


  /// \brief the value of x, st f(x) is minimum
  mutable real _x_min;
  /// \brief integer offset where f( _X + i*_dX)
  mutable int _i_min;
  /// \brief the minimum value of f on the grid
  mutable real _f_x_min;

  /// \brief the values of x to be used in the search
  mutable vector<real> _x;

  /// \brief the sequence of f_x's
  mutable vector<real> _F_x;

 public:  

  /// returns true if maximum is on a boundary
  bool get_boundary_max() const {
    return (_i_max == 0 || _i_max == _max_number_increment);
  }

  /// returns true if miminum is on a boundary  
  bool get_boundary_min() const {
    return (_i_min == 0 || _i_min == _max_number_increment);
  }

  /// this returns a reference to the parent object with the hidden variables
  functor& get_functor() { return _obj;}

  /// This constructor sets everything needed by the algorithm.
  GridSearch( 
    int  Max_Number_Increment,
    real X0, 
    real dX,
    functor& obj, 
    real (functor::*f)(real))
    : _max_number_increment( Max_Number_Increment), _X0(X0), _dX(dX), _f(f), _obj(obj),
    _x(Max_Number_Increment+1), _F_x(Max_Number_Increment+1)
  {

  }

  /// This function does the calculations.
  void do_iteration( bool print_results) {
    int i;
    for (i=0; i<= _max_number_increment;i++) {
      _x[i] = _X0 + _dX * i;
      _F_x[i] = (_obj.*_f)( _x[i]);
      if (i==0) {
	_i_max = _i_min = 0;
	_x_max = _x_min = _X0;
	_f_x_max = _f_x_min = _F_x[0];
      } else {
	if (_F_x[i] > _f_x_max) {
	  _i_max = i;
	  _x_max = _x[i];
	  _f_x_max = _F_x[i];
	} else if (_F_x[i] < _f_x_min) {
	  _i_min = i;
	  _x_min = _x[i];
	  _f_x_min = _F_x[i];
	}
      }
      if (print_results) {
	cout << i << " " << _x[i] << " " << _F_x[i] << endl;
      }
    }
  }

  /// \brief the value of x, st f(x) is maximum
  real get_x_max() const { return _x_max;}
  /// \brief integer offset where f( _X + i*_dX)
  int get_i_max() const { return _i_max;}
  /// \brief the maximum value of f on the grid
  real get_f_x_max() const { return _f_x_max;}

  /// \brief the value of x, st f(x) is minimum
  real get_x_min() const { return _x_min;}
  /// \brief integer offset where f( _X + i*_dX)
  int get_i_min() const { return _i_min;}
  /// \brief the minimum value of f on the grid
  real get_f_x_min() const { return _f_x_min;}

};


#endif
