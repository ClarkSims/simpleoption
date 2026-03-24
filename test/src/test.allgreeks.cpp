/// \file test.allgreeks.cpp
/// This program checks the valuations of the four different methods of calculating option prices. The macros ALPHA and R are for the 
/// parameters alpha and r. In the case where alpha = r = 0, all methods should give the same answer.

#include <iostream>
#include <iomanip>
#include "simpleoption/american_option_approximation.h"
#include "simpleoption/binomial.h"
#include "simpleoption/binomial_greeks_option.h"

using namespace std;

#define ALPHA 0.05
#define R 0.05
#define SIGMA .15

int main() {
  double K;

  american_option_approximation
    aoa( 100.0,
	 100.0,
	 1.0,
	 ALPHA,
	 R,
	 SIGMA);

  binomial_option bo( 
         100.0,
	 100.0,
	 1.0,
	 ALPHA,
	 R,
	 SIGMA,
	 100);

  binomial_greeks_option bgo( 
         100.0,
	 100.0,
	 1.0,
	 ALPHA,
	 R,
	 SIGMA,
	 100);

  cout << setiosflags (ios::showpoint | ios::fixed) << setprecision(2);

  cout << "############################## PRICES OF PUTS ###############################" << endl;
  for (K=70;K<140;K+=1) {
    aoa.put_K( K);
    bo.put_K( K);
    bgo.put_K( K);

    cout << K << " " << aoa.get_P() << " " <<   bo.get_P() << " " << bgo.get_P() << " * " << 
      aoa.get_dP_dS() << " "  << bgo.get_dP_dS() << endl;

  }

  cout << "############################## PRICES OF CALLS ###############################" << endl;
  for (K=70;K<140;K+=1) {
    aoa.put_K( K);
    bo.put_K( K);
    bgo.put_K( K);

    cout << K << " " << aoa.get_C() << " " <<   bo.get_C() << " " << bgo.get_C() << " * " << 
      aoa.get_dC_dS() << " "  << bgo.get_dC_dS() << endl;

  }
  
  return 0;
}
    
