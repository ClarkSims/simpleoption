/// \file test1.americanoption.cpp
/// This program does a very simple calculation, calculating a European Option pair, and then a binomial option pair.
#include <iostream>
#include <iomanip>
#include "simpleoption/binomial.h"
#include "simpleoption/simpleoption.h"

using namespace std;

int main() {
  european_option_pair
    eop( 100.0,
	 100.0,
	 1.0,
	 .05,
	 .05,
	 .2);

  cerr << eop << endl;

  binomial_option bo( 100.0,
	 100.0,
	 1.0,
	 .05,
	 .05,
	 .2,
	 20);

  cerr << "Call = " << setprecision(3) << bo.get_C() << endl;
  cerr << "Put  = " << setprecision(3) << bo.get_P() << endl;
  
  return 0;
}
    
