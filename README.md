This library is for calculating option prices, Greeks and implied volatilities.

demo_binomial_greeks_option_calc_alpha.cpp demonstrates how to:
 1) calculate the prices of a call and put as well of the Greeks
 2) calculate the price of a forward on the underlying, as well as the implied volatility given the price of the put and call
 3) calculate the implied volatility of a call and put

demo_binomial_greeks_option.cpp demonstrates how to:
 1) calculate the prices of a call as well of the Greeks
 2) calculate the price of a forward on the underlying, as well as the implied volatility given the price of the put and call
 3) calculate the implied volatility of a call

demo_option_tenor demonstrates how to calculate the Greeks and implied volatilities for an option tenor

build.sh builds libsimpleoption.so and all executables

build_and_test.sh builds libsimpleoption.so and all executables and runs all tests

clean.sh removes all build directories

install.sh installs headers, libsimpleoption.so, and simpleoption.pc to /usr/local/include/simpleoption, /usr/local/lib, and /usr/local/lib/pkgconfig. Must be run as root or with sudo.
