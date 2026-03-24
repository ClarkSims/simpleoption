/// \file test4.simpleoption.cpp
#include <iostream>
#include <iomanip>
#include "simpleoption/simpleoption.h"

using namespace std;

int main() {
  double K, tau;

  K = 105;

  european_option_pair
    eop( 100.0,
	 K,
	 1.0,
	 .05,
	 .05,
	 .20);

  cout << setiosflags (ios::showpoint | ios::fixed) << setprecision(3);

  for (tau=.001;tau<=.05;tau+=.001) {
    eop.put_tau( tau);
    cout << tau << " " << eop.get_P() << " " << eop.get_dP_dS() << endl;
  }
  
  return 0;
}
    
