/// \file Secant.cpp
/// \brief This is a demonstration of how to use Secant.h<br />
/// description: this test the algorithm in 2 situations:<br />
///    1) with no boudaries and a well defined solution <br />
///    2) with boundaries, and a boundary solution

// to compile:  g++ -I../include -lm Secant.cpp<br />
#include "simpleoption/Secant.h"
#include <math.h>


/// \brief This is a functor which encapsulates the function: exp(x) - offset.  The function variable is x. Offset is a hidden variable. This class is used to demonstrate the SecantSolve0< functor, real > template class, in the test file Secant.cpp
class secant_functor {
public:
  /// the hidden variable for class secant_functor, which makes this class a functor, as opposed to a group of function pointers
  double _offset;
  
  /// the creator for secant_functor, which assigns the hidden variable offset.
  secant_functor( double offset) : _offset(offset) { }

  /// the function to be solved
  double f( double x) {
    return exp( x) - _offset;
  }
  
};

int main() {
  secant_functor sf( 5);

  SecantSolve0< secant_functor, double > foobar( 1.0, 10.0, 0.0, 1e-8, 100, sf, &secant_functor::f);

  foobar.do_iteration( true);

  foobar.set_check_boundary( true);
  foobar.set_max_x( 1.6);

  foobar.do_iteration( true);

  return 0;
}
