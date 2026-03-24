/// \file test4.americanoption.cpp
#include <iostream>
#include <iomanip>
#include "simpleoption/binomial_greeks_option.h"

using namespace std;

void print_derivative( const char *note, double dy_dx, double Y, double Y_dy, double dx) {
  double calc_dy_dx = (Y_dy - Y) / dx;

  cout << setiosflags (ios::showpoint | ios::fixed) << setprecision(4);

  cout << setw(10) << note << setw(10) << dy_dx << " " << setw(10) << calc_dy_dx << endl;
}

int main() {
  double S, R, K, Tau, Alpha, Sigma, foo;
  int NumberIterations;

  S = 424;
  K = 520;
  Tau = 145.0/365;
  Alpha = 0;
  R = .0534;
  Sigma = .3640;
  NumberIterations = 250;

  binomial_greeks_option bgo( S, K, Tau, Alpha, R, Sigma, NumberIterations);

  {
    double dS = 2 * Sigma * sqrt( Tau / NumberIterations);
  binomial_greeks_option Dbgo( S+dS, K, Tau, Alpha, R, Sigma, NumberIterations);
   print_derivative( "dC_dS ", bgo.get_dC_dS(), bgo.get_C(), Dbgo.get_C(), dS);
  }

  {
    double dS = 2 * Sigma * sqrt( Tau / NumberIterations);
  binomial_greeks_option Dbgo( S+dS, K, Tau, Alpha, R, Sigma, NumberIterations);
   print_derivative( "d2C_dS2 ", bgo.get_d2C_dS2(), bgo.get_dC_dS(), Dbgo.get_dC_dS(), dS);
  }

  {
    double dsigma = Sigma / NumberIterations;
  binomial_greeks_option Dbgo( S, K, Tau, Alpha, R, Sigma+dsigma, NumberIterations);
   print_derivative( "dC_dsigma ", bgo.get_dC_dsigma(), bgo.get_C(), Dbgo.get_C(),  dsigma);
  }

  {
    double dtau = Tau / NumberIterations;
  binomial_greeks_option Dbgo( S, K, Tau+dtau, Alpha, R, Sigma, NumberIterations);
   print_derivative( "dC_dtau ", bgo.get_dC_dtau(), bgo.get_C(), Dbgo.get_C(),  dtau);
  }

  
  return 0;
}
    
