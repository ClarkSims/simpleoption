/// \file simpleoption.h
#ifndef SIMPOPT_H
#define SIMPOPT_H
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

#if defined(ALLSTATIC) && defined(SIMPOPTDLL)
#error ALLSTATIC and SIMPOPTDLL are mutually exclusive
#endif

#if defined(ALLSTATIC) || defined(SIMPOPTSTATIC)
#undef  SIMPOPTPTR
#define SIMPOPTPTR 
#define SIMPOPTFNC
#elif !defined(SIMPOPTPTR)
   #ifdef  __NT__
      #ifdef SIMPOPTDLL
         #define SIMPOPTPTR __declspec(dllexport)
         #define SIMPOPTFNC __declspec(dllexport)
      #else
         #define SIMPOPTPTR __declspec(dllimport)
         #define SIMPOPTFNC __declspec(dllimport)
      #endif
   #else
      #define SIMPOPTFNC
      #define SIMPOPTPTR
   #endif
#endif

#ifdef __GNUC__
  #ifdef  SIMPOPTFNC
    #undef SIMPOPTFNC
  #endif
  #define SIMPOPTFNC
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

#include "option_pair.h"

using namespace std;

/// \brief These are the Black Scholes option pricing formulae for puts and calls.
///
///This encapsulation allows for:<ul>
/// <li>Stocks, stock indexes, futures and cash options to be represented with the same formulae. Each underlying has the same forumula, but different values of alpha:<ol type=A>
///    <li>For stocks and stock indexes, alpha = r - dividend yield.</li>
///    <li>For futures alpha = 0</li>
///    <li>For currencies alpha = the foreign interest rate</li>
///   </ol>
///  </li>
/// <li>A large number of derivatives to be calculated</li>
/// <li>very fast calculation because the Cumulative Normal distribution, is calculated with the approximations from <a href="http://www.math.sfu.ca/~cbm/aands/page_932.htm">Abramowitz and Stegun</a></li>
/// <li>The formulae are described <a href="../../SimpleOptions/index.html">here</a></li>
/// </ul>

class SIMPOPTPTR european_option_pair: public option_pair {
  protected:
    //key attributes

    //functional attributes

    //derived attributes
      mutable double _mu;
      mutable double _sqrt_tau;
      mutable double _sigma_sqrt_tau;
      mutable double _log_S_K;
      mutable double _Zk;
      mutable double _dZk_dtau;
      mutable double _h;
      mutable double _dh_dtau;
      mutable double _N_h;
      mutable double _N_Zk;
      mutable double _exp_alpha_r_t;
      mutable double _exp_r_t;
      mutable double _S_exp_alpha_r_t;
      mutable double _K_exp_r_t;
      mutable double _d3C_dS2_dtau;
      mutable double _d3P_dS2_dtau;
      mutable double _dh_dsigma;
      mutable double _dZ_dsigma;
      mutable double _d2C_dsigma2;
      mutable double _d2P_dsigma2;
      mutable double _dC_dtau;
      mutable double _dP_dtau;
      mutable double _d2C_dtau2;
      mutable double _d2P_dtau2;
      mutable double _d2C_dS_dtau;
      mutable double _d2P_dS_dtau;
      mutable double _P_h;

    //static data for error handling
      static const char *_error_msg;
      static int _erno;

  public:
    //static functions for getting error messages
      static const char *get_error_msg() { return _error_msg;}
      static int get_erno() { return _erno;}
virtual const char *get_class_name() const { return "european_option_pair";}

