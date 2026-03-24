/// \file test3.simpleoption.cpp
#include <iostream>
#include <iomanip>
#include "simpleoption/american_option_approximation.h"
#include "simpleoption/simpleoption.h"

using namespace std;

int main() {
  double K;

  american_option_approximation
    aoa( 100.0,
	 100.0,
	 1.0,
	 .05,
	 .05,
	 .10,
	 1e-5,
	 100);

  for (K=90;K<120;K+=.25) {
    aoa.put_K( K);
    cout << setiosflags (ios::showpoint | ios::fixed) << setprecision(2);
    cout << K << " " << aoa.get_P() << " " << aoa.get_put_tau() << " " << aoa.get_dP_dS() << endl;
  }
  
  return 0;
}
    
