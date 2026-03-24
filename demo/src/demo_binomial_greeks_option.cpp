/// \file demo_binomial_greeks_option.cpp
/// g++ -I. demo_binomial_greeks_option.cpp binomial_greeks_option.cpp  binomial.cpp american_option_approximation.cpp simpleoption.cpp  option_pair.cpp rote_option_pair.cpp rote_american_option_approximation.cpp rote_americanfudge.cpp rote_simpleoption.cpp rote_binomial.cpp  americanfudge.cpp rote_binomial_greeks_option.cpp   normal.cpp
#include <iostream>
#include <iomanip>
#include "simpleoption/binomial_greeks_option.h"
#include "simpleoption/simpleoption.h"

using namespace std;

int main() {
  double S = 15.0; // underlying price
  double K = 15.0; // strike price
  double Tau = 17.0/250; // time till expiration in years
  double Alpha = 0; // return of underlying (0 for future)
  double R = .0; // interest rate
  double Sigma = .8; // volatility as whole number in annualized terms. ie  .8 = 80% annual volatility
  int NumberIterations = 1000;
  double CallPrice = 1.07;
  double PutPrice = .83;

  binomial_greeks_option bog( 
	 S,
	 K,
	 Tau,
	 Alpha,
	 R,
	 Sigma, // this will be overwritten
	 NumberIterations);

  bog.call_implied_sigma( CallPrice, false);
  cerr << "implied volatility from Call = " << bog.get_sigma() << endl;
  cerr << "dC_dS (delta)   = " << bog.get_dC_dS() << endl;
  cerr << "d2C_dS2 (gamma) = " << bog.get_d2C_dS2() << endl;
  cerr << "dC_dtau (tau)   = " << bog.get_dC_dtau() << endl;

  bog.fast_approximate_dX_dsigma();
  cerr << "fast approx dC_dsigma (vega)= " << bog.get_dC_dsigma() << endl;

  bog.numerical_approximate_dX_dsigma();
  cerr << "finite diff dC_dsigma (vega)= " << bog.get_dC_dsigma() << endl;

  bog.put_implied_sigma( PutPrice, false);
  cerr << "implied volatility from Put = " << bog.get_sigma() << endl;
  cerr << "dP_dS (delta)   = " << bog.get_dP_dS() << endl;
  cerr << "d2C_dS2 (gamma) = " << bog.get_d2P_dS2() << endl;
  cerr << "dP_dtau (tau)   = " << bog.get_dP_dtau() << endl;

  bog.fast_approximate_dX_dsigma();
  cerr << "fast approx dP_dsigma (vega)= " << bog.get_dP_dsigma() << endl;

  bog.numerical_approximate_dX_dsigma();
  cerr << "finite diff dP_dsigma (vega)= " << bog.get_dP_dsigma() << endl;
  
  return 0;
}
    
