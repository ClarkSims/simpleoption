/// \file binomial_greeks_option.h
#ifndef BINOMIALGREEKS_H
#define BINOMIALGREEKS_H
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

#if defined(ALLSTATIC) && defined(BINOMIALGREEKSDLL)
#error ALLSTATIC and BINOMIALGREEKSDLL are mutually exclusive
#endif

#if defined(ALLSTATIC) || defined(BINOMIALGREEKSSTATIC)
#undef  BINOMIALGREEKSPTR
#define BINOMIALGREEKSPTR 
#define BINOMIALGREEKSFNC
#elif !defined(BINOMIALGREEKSPTR)
   #ifdef  __NT__
      #ifdef BINOMIALGREEKSDLL
         #define BINOMIALGREEKSPTR __declspec(dllexport)
         #define BINOMIALGREEKSFNC __declspec(dllexport)
      #else
         #define BINOMIALGREEKSPTR __declspec(dllimport)
         #define BINOMIALGREEKSFNC __declspec(dllimport)
      #endif
   #else
      #define BINOMIALGREEKSFNC
      #define BINOMIALGREEKSPTR
   #endif
#endif

#ifdef __GNUC__
  #ifdef  BINOMIALGREEKSFNC
    #undef BINOMIALGREEKSFNC
  #endif
  #define BINOMIALGREEKSFNC
#endif

#ifdef WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif

#ifdef WIN32
#pragma warning(disable : 4251) //4251=needs to have dll-interface
#endif

#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

#include "binomial.h"

/// \brief This class is for doing binomial option calculations, and calculating the dlta, gama and tau of the option
class BINOMIALGREEKSPTR binomial_greeks_option: public binomial_option {
  protected:
    //other data, functions and typedefs
    virtual int  get_grid_offset( int i, int j) const;
    virtual int  get_max_j( int i) const;
    virtual int  get_min_j( int i) const;
    virtual void allocate_price_vectors() const;
    virtual void solve_derivatives() const;

    //key attributes

    //functional attributes
      int _vega_method;
      int _skip_calculations;

    //derived attributes
      mutable double _exp_alpha_r_t;
      mutable double _sqrt_tau;
      mutable double _sigma_sqrt_tau;
      mutable double _P_h_call;
      mutable double _P_h_put;
      mutable double _del_sigma;
      mutable binomial_option _vega_temp;

    //static data for error handling
      static const char *_error_msg;
      static int _erno;

  public:
    //static functions for getting error messages
      static const char *get_error_msg() { return _error_msg;}
      static int get_erno() { return _erno;}
virtual const char *get_class_name() const { return "binomial_greeks_option";}