    static int check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma);

    void local_set_null() {
      _mu = 0;
      _sqrt_tau = 0;
      _sigma_sqrt_tau = 0;
      _log_S_K = 0;
      _Zk = 0;
      _dZk_dtau = 0;
      _h = 0;
      _dh_dtau = 0;
      _N_h = 0;
      _N_Zk = 0;
      _exp_alpha_r_t = 0;
      _exp_r_t = 0;
      _S_exp_alpha_r_t = 0;
      _K_exp_r_t = 0;
      _d3C_dS2_dtau = 0;
      _d3P_dS2_dtau = 0;
      _dh_dsigma = 0;
      _dZ_dsigma = 0;
      _d2C_dsigma2 = 0;
      _d2P_dsigma2 = 0;
      _dC_dtau = 0;
      _dP_dtau = 0;
      _d2C_dtau2 = 0;
      _d2P_dtau2 = 0;
      _d2C_dS_dtau = 0;
      _d2P_dS_dtau = 0;
      _P_h = 0;
    }

    void set_null() {
      option_pair::set_null();
      local_set_null();
    }

    virtual void init_calc_derived_attributes() const;

    void init( const european_option_pair& rhs) {
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
    }

    european_option_pair& operator = ( const european_option_pair& rhs) {
        init( rhs);
        return *this;
    }

    european_option_pair( const european_option_pair& rhs) { 
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
      const char* funcname = "european_option_pair::init";

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

    explicit european_option_pair(
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

    virtual ~european_option_pair() { }

    //data access functions

    double get_mu() const { return _mu;}
    double get_sqrt_tau() const { return _sqrt_tau;}
    double get_sigma_sqrt_tau() const { return _sigma_sqrt_tau;}
    double get_log_S_K() const { return _log_S_K;}
    double get_Zk() const { return _Zk;}
    double get_dZk_dtau() const { return _dZk_dtau;}
    double get_h() const { return _h;}
    double get_dh_dtau() const { return _dh_dtau;}
    double get_N_h() const { return _N_h;}
    double get_N_Zk() const { return _N_Zk;}
    double get_exp_alpha_r_t() const { return _exp_alpha_r_t;}
    double get_exp_r_t() const { return _exp_r_t;}
    double get_S_exp_alpha_r_t() const { return _S_exp_alpha_r_t;}
    double get_K_exp_r_t() const { return _K_exp_r_t;}
    double get_d3C_dS2_dtau() const { return _d3C_dS2_dtau;}
    double get_d3P_dS2_dtau() const { return _d3P_dS2_dtau;}
    double get_dh_dsigma() const { return _dh_dsigma;}
    double get_dZ_dsigma() const { return _dZ_dsigma;}
    double get_d2C_dsigma2() const { return _d2C_dsigma2;}
    double get_d2P_dsigma2() const { return _d2P_dsigma2;}
    double get_dC_dtau() const { return _dC_dtau;}
    double get_dP_dtau() const { return _dP_dtau;}
    double get_d2C_dtau2() const { return _d2C_dtau2;}
    double get_d2P_dtau2() const { return _d2P_dtau2;}
    double get_d2C_dS_dtau() const { return _d2C_dS_dtau;}
    double get_d2P_dS_dtau() const { return _d2P_dS_dtau;}
    double get_P_h() const { return _P_h;}

    void init_simple_attributes( const european_option_pair& rhs) {
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
    }

    virtual double call_intrinsic_value() const { double tmp = _S_exp_alpha_r_t - _K_exp_r_t;  return (tmp>0)? tmp : 0;}
    virtual double put_intrinsic_value() const { double tmp = _K_exp_r_t - _S_exp_alpha_r_t;  return (tmp>0)? tmp : 0;}
    friend class american_option_approximation;
    friend SIMPOPTFNC ostream& STDCALL operator << (ostream& os, const european_option_pair& rhs);
    void init_call_values( const european_option_pair& rhs);
    void init_put_values( const european_option_pair& rhs);
    double dC_dt_tau( double tau);
    double d2C_dt2_tau( double tau);
    double dP_dt_tau( double tau);
    double d2P_dt2_tau( double tau);
    double C_tau( double tau);
    double P_tau( double tau);
    double dC_dsigma( double sigma);
    double d2C_dsigma2( double sigma);
    double dP_dsigma( double sigma);
    double d2P_dsigma2( double sigma);
    void set_call_immediate_exercise() const;
    void set_put_immediate_exercise() const;
};



#endif
