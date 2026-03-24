#include "simpleoption/option_tenor.h"

#define N 8

double call_price[N] = {
  2.10,
  1.20,
  0.60,
  0.35,
  0.25,
  0.20,
  0.15,
  0.10,
};

double put_price[N] = {
  0.01,
  0.08,
  0.48,
  1.30,
  2.20,
  3.10,
  4.00,
  4.83
};

int main() {
  double S = 0, R = .001, Alpha = 0, Tau=13.0/250, Kmin=12, Kinc=1.0;
  option_tenor ot( S, Alpha, R, Tau, Kmin, Kinc, N, 50);
  size_t i;
  for (i=0; i<N; ++i) {
    ot.add_option_tenor_element( 12.0+i, call_price[i], put_price[i]);
  }
  ot.calculate_vol_curve();
  ot.print( std::cout);
}