    static int check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma,
      int NumberIterations,
      int Vega_method,
      int Skip_calculations);

    void local_set_null() {
      _vega_method = 1;
      _skip_calculations = 0;
      _exp_alpha_r_t = 0;
      _sqrt_tau = 0;
      _sigma_sqrt_tau = 0;
      _P_h_call = 0;
      _P_h_put = 0;
      _del_sigma = 0;
      _vega_temp.set_null();
    }

    void set_null() {
      binomial_option::set_null();
      local_set_null();
    }

    virtual void init_calc_derived_attributes() const;

    void init( const binomial_greeks_option& rhs) {
      _S = rhs._S;
      _K = rhs._K;
      _tau = rhs._tau;
      _alpha = rhs._alpha;
      _r = rhs._r;
      _sigma = rhs._sigma;
      _NumberIterations = rhs._NumberIterations;
      _vega_method = rhs._vega_method;
      _skip_calculations = rhs._skip_calculations;
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
      _Scap = rhs._Scap;
      _Ccap = rhs._Ccap;
      _dt = rhs._dt;
      _mu = rhs._mu;
      _exp_r_t = rhs._exp_r_t;
      _sqrt_tau = rhs._sqrt_tau;
      _sigma_sqrt_tau = rhs._sigma_sqrt_tau;
      _u = rhs._u;
      _v = rhs._v;
      _call_grid = rhs._call_grid;
      _put_grid = rhs._put_grid;
      _S_grid = rhs._S_grid;
      _exp_alpha_r_t = rhs._exp_alpha_r_t;
      _sqrt_tau = rhs._sqrt_tau;
      _sigma_sqrt_tau = rhs._sigma_sqrt_tau;
      _P_h_call = rhs._P_h_call;
      _P_h_put = rhs._P_h_put;
      _del_sigma = rhs._del_sigma;
      _vega_temp = rhs._vega_temp;
    }

    binomial_greeks_option& operator = ( const binomial_greeks_option& rhs) {
        init( rhs);
        return *this;
    }

    binomial_greeks_option( const binomial_greeks_option& rhs) { 
        init( rhs);
    }

    void init(
      double S,
      double K,
      double Tau,
      double Alpha = 0,
      double R = 0.05,
      double Sigma = 0.2,
      int NumberIterations = 100,
      int Vega_method = 1,
      int Skip_calculations = 0)
    {
      const char* funcname = "binomial_greeks_option::init";

      check_attributes(
        S,
        K,
        Tau,
        Alpha,
        R,
        Sigma,
        NumberIterations,
        Vega_method,
        Skip_calculations);

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
      _NumberIterations = NumberIterations;
      _vega_method = Vega_method;
      _skip_calculations = Skip_calculations;
      init_calc_derived_attributes();
    }

    explicit binomial_greeks_option(
      double S = 0,
      double K = 0,
      double Tau = 0,
      double Alpha = 0,
      double R = 0.05,
      double Sigma = 0.2,
      int NumberIterations = 100,
      int Vega_method = 1,
      int Skip_calculations = 0)
    {
      set_null();

      init(
        S,
        K,
        Tau,
        Alpha,
        R,
        Sigma,
        NumberIterations,
        Vega_method,
        Skip_calculations);
    }

    virtual ~binomial_greeks_option() { }

    //data access functions

    int get_vega_method() const { return _vega_method;}
    int get_skip_calculations() const { return _skip_calculations;}
    double get_exp_alpha_r_t() const { return _exp_alpha_r_t;}
    double get_sqrt_tau() const { return _sqrt_tau;}
    double get_sigma_sqrt_tau() const { return _sigma_sqrt_tau;}
    double get_P_h_call() const { return _P_h_call;}
    double get_P_h_put() const { return _P_h_put;}
    double get_del_sigma() const { return _del_sigma;}
    const binomial_option& get_vega_temp() const { return _vega_temp;}

    //change data functions

    void put_vega_method(int Vega_method) {
      const char* funcname = "put_vega_method";

      check_attributes(
        _S,
        _K,
        _tau,
        _alpha,
        _r,
        _sigma,
        _NumberIterations,
        Vega_method,
        _skip_calculations);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _vega_method = Vega_method;

      init_calc_derived_attributes();
    }

    void put_skip_calculations(int Skip_calculations) {
      const char* funcname = "put_skip_calculations";

      check_attributes(
        _S,
        _K,
        _tau,
        _alpha,
        _r,
        _sigma,
        _NumberIterations,
        _vega_method,
        Skip_calculations);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _skip_calculations = Skip_calculations;

      init_calc_derived_attributes();
    }

    void init_simple_attributes( const binomial_greeks_option& rhs) {
       _vega_method = rhs._vega_method;
       _skip_calculations = rhs._skip_calculations;
       _exp_alpha_r_t = rhs._exp_alpha_r_t;
       _sqrt_tau = rhs._sqrt_tau;
       _sigma_sqrt_tau = rhs._sigma_sqrt_tau;
       _P_h_call = rhs._P_h_call;
       _P_h_put = rhs._P_h_put;
       _del_sigma = rhs._del_sigma;
    }

        void alt_put_skip_calculations(int Skip_calculations) {
           _skip_calculations = Skip_calculations;
        }
    friend class option_tenor;

    /**
     * @brief Approximates the vega (dX/d&sigma;) for both call and put using
     *        the second derivative of the binomial tree.
     *
     * Uses the relationship between the second-order price sensitivity and
     * volatility to compute an analytic approximation of d_C_d_sigma and
     * d_P_d_sigma without requiring a second full binomial valuation.
     * Results are stored in @c _dC_dsigma and @c _dP_dsigma.
     *
     * @note This is faster than numerical_approximate_dX_dsigma() because it
     *       avoids rebuilding the binomial tree, but may be less accurate for
     *       deep in-the-money or deep out-of-the-money options.
     *
     * @see numerical_approximate_dX_dsigma()
     */
    void fast_approximate_dX_dsigma() const;

    /**
     * @brief Approximates the vega (dX/d&sigma;) numerically by re-pricing
     *        the option at a perturbed volatility.
     *
     * Performs a second full binomial valuation at volatility
     * (@c _sigma + @p dsigma) and computes the finite-difference approximation:
     * @f[
     *   \frac{dX}{d\sigma} \approx \frac{X(\sigma + \Delta\sigma) - X(\sigma)}{\Delta\sigma}
     * @f]
     * Results are stored in @c _dC_dsigma and @c _dP_dsigma.
     *
     * @param dsigma  The volatility perturbation used for the finite difference.
     *                If @p dsigma is 0 (the default), it is set automatically to
     *                @c _sigma / @c _NumberIterations.
     *
     * @note More accurate than fast_approximate_dX_dsigma() at the cost of
     *       an additional binomial tree evaluation.
     *
     * @see fast_approximate_dX_dsigma()
     */
    void numerical_approximate_dX_dsigma( double dsigma=0) const;
};


extern "C" {

  BINOMIALGREEKSFNC double STDCALL AddArgs(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);


  BINOMIALGREEKSFNC double STDCALL C(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL dC_dS(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL d2C_dS2(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL dC_dtau(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL dC_dsigma(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);


  BINOMIALGREEKSFNC double STDCALL Call_implied_volatility(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Call_Price,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL P(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL dP_dS(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL d2P_dS2(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL dP_dtau(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL dP_dsigma(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL Put_implied_volatility(
				    double S,
				    double K,
				    double Tau,
				    double Alpha,
				    double R,
				    double Call_Price,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_C(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_dC_dS(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_d2C_dS2(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_dC_dtau(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_dC_dsigma(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);


  BINOMIALGREEKSFNC double STDCALL CBOT_Call_implied_volatility(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Call_Price,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_P(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_dP_dS(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_d2P_dS2(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_dP_dtau(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_dP_dsigma(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_Put_implied_volatility(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyy, char month_code,
				    double Alpha,
				    double R,
				    double Call_Price,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_C(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_dC_dS(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_d2C_dS2(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_dC_dtau(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_dC_dsigma(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_Call_implied_volatility(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Call_Price,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_P(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_dP_dS(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_d2P_dS2(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_dP_dtau(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_dP_dsigma(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Sigma,
				    int NumberIterations);

  BINOMIALGREEKSFNC double STDCALL CBOT_terse_Put_implied_volatility(
				    double S,
				    double K,
				    int ccyymmdd, int expire_ccyymm,
				    double Alpha,
				    double R,
				    double Put_Price,
				    int NumberIterations);

}

#endif
