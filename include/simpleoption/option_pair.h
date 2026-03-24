/// \file option_pair.h
#ifndef OPTIONPAIR_H
#define OPTIONPAIR_H
/***************************************************************************
 *   Copyright (C) 2026 by Clark Sims                                      *
 *   Clark.N.Sims@gmail.com                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#if defined( WIN32) && !defined(__NT__)
#define __NT__
#endif

#if defined( __NT__) && !defined(WIN32)
#define WIN32
#endif

#if defined(ALLSTATIC) && defined(OPTIONPAIRDLL)
#error ALLSTATIC and OPTIONPAIRDLL are mutually exclusive
#endif

#if defined(ALLSTATIC) || defined(OPTIONPAIRSTATIC)
#undef  OPTIONPAIRPTR
#define OPTIONPAIRPTR 
#define OPTIONPAIRFNC
#elif !defined(OPTIONPAIRPTR)
   #ifdef  __NT__
      #ifdef OPTIONPAIRDLL
         #define OPTIONPAIRPTR __declspec(dllexport)
         #define OPTIONPAIRFNC __declspec(dllexport)
      #else
         #define OPTIONPAIRPTR __declspec(dllimport)
         #define OPTIONPAIRFNC __declspec(dllimport)
      #endif
   #else
      #define OPTIONPAIRFNC
      #define OPTIONPAIRPTR
   #endif
#endif

#ifdef __GNUC__
  #ifdef  OPTIONPAIRFNC
    #undef OPTIONPAIRFNC
  #endif
  #define OPTIONPAIRFNC
#endif

#ifdef WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif

#ifdef WIN32
#pragma warning(disable : 4251) //4251=needs to have dll-interface
#endif

#include <math.h>
#include <string.h>
#include <stdexcept>
#include <sstream>

using namespace std;

/// \brief This is an abstract interface class, which contains algorithms 
///for calculating implied volatility, the derivatives with respect to sigma, and 
/// time decay. The interface also provides a standardized way of 
/// specifying options as a function of:<ul>
/// <li> S, underlying price </li>
/// <li> K, strike price </li>
/// <li> alpha, the return of the underlying </li>
/// <li> r, interest rate </li>
/// <li> tau, the amount of time till expiration </li>
/// <li> sigma, the volatility of the underlying </li>
/// </ul>
///Stocks, stock indexes, futures and cash options can be represented with 
///the same parameterization. Each underlying has the same forumula, but
/// different values of alpha:<ol type=A>
///    <li>For stocks and stock indexes, alpha = r - dividend yield.</li>
///    <li>For futures alpha = 0</li>
///    <li>For currencies alpha = the foreign interest rate</li>
///   </ol>
class OPTIONPAIRPTR option_pair {
  protected:
    //other data, functions and typedefs
    static int strictly_check_attributes(
          double S,
          double K,
          double Tau,
          double Alpha,
          double R,
          double Sigma);

    //key attributes
      double _S;
      double _K;
      double _tau;

    //functional attributes
      double _alpha;
      double _r;
      double _sigma;

    //derived attributes
      mutable double _C;
      mutable double _P;
      mutable double _dC_dS;
      mutable double _dP_dS;
      mutable double _d2C_dS2;
      mutable double _d2P_dS2;
      mutable double _dC_dtau;
      mutable double _dP_dtau;
      mutable double _dC_dsigma;
      mutable double _dP_dsigma;
      mutable bool _calc_call;
      mutable bool _calc_put;
      mutable double _input_call_price;
      mutable double _input_put_price;
      mutable double _C_implied_sigma;
      mutable double _P_implied_sigma;

    //static data for error handling
      static const char *_error_msg;
      static int _erno;

  public:
    //static functions for getting error messages
      static const char *get_error_msg() { return _error_msg;}
      static int get_erno() { return _erno;}
virtual const char *get_class_name() const { return "option_pair";}

    static int check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma);

    void local_set_null() {
      _S = 0;
      _K = 0;
      _tau = 0;
      _alpha = 0;
      _r = 0.05;
      _sigma = 0.2;
      _C = 0;
      _P = 0;
      _dC_dS = 0;
      _dP_dS = 0;
      _d2C_dS2 = 0;
      _d2P_dS2 = 0;
      _dC_dtau = 0;
      _dP_dtau = 0;
      _dC_dsigma = 0;
      _dP_dsigma = 0;
      _calc_call = true;
      _calc_put = true;
      _input_call_price = NA;
      _input_put_price = NA;
      _C_implied_sigma = NA;
      _P_implied_sigma = NA;
    }

    void set_null() {
      local_set_null();
    }

    virtual void init_calc_derived_attributes() const;

    void init( const option_pair& rhs) {
      _S = rhs._S;
      _K = rhs._K;
      _tau = rhs._tau;
      _alpha = rhs._alpha;
      _r = rhs._r;
      _sigma = rhs._sigma;
      _C = rhs._C;
      _P = rhs._P;
      _dC_dS = rhs._dC_dS;
      _dP_dS = rhs._dP_dS;
      _d2C_dS2 = rhs._d2C_dS2;
      _d2P_dS2 = rhs._d2P_dS2;
      _dC_dtau = rhs._dC_dtau;
      _dP_dtau = rhs._dP_dtau;
      _dC_dsigma = rhs._dC_dsigma;
      _dP_dsigma = rhs._dP_dsigma;
      _calc_call = rhs._calc_call;
      _calc_put = rhs._calc_put;
      _input_call_price = rhs._input_call_price;
      _input_put_price = rhs._input_put_price;
      _C_implied_sigma = rhs._C_implied_sigma;
      _P_implied_sigma = rhs._P_implied_sigma;
    }

    option_pair& operator = ( const option_pair& rhs) {
        init( rhs);
        return *this;
    }

    bool operator == ( const option_pair& rhs) const {
      if (_S != rhs._S) return false;
      if (_K != rhs._K) return false;
      if (_tau != rhs._tau) return false;
      if (_alpha != rhs._alpha) return false;
      if (_r != rhs._r) return false;
      if (_sigma != rhs._sigma) return false;
      if (_C != rhs._C) return false;
      if (_P != rhs._P) return false;
      if (_dC_dS != rhs._dC_dS) return false;
      if (_dP_dS != rhs._dP_dS) return false;
      if (_d2C_dS2 != rhs._d2C_dS2) return false;
      if (_d2P_dS2 != rhs._d2P_dS2) return false;
      if (_dC_dtau != rhs._dC_dtau) return false;
      if (_dP_dtau != rhs._dP_dtau) return false;
      if (_dC_dsigma != rhs._dC_dsigma) return false;
      if (_dP_dsigma != rhs._dP_dsigma) return false;
      if (_calc_call != rhs._calc_call) return false;
      if (_calc_put != rhs._calc_put) return false;
      if (_input_call_price != rhs._input_call_price) return false;
      if (_input_put_price != rhs._input_put_price) return false;
      if (_C_implied_sigma != rhs._C_implied_sigma) return false;
      if (_P_implied_sigma != rhs._P_implied_sigma) return false;
      return true;
    }

    bool operator != ( const option_pair& rhs) const {
      return !(*this == rhs);
    }

    option_pair( const option_pair& rhs) { 
        init( rhs);
    }

    void init(
      double S,
      double K,
      double Tau,
      double Alpha = 0,
      double R = 0.05,
      double Sigma = 0.2)
    {
      const char* funcname = "option_pair::init";

      check_attributes(
        S,
        K,
        Tau,
        Alpha,
        R,
        Sigma);

      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      //copy all_key attributes
      _S = S;
      _K = K;
      _tau = Tau;

      //copy all_functional attributes
      _alpha = Alpha;
      _r = R;
      _sigma = Sigma;
      init_calc_derived_attributes();
    }

    explicit option_pair(
      double S = 0,
      double K = 0,
      double Tau = 0,
      double Alpha = 0,
      double R = 0.05,
      double Sigma = 0.2)
    {
      set_null();

      init(
        S,
        K,
        Tau,
        Alpha,
        R,
        Sigma);
    }

    virtual ~option_pair() { }

    //data access functions

    double get_S() const { return _S;}
    double get_K() const { return _K;}
    double get_tau() const { return _tau;}
    double get_alpha() const { return _alpha;}
    double get_r() const { return _r;}
    double get_sigma() const { return _sigma;}
    double get_C() const { return _C;}
    double get_P() const { return _P;}
    double get_dC_dS() const { return _dC_dS;}
    double get_dP_dS() const { return _dP_dS;}
    double get_d2C_dS2() const { return _d2C_dS2;}
    double get_d2P_dS2() const { return _d2P_dS2;}
    double get_dC_dtau() const { return _dC_dtau;}
    double get_dP_dtau() const { return _dP_dtau;}
    double get_dC_dsigma() const { return _dC_dsigma;}
    double get_dP_dsigma() const { return _dP_dsigma;}
    bool get_calc_call() const { return _calc_call;}
    bool get_calc_put() const { return _calc_put;}
    double get_input_call_price() const { return _input_call_price;}
    double get_input_put_price() const { return _input_put_price;}
    double get_C_implied_sigma() const { return _C_implied_sigma;}
    double get_P_implied_sigma() const { return _P_implied_sigma;}

    //change data functions

    void put_S(double S) {
      const char* funcname = "put_S";

      check_attributes(
        S,
        _K,
        _tau,
        _alpha,
        _r,
        _sigma);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _S = S;

      init_calc_derived_attributes();
    }

    void put_K(double K) {
      const char* funcname = "put_K";

      check_attributes(
        _S,
        K,
        _tau,
        _alpha,
        _r,
        _sigma);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _K = K;

      init_calc_derived_attributes();
    }

    void put_tau(double Tau) {
      const char* funcname = "put_tau";

      check_attributes(
        _S,
        _K,
        Tau,
        _alpha,
        _r,
        _sigma);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _tau = Tau;

      init_calc_derived_attributes();
    }

    void put_alpha(double Alpha) {
      const char* funcname = "put_alpha";

      check_attributes(
        _S,
        _K,
        _tau,
        Alpha,
        _r,
        _sigma);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _alpha = Alpha;

      init_calc_derived_attributes();
    }

    void put_r(double R) {
      const char* funcname = "put_r";

      check_attributes(
        _S,
        _K,
        _tau,
        _alpha,
        R,
        _sigma);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _r = R;

      init_calc_derived_attributes();
    }

    void put_sigma(double Sigma) {
      const char* funcname = "put_sigma";

      check_attributes(
        _S,
        _K,
        _tau,
        _alpha,
        _r,
        Sigma);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _sigma = Sigma;

      init_calc_derived_attributes();
    }

    void init_simple_attributes( const option_pair& rhs) {
       _S = rhs._S;
       _K = rhs._K;
       _tau = rhs._tau;
       _alpha = rhs._alpha;
       _r = rhs._r;
       _sigma = rhs._sigma;
       _C = rhs._C;
       _P = rhs._P;
       _dC_dS = rhs._dC_dS;
       _dP_dS = rhs._dP_dS;
       _d2C_dS2 = rhs._d2C_dS2;
       _d2P_dS2 = rhs._d2P_dS2;
       _dC_dtau = rhs._dC_dtau;
       _dP_dtau = rhs._dP_dtau;
       _dC_dsigma = rhs._dC_dsigma;
       _dP_dsigma = rhs._dP_dsigma;
       _calc_call = rhs._calc_call;
       _calc_put = rhs._calc_put;
       _input_call_price = rhs._input_call_price;
       _input_put_price = rhs._input_put_price;
       _C_implied_sigma = rhs._C_implied_sigma;
       _P_implied_sigma = rhs._P_implied_sigma;
    }

    virtual double call_intrinsic_value() const = 0;
    virtual double put_intrinsic_value() const = 0;
    virtual double call_implied_sigma( double call_price, bool show_iterations=false);
    virtual double put_implied_sigma( double call_price, bool show_iterations=false);
    void solve_alpha_and_sigma(
            double call_price, 
            double put_price, 
            int ishow_iterations);
    void solve_forward_and_sigma(
            double call_price, 
            double put_price, 
            int ishow_iterations); 
    friend OPTIONPAIRFNC ostream& STDCALL operator << (ostream& os, const option_pair& rhs);
    /// \brief this function serves as a function pointer for calls to Bisection
    double C_sigma( double sigma);
    /// \brief this function serves as a function pointer for calls to Bisection
    double P_sigma( double sigma);
    void set_call_immediate_exercise() const;
    void set_put_immediate_exercise() const;
    static const double NA;
    static bool is_NA( const double& tst) { return memcmp( &tst, &NA, sizeof(double))==0;}
};


OPTIONPAIRFNC double STDCALL lookup_month_rate( int ccyymm);
OPTIONPAIRFNC void STDCALL deinit_month_rate();

#endif
