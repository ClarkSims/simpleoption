/// \file test.allopt.cpp
/// \brief This program checks the valuations of the four different methods of calculating option prices. The macros ALPHA and R are for the 
/// parameters alpha and r. In the case where alpha = r = 0, all methods should give the same answer.

#include <iostream>
#include <iomanip>
#include "simpleoption/binomial.h"
#include "simpleoption/american_option_approximation.h"
#include "simpleoption/americanfudge.h"
#include "simpleoption/simpleoption.h"

using namespace std;

#define ALPHA 0.05
#define R 0.05

int main() {
  double K;
  european_option_pair
    eop( 100.0,
	 100.0,
	 1.0,
	 ALPHA,
	 R,
	 .2);

  american_option_fudge
    aof( 100.0,
	 120.0,
	 1.0,
	 ALPHA,
	 R,
	 .20);

  american_option_approximation
    aoa( 100.0,
	 100.0,
	 1.0,
	 ALPHA,
	 R,
	 .20);

  binomial_option bo( 
         100.0,
	 100.0,
	 1.0,
	 ALPHA,
	 R,
	 .2,
	 100);

  cout << setiosflags (ios::showpoint | ios::fixed) << setprecision(2);

  cout << "############################## PRICES OF PUTS ###############################" << endl;
  for (K=90;K<120;K+=.25) {
    eop.put_K( K);
    aof.put_K( K);
    aoa.put_K( K);
    bo.put_K( K);

    cout << K << " " << eop.get_P() << " " << aof.get_P() << " " << aoa.get_P() << " " <<  bo.get_P() << endl;
  }

  cout << "############################## PRICES OF CALLS ###############################" << endl;
  for (K=90;K<120;K+=.25) {
    eop.put_K( K);
    aof.put_K( K);
    aoa.put_K( K);
    bo.put_K( K);

    cout << K << " " << eop.get_C() << " " << aof.get_C() << " " << aoa.get_C() << " " <<  bo.get_C() << endl;
  }
  
  return 0;
}
    
