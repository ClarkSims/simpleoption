/// \file binomial.h
#ifndef BINOMIAL_H
#define BINOMIAL_H
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

#if defined(ALLSTATIC) && defined(BINOMIALDLL)
#error ALLSTATIC and BINOMIALDLL are mutually exclusive
#endif

#if defined(ALLSTATIC) || defined(BINOMIALSTATIC)
#undef  BINOMIALPTR
#define BINOMIALPTR 
#define BINOMIALFNC
#elif !defined(BINOMIALPTR)
   #ifdef  __NT__
      #ifdef BINOMIALDLL
         #define BINOMIALPTR __declspec(dllexport)
         #define BINOMIALFNC __declspec(dllexport)
      #else
         #define BINOMIALPTR __declspec(dllimport)
         #define BINOMIALFNC __declspec(dllimport)
      #endif
   #else
      #define BINOMIALFNC
      #define BINOMIALPTR
   #endif
#endif

#ifdef __GNUC__
  #ifdef  BINOMIALFNC
    #undef BINOMIALFNC
  #endif
  #define BINOMIALFNC
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

#include "option_pair.h"

using namespace std;

/// \brief This is the classic binomial option algorithm.
class BINOMIALPTR binomial_option: public option_pair {
  protected:
    //other data, functions and typedefs
    int strictly_check_attributes(
          double S,
          double K,
          double Tau,
          double Alpha,
          double R,
          double Sigma,
          int NumberIterations);
    virtual double S( int i, int j) const;
    virtual int get_grid_offset( int i, int j) const;
    virtual int get_max_j( int i) const;
    virtual int get_min_j( int i) const;
    virtual void allocate_price_vectors() const;
    virtual void solve_call_put() const;
    /// \brief This function calculates the intrinsic value of an American call. The value is the maximum of:<ol> <li> The value immedieate exericise</li> <li>The value of exercise at expiration</li> <li> 0 </li></ol> 
    inline double call_intrinsic_value( double S, double exp_alpha_r_t, double exp_r_t) const { 
      double amval = S - _K;
      if (amval < 0) amval = 0;
      double euval = S * exp_alpha_r_t - _K * exp_r_t;
      if (euval < 0) euval = 0;
      return (amval > euval)? amval : euval;
    }
    /// \brief This function calculates the intrinsic value of an American put. The value is the maximum of:<ol> <li> The value immedieate exericise</li> <li>The value of exercise at expiration</li> <li> 0 </li></ol> 
    inline double put_intrinsic_value( double S, double exp_alpha_r_t, double exp_r_t) const { 
      double amval = _K - S;
      if (amval < 0) amval = 0;
      double euval = _K * exp_r_t - S * exp_alpha_r_t;
      if (euval < 0) euval = 0;
      return (amval > euval)? amval : euval;
    }

    //key attributes

    //functional attributes
      int _NumberIterations;

    //derived attributes
      mutable double _Scap;
      mutable double _Ccap;
      mutable double _dt;
      mutable double _mu;
      mutable double _exp_r_t;
      mutable double _sqrt_tau;
      mutable double _sigma_sqrt_tau;
      mutable double _u;
      mutable double _v;
      mutable std::vector<double> _call_grid;
      mutable std::vector<double> _put_grid;
      mutable std::vector<double> _S_grid;

    //static data for error handling
      static const char *_error_msg;
      static int _erno;

  public:
    //static functions for getting error messages
      static const char *get_error_msg() { return _error_msg;}
      static int get_erno() { return _erno;}
virtual const char *get_class_name() const { return "binomial_option";}

    static int check_attributes(
      double S,
      double K,
      double Tau,
      double Alpha,
      double R,
      double Sigma,
      int NumberIterations);

    void local_set_null() {
      _NumberIterations = 100;
      _Scap = 0;
      _Ccap = 0;
      _dt = 0;
      _mu = 0;
      _exp_r_t = 0;
      _sqrt_tau = 0;
      _sigma_sqrt_tau = 0;
      _u = 0;
      _v = 0;
      _call_grid.clear();
      _put_grid.clear();
      _S_grid.clear();
    }

    void set_null() {
      option_pair::set_null();
      local_set_null();
    }

    virtual void init_calc_derived_attributes() const;

    void init( const binomial_option& rhs) {
      _S = rhs._S;
      _K = rhs._K;
      _tau = rhs._tau;
      _alpha = rhs._alpha;
      _r = rhs._r;
      _sigma = rhs._sigma;
      _NumberIterations = rhs._NumberIterations;
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
    }

    binomial_option& operator = ( const binomial_option& rhs) {
        init( rhs);
        return *this;
    }

    binomial_option( const binomial_option& rhs) { 
        init( rhs);
    }

    void init(
      double S,
      double K,
      double Tau,
      double Alpha = 0,
      double R = 0.05,
      double Sigma = 0.2,
      int NumberIterations = 100)
    {
      const char* funcname = "binomial_option::init";

      check_attributes(
        S,
        K,
        Tau,
        Alpha,
        R,
        Sigma,
        NumberIterations);

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
      init_calc_derived_attributes();
    }

    explicit binomial_option(
      double S = 0,
      double K = 0,
      double Tau = 0,
      double Alpha = 0,
      double R = 0.05,
      double Sigma = 0.2,
      int NumberIterations = 100)
    {
      set_null();

      init(
        S,
        K,
        Tau,
        Alpha,
        R,
        Sigma,
        NumberIterations);
    }

    virtual ~binomial_option() { }

    //data access functions

    int get_NumberIterations() const { return _NumberIterations;}
    double get_Scap() const { return _Scap;}
    double get_Ccap() const { return _Ccap;}
    double get_dt() const { return _dt;}
    double get_mu() const { return _mu;}
    double get_exp_r_t() const { return _exp_r_t;}
    double get_sqrt_tau() const { return _sqrt_tau;}
    double get_sigma_sqrt_tau() const { return _sigma_sqrt_tau;}
    double get_u() const { return _u;}
    double get_v() const { return _v;}
    const std::vector<double>& get_call_grid() const { return _call_grid;}
    const std::vector<double>& get_put_grid() const { return _put_grid;}
    const std::vector<double>& get_S_grid() const { return _S_grid;}

    //change data functions

    void put_NumberIterations(int NumberIterations) {
      const char* funcname = "put_NumberIterations";

      check_attributes(
        _S,
        _K,
        _tau,
        _alpha,
        _r,
        _sigma,
        NumberIterations);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _NumberIterations = NumberIterations;

      init_calc_derived_attributes();
    }

    void init_simple_attributes( const binomial_option& rhs) {
       _NumberIterations = rhs._NumberIterations;
       _Scap = rhs._Scap;
       _Ccap = rhs._Ccap;
       _dt = rhs._dt;
       _mu = rhs._mu;
       _exp_r_t = rhs._exp_r_t;
       _sqrt_tau = rhs._sqrt_tau;
       _sigma_sqrt_tau = rhs._sigma_sqrt_tau;
       _u = rhs._u;
       _v = rhs._v;
    }

    double C_tau( double tau);
    double P_tau( double tau);
    double C_sigma( double sigma);
    double P_sigma( double sigma);
    virtual double call_intrinsic_value() const;
    virtual double put_intrinsic_value() const;
    virtual double call_implied_sigma( double call_price, bool show_iterations=false, double eps=.005, bool nothrow=false);
    virtual double put_implied_sigma( double call_price, bool show_iterations=false, double eps=.005, bool nothrow=false);
};



#endif
