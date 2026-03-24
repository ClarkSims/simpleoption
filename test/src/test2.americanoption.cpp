/// \file test2.americanoption.cpp
/// This program calculations the values of a European option pair, an American optin pair, and then does the implied volatility calculation for an 
/// an American call, which is close to parity. This test the extreme behavior of the function.
#include <iostream>
#include <iomanip>
#include "simpleoption/binomial_greeks_option.h"
#include "simpleoption/simpleoption.h"

using namespace std;

int main() {
  european_option_pair
    eop( 424.0,
	 320.0,
	 .39726,
	 0,
	 .0534,
	 .2);

  cerr << eop << endl;

  binomial_option bo( 
	 424,
	 320.0,
	 .39726,
	  0,
	 .0534,
	 .2,
	 250);

  cerr << "Call = " << setprecision(3) << bo.get_C() << endl;
  cerr << "Put  = " << setprecision(3) << bo.get_P() << endl;

  binomial_greeks_option bog( 
	 424,
	 320.0,
	 .39726,
	  0,
	 .0534,
	 .2,
	 250);

  cerr << "Call = " << setprecision(3) << bog.get_C() << " dC_dS = " << bog.get_dC_dS() << endl;
  cerr << "Put  = " << setprecision(3) << bog.get_P() << " dP_dS = " << bog.get_dP_dS() << endl;


  bog.call_implied_sigma( 104.25, true);
  
  return 0;
}
    
