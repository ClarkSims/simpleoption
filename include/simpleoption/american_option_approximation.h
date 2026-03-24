/// \file american_option_approximation.h
#ifndef AMERICANAPPROX_H
#define AMERICANAPPROX_H
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

#if defined(ALLSTATIC) && defined(AMERICANAPPROXDLL)
#error ALLSTATIC and AMERICANAPPROXDLL are mutually exclusive
#endif

#if defined(ALLSTATIC) || defined(AMERICANAPPROXSTATIC)
#undef  AMERICANAPPROXPTR
#define AMERICANAPPROXPTR 
#define AMERICANAPPROXFNC
#elif !defined(AMERICANAPPROXPTR)
   #ifdef  __NT__
      #ifdef AMERICANAPPROXDLL
         #define AMERICANAPPROXPTR __declspec(dllexport)
         #define AMERICANAPPROXFNC __declspec(dllexport)
      #else
         #define AMERICANAPPROXPTR __declspec(dllimport)
         #define AMERICANAPPROXFNC __declspec(dllimport)
      #endif
   #else
      #define AMERICANAPPROXFNC
      #define AMERICANAPPROXPTR
   #endif
#endif

#ifdef __GNUC__
  #ifdef  AMERICANAPPROXFNC
    #undef AMERICANAPPROXFNC
  #endif
  #define AMERICANAPPROXFNC
#endif

#ifdef WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif

#ifdef WIN32
#pragma warning(disable : 4251) //4251=needs to have dll-interface
#endif

#include "simpleoption.h"

using namespace std;

typedef unsigned int uint;

/// \brief This class uses the Black Scholes formulae for European options, to approximate the values of American options. The value of the American option, is approximated as the maximum value of all European options, with Tau between 0, and the Tau of the American option. This formula sounded really good in the academic journals, but does not give accurate prices.
class AMERICANAPPROXPTR american_option_approximation: public european_option_pair {
  protected:
    //key attributes

    //functional attributes
      double _vega_epsilon;
      uint _max_iter;

    //derived attributes
      mutable european_option_pair _optvar;
      mutable bool _call_early_exercise;
      mutable bool _put_early_exercise;
      mutable double _call_tau;
      mutable double _put_tau;

    //static data for error handling
      static const char *_error_msg;
      static int _erno;

  public:
    //static functions for getting error messages
      static const char *get_error_msg() { return _error_msg;}
      static int get_erno() { return _erno;}
virtual const char *get_class_name() const { return "american_option_approximation";}

