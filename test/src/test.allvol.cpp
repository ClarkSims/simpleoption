/// \file  test.allvol.cpp
/// \brief This program checks the implied volatility calculation for these valuation models:<ul>
///  <li> european_option_pair </li>
///  <li> american_option_fudge </li>
///  <li> american_option_approximation </li>
/// <li> binomial_option </li>
/// </ul>
/// for the case where ALPHA = R = 0,  they should all give the same answer.
#include <iostream>
#include <iomanip>
#include "simpleoption/binomial.h"
#include "simpleoption/american_option_approximation.h"
#include "simpleoption/americanfudge.h"
#include "simpleoption/simpleoption.h"

using namespace std;
/*
#define ALPHA 0
#define R     0.05
#define S     424     
#define K     320
#define Tau   (145.0/365)
#define CallPrice 104.25

#define ALPHA 0
#define R     0.05
#define S     424    
#define K     470
#define Tau   (145.0/365)
#define CallPrice 21

#define ALPHA  0
#define R     0.05
#define S     424    
#define K     480
#define Tau   (145.0/365)
#define CallPrice 18.5
*/

#define ALPHA     0
#define R         0.05
#define S         424    
#define K         340
#define Tau       (145.0/365)
#define CallPrice 85.75

int main() {

  european_option_pair
    eop( S,
	 K,
	 Tau,
	 ALPHA,
	 R,
	 .2);

  cerr << "european call implied volatility = " << eop.call_implied_sigma( CallPrice) << endl;

  american_option_fudge
    aof( S,
	 K,
	 Tau,
	 ALPHA,
	 R,
	 .20);

  cerr << "american_option_fudge call implied volatility = " << aof.call_implied_sigma( CallPrice) << endl;

  american_option_approximation
    aoa( S,
	 K,
	 Tau,
	 ALPHA,
	 R,
	 .20);

  cerr << "american_option_approximation call implied volatility = " << aoa.call_implied_sigma( CallPrice) << endl;

  
  binomial_option bo( 
         S,
	 K,
	 Tau,
	 ALPHA,
	 R,
	 .2,
	 20);

  cerr << "binomial_option call implied volatility = " << bo.call_implied_sigma( CallPrice) << endl;
  

  return 0;
}
    
