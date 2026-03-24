/// \file test.american_option_approximation.cpp
#include <iostream>
#include "simpleoption/american_option_approximation.h"
#include "simpleoption/simpleoption.h"

using namespace std;

void print_derivative( const char *note, double dy_dx, double Y, double Y_dy, double dx) {
  double calc_dy_dx = (Y_dy - Y) / dx;
  cout << note << dy_dx << " " << calc_dy_dx << endl;
}

int main() {
  american_option_approximation
    aoa( 100.0,
	 102.0,
	 1.0,
	 .05,
	 .05,
	 .10,
	 1e-5,
	 100);

  cerr << aoa << endl;


  {
    american_option_approximation
      Daoa( 100.0,
	   100.0,
	   1.0,
	   .05,
	   .05,
	   .2 + .001);

    print_derivative( "dC_dsigma ", aoa.get_dC_dsigma(), aoa.get_C(), Daoa.get_C(), .001);
  }

  {
    american_option_approximation
      Daoa( 100.0+.01,
	   100.0,
	   1.0,
	   .05,
	   .05,
	   .2);

    print_derivative( "dC_dS ", aoa.get_dC_dS(), aoa.get_C(), Daoa.get_C(), .01);
  }

  {
    american_option_approximation
      Daoa( 100.0+.01,
	   100.0,
	   1.0,
	   .05,
	   .05,
	   .2);

    print_derivative( "d2C_dS2 ", aoa.get_d2C_dS2(), aoa.get_dC_dS(), Daoa.get_dC_dS(), .01);
  }

  {
    american_option_approximation
      Daoa( 100.0,
	   100.0,
	   1.01,
	   .05,
	   .05,
	   .2);

    print_derivative( "dC_dtau ", aoa.get_dC_dtau(), aoa.get_C(), Daoa.get_C(), .01);
  }

  {
    american_option_approximation
      Daoa( 100.0,
	   100.0,
	   1.0001,
	   .05,
	   .05,
	   .2);

    print_derivative( "dZk_dtau     : ", aoa.get_dZk_dtau(), aoa.get_Zk(), Daoa.get_Zk(), .0001);
    print_derivative( "dh_dtau      : ", aoa.get_dh_dtau(), aoa.get_h(), Daoa.get_h(), .0001);
    print_derivative( "d2C_dS_dtau  : ", aoa.get_d2C_dS_dtau(), aoa.get_dC_dS(), Daoa.get_dC_dS(), .0001);
    print_derivative( "d3C_dS2_dtau : ", aoa.get_d3C_dS2_dtau(), aoa.get_d2C_dS2(), Daoa.get_d2C_dS2(), .0001);
    print_derivative( "d2C_dtau2    : ", aoa.get_d2C_dtau2(), aoa.get_dC_dtau(), Daoa.get_dC_dtau(), .0001);
  }
  
  return 0;
}
    
