/// \file Bisection.cpp
/// \brief This is a demonstration of how to use Secant.h<br />
/// This test the algorithm in 2 situations:<br />
///    1) with no boudaries and a well defined solution <br/>
///    2) with boundaries, and a boundary solution

// to compile:  g++ -I../include -lm Bisection.cpp<br />
// to compile with debugging: g++ -g -I../include -lm Bisection.cpp<br />
#include "simpleoption/Bisection.h"
#include <math.h>

/// \brief This is a functor which encapsulates the function: exp(x) - offset.  The function variable is x. Offset is a hidden variable. This class is used to demonstrate the Bisection0< functor, real > template class, in the test file Secant.cpp
class secant_functor {
public:
  /// the hidden variable for class secant_functor, which makes this class a functor, as opposed to a group of function pointers
  double _offset;
  
  /// the creator for secant_functor, which assigns the hidden variable offset.
  secant_functor( double offset) : _offset(offset) { }

  /// the function to be solved
  double f( double x) {
    return - (exp( x) - _offset);
  }
  
};

int main() {
  double x0=1.0, x1=10.0, fx0, fx1;
  secant_functor sf( 5);

  stradle_value( x0, x1, fx0, fx1, 0.0, sf, &secant_functor::f);

  Bisection< secant_functor, double > 
    foobar( 1.0, 10.0, fx0, fx1, 0.0, 1e-8, 1e-8, 100, sf, &secant_functor::f);

  foobar.do_iteration( true);

  cout << "***************************" << endl;

  cout << foobar.get_x_mid() << endl;

  return 0;
}