    static int check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma,
      double Vega_epsilon,
      uint Max_iter);

    void local_set_null() {
      _vega_epsilon = 1e-5;
      _max_iter = 100;
      _optvar.set_null();
      _call_early_exercise = 0;
      _put_early_exercise = 0;
      _call_tau = 0;
      _put_tau = 0;
    }

    void set_null() {
      european_option_pair::set_null();
      local_set_null();
    }

    virtual void init_calc_derived_attributes() const;

    void init( const american_option_approximation& rhs) {
      _S = rhs._S;
      _K = rhs._K;
      _tau = rhs._tau;
      _alpha = rhs._alpha;
      _r = rhs._r;
      _sigma = rhs._sigma;
      _vega_epsilon = rhs._vega_epsilon;
      _max_iter = rhs._max_iter;
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
      _mu = rhs._mu;
      _sqrt_tau = rhs._sqrt_tau;
      _sigma_sqrt_tau = rhs._sigma_sqrt_tau;
      _log_S_K = rhs._log_S_K;
      _Zk = rhs._Zk;
      _dZk_dtau = rhs._dZk_dtau;
      _h = rhs._h;
      _dh_dtau = rhs._dh_dtau;
      _N_h = rhs._N_h;
      _N_Zk = rhs._N_Zk;
      _exp_alpha_r_t = rhs._exp_alpha_r_t;
      _exp_r_t = rhs._exp_r_t;
      _S_exp_alpha_r_t = rhs._S_exp_alpha_r_t;
      _K_exp_r_t = rhs._K_exp_r_t;
      _d3C_dS2_dtau = rhs._d3C_dS2_dtau;
      _d3P_dS2_dtau = rhs._d3P_dS2_dtau;
      _dh_dsigma = rhs._dh_dsigma;
      _dZ_dsigma = rhs._dZ_dsigma;
      _d2C_dsigma2 = rhs._d2C_dsigma2;
      _d2P_dsigma2 = rhs._d2P_dsigma2;
      _dC_dtau = rhs._dC_dtau;
      _dP_dtau = rhs._dP_dtau;
      _d2C_dtau2 = rhs._d2C_dtau2;
      _d2P_dtau2 = rhs._d2P_dtau2;
      _d2C_dS_dtau = rhs._d2C_dS_dtau;
      _d2P_dS_dtau = rhs._d2P_dS_dtau;
      _P_h = rhs._P_h;
      _optvar = rhs._optvar;
      _call_early_exercise = rhs._call_early_exercise;
      _put_early_exercise = rhs._put_early_exercise;
      _call_tau = rhs._call_tau;
      _put_tau = rhs._put_tau;
    }

    american_option_approximation& operator = ( const american_option_approximation& rhs) {
        init( rhs);
        return *this;
    }

    american_option_approximation( const american_option_approximation& rhs) { 
        init( rhs);
    }

    void init(
      double S,
      double K,
      double Tau,
      double Alpha = 0,
      double R = 0.05,
      double Sigma = 0.2,
      double Vega_epsilon = 1e-5,
      uint Max_iter = 100)
    {
      const char* funcname = "american_option_approximation::init";

      check_attributes(
        S,
        K,
        Tau,
        Alpha,
        R,
        Sigma,
        Vega_epsilon,
        Max_iter);

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
      _vega_epsilon = Vega_epsilon;
      _max_iter = Max_iter;
      init_calc_derived_attributes();
    }

    explicit american_option_approximation(
      double S = 0,
      double K = 0,
      double Tau = 0,
      double Alpha = 0,
      double R = 0.05,
      double Sigma = 0.2,
      double Vega_epsilon = 1e-5,
      uint Max_iter = 100)
    {
      set_null();

      init(
        S,
        K,
        Tau,
        Alpha,
        R,
        Sigma,
        Vega_epsilon,
        Max_iter);
    }

    virtual ~american_option_approximation() { }

    //data access functions

    double get_vega_epsilon() const { return _vega_epsilon;}
    const uint& get_max_iter() const { return _max_iter;}
    const european_option_pair& get_optvar() const { return _optvar;}
    bool get_call_early_exercise() const { return _call_early_exercise;}
    bool get_put_early_exercise() const { return _put_early_exercise;}
    double get_call_tau() const { return _call_tau;}
    double get_put_tau() const { return _put_tau;}

    //change data functions

    void put_vega_epsilon(double Vega_epsilon) {
      const char* funcname = "put_vega_epsilon";

      check_attributes(
        _S,
        _K,
        _tau,
        _alpha,
        _r,
        _sigma,
        Vega_epsilon,
        _max_iter);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _vega_epsilon = Vega_epsilon;

      init_calc_derived_attributes();
    }

    void put_max_iter(uint Max_iter) {
      const char* funcname = "put_max_iter";

      check_attributes(
        _S,
        _K,
        _tau,
        _alpha,
        _r,
        _sigma,
        _vega_epsilon,
        Max_iter);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _max_iter = Max_iter;

      init_calc_derived_attributes();
    }

    void init_simple_attributes( const american_option_approximation& rhs) {
       _vega_epsilon = rhs._vega_epsilon;
       _call_early_exercise = rhs._call_early_exercise;
       _put_early_exercise = rhs._put_early_exercise;
       _call_tau = rhs._call_tau;
       _put_tau = rhs._put_tau;
    }

    virtual double call_intrinsic_value() const;
    virtual double put_intrinsic_value() const;
    void set_call( const european_option_pair& rhs) const;
    void set_put( const european_option_pair& rhs) const;
    friend AMERICANAPPROXFNC ostream& STDCALL operator << (ostream& os, const american_option_approximation& rhs);
};


#endif
