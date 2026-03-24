/// \file test1.simpleoption
/// This program calculates the derivatives of option prices both using a finite difference approximation, and the internal formulae. The two values
///  are printed out side by side, so they can be compared.
#include <iostream>
#include "simpleoption/simpleoption.h"

using namespace std;

void print_derivative( const char *note, double dy_dx, double Y, double Y_dy, double dx) {
  double calc_dy_dx = (Y_dy - Y) / dx;
  cout << note << dy_dx << " " << calc_dy_dx << endl;
}

int main() {
  european_option_pair
    eop( 100.0,
	 100.0,
	 1.0,
	 .05,
	 .05,
	 .2);

  cerr << eop << endl;

  {
    european_option_pair
      Deop( 100.0,
	   100.0,
	   1.0,
	   .05,
	   .05,
	   .2 + .001);

    print_derivative( "dC_dsigma ", eop.get_dC_dsigma(), eop.get_C(), Deop.get_C(), .001);
  }

  {
    european_option_pair
      Deop( 100.0+.01,
	   100.0,
	   1.0,
	   .05,
	   .05,
	   .2);

    print_derivative( "dC_dS ", eop.get_dC_dS(), eop.get_C(), Deop.get_C(), .01);
  }

  {
    european_option_pair
      Deop( 100.0+.01,
	   100.0,
	   1.0,
	   .05,
	   .05,
	   .2);

    print_derivative( "d2C_dS2 ", eop.get_d2C_dS2(), eop.get_dC_dS(), Deop.get_dC_dS(), .01);
  }

  {
    european_option_pair
      Deop( 100.0,
	   100.0,
	   1.01,
	   .05,
	   .05,
	   .2);

    print_derivative( "dC_dtau ", eop.get_dC_dtau(), eop.get_C(), Deop.get_C(), .01);
  }

  {
    european_option_pair
      Deop( 100.0,
	   100.0,
	   1.0001,
	   .05,
	   .05,
	   .2);

    print_derivative( "dZk_dtau     : ", eop.get_dZk_dtau(), eop.get_Zk(), Deop.get_Zk(), .0001);
    print_derivative( "dh_dtau      : ", eop.get_dh_dtau(), eop.get_h(), Deop.get_h(), .0001);
    print_derivative( "d2C_dS_dtau  : ", eop.get_d2C_dS_dtau(), eop.get_dC_dS(), Deop.get_dC_dS(), .0001);
    print_derivative( "d3C_dS2_dtau : ", eop.get_d3C_dS2_dtau(), eop.get_d2C_dS2(), Deop.get_d2C_dS2(), .0001);
    print_derivative( "d2C_dtau2    : ", eop.get_d2C_dtau2(), eop.get_dC_dtau(), Deop.get_dC_dtau(), .0001);
  }

  
  return 0;
}
    
