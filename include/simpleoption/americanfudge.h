/// \file americanfudge.h
#ifndef AMERICANFUDGE_H
#define AMERICANFUDGE_H
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

#if defined(ALLSTATIC) && defined(AMERICANFUDGEDLL)
#error ALLSTATIC and AMERICANFUDGEDLL are mutually exclusive
#endif

#if defined(ALLSTATIC) || defined(AMERICANFUDGESTATIC)
#undef  AMERICANFUDGEPTR
#define AMERICANFUDGEPTR 
#define AMERICANFUDGEFNC
#elif !defined(AMERICANFUDGEPTR)
   #ifdef  __NT__
      #ifdef AMERICANFUDGEDLL
         #define AMERICANFUDGEPTR __declspec(dllexport)
         #define AMERICANFUDGEFNC __declspec(dllexport)
      #else
         #define AMERICANFUDGEPTR __declspec(dllimport)
         #define AMERICANFUDGEFNC __declspec(dllimport)
      #endif
   #else
      #define AMERICANFUDGEFNC
      #define AMERICANFUDGEPTR
   #endif
#endif

#ifdef __GNUC__
  #ifdef  AMERICANFUDGEFNC
    #undef AMERICANFUDGEFNC
  #endif
  #define AMERICANFUDGEFNC
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

/// \brief This class approximates the value of American Options as the value of the corresponding European option, plus the addition of a fudge factor, to make the exercise boundaries align correctly. It is much faster than the binomial model.
class AMERICANFUDGEPTR american_option_fudge: public european_option_pair {
  protected:
    //key attributes

    //functional attributes

    //derived attributes
      mutable double _call_fudge;
      mutable double _d_call_fudge_ds;
      mutable double _d_call_fudge_dt;
      mutable double _put_fudge;
      mutable double _d_put_fudge_ds;
      mutable double _d_put_fudge_dt;

    //static data for error handling
      static const char *_error_msg;
      static int _erno;

  public:
    //static functions for getting error messages
      static const char *get_error_msg() { return _error_msg;}
      static int get_erno() { return _erno;}
virtual const char *get_class_name() const { return "american_option_fudge";}

    static int check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma);

    void local_set_null() {
      _call_fudge = 0;
      _d_call_fudge_ds = 0;
      _d_call_fudge_dt = 0;
      _put_fudge = 0;
      _d_put_fudge_ds = 0;
      _d_put_fudge_dt = 0;
    }

    void set_null() {
      european_option_pair::set_null();
      local_set_null();
    }

    virtual void init_calc_derived_attributes() const;

    void init( const american_option_fudge& rhs) {
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
      _call_fudge = rhs._call_fudge;
      _d_call_fudge_ds = rhs._d_call_fudge_ds;
      _d_call_fudge_dt = rhs._d_call_fudge_dt;
      _put_fudge = rhs._put_fudge;
      _d_put_fudge_ds = rhs._d_put_fudge_ds;
      _d_put_fudge_dt = rhs._d_put_fudge_dt;
    }

    american_option_fudge& operator = ( const american_option_fudge& rhs) {
        init( rhs);
        return *this;
    }

    american_option_fudge( const american_option_fudge& rhs) { 
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
      const char* funcname = "american_option_fudge::init";

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

    explicit american_option_fudge(
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

    virtual ~american_option_fudge() { }

    //data access functions

    double get_call_fudge() const { return _call_fudge;}
    double get_d_call_fudge_ds() const { return _d_call_fudge_ds;}
    double get_d_call_fudge_dt() const { return _d_call_fudge_dt;}
    double get_put_fudge() const { return _put_fudge;}
    double get_d_put_fudge_ds() const { return _d_put_fudge_ds;}
    double get_d_put_fudge_dt() const { return _d_put_fudge_dt;}

    void init_simple_attributes( const american_option_fudge& rhs) {
       _call_fudge = rhs._call_fudge;
       _d_call_fudge_ds = rhs._d_call_fudge_ds;
       _d_call_fudge_dt = rhs._d_call_fudge_dt;
       _put_fudge = rhs._put_fudge;
       _d_put_fudge_ds = rhs._d_put_fudge_ds;
       _d_put_fudge_dt = rhs._d_put_fudge_dt;
    }

    virtual double call_intrinsic_value() const;
    virtual double put_intrinsic_value() const;
    friend AMERICANFUDGEFNC ostream& STDCALL operator << (ostream& os, const american_option_fudge& rhs);
};


#endif
