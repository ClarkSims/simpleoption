/// \file GridSearch.cpp
/// \brief This is a demonstration of how to use Secant.h<br />
/// This test the algorithm in 2 situations:<br />
///    1) with no boudaries and a well defined solution<br/>
///    2) with boundaries, and a boundary solution

// to compile:  g++ -I../include -lm GridSearch.cpp<br />
#include "simpleoption/GridSearch.h"
#include <math.h>

/// \brief This is a functor which encapsulates the function: exp(x) - offset.  The function variable is x. Offset is a hidden variable.
class somefunc {
public:
  double a, b, c, d, e;
  
  somefunc( double A, double B, double C, double D, double E) : a(A), b(B), c(C), d(D), e(E) { }

  double f( double x) {
    return exp( a + b + c * x * x  +  d * x * x * x +  e * x * x * x * x);
  }

};


int main() {
  somefunc sf( -1, 2, 3, -.125, -.125);

  GridSearch< somefunc, double > foobar( 201, -10.0, .1, sf, &somefunc::f);

  foobar.do_iteration( true);

  cout << "x_max = " << foobar.get_x_max() << endl;
  cout << "f_x_max = " << foobar.get_f_x_max() << endl;

  return 0;
}
