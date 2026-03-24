/// \file test3.americanoption.cpp
/// This program creates an object of type binomaial_greeks_option, and then calls it repeatedly with different values of sigma.
/// It demonstrates howw price and delta change as sigma increases.
#include <iostream>
#include <iomanip>
#include "simpleoption/binomial_greeks_option.h"
#include "simpleoption/simpleoption.h"

using namespace std;

int main() {

  binomial_greeks_option bog( 
	 424,
	 320.0,
	 .39726,
	  0,
	 .0534,
	 .2,
	 250);

  double sigma;

  for (sigma=.1; sigma<=2.00; sigma+=.1) {
    bog.put_sigma( sigma);
    cerr << "sigma= " << sigma << " Call = " << setprecision(3) << bog.get_C() << " " << bog.get_dC_dS() << endl;
    //cerr << "Put  = " << setprecision(3) << bog.get_P() << " " << bog.get_dP_dS() << endl;
  }
  
  return 0;
}
    
