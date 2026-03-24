/// \file option_tenor.h
#ifndef OPTION_TENOR_H
#define OPTION_TENOR_H
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

#if defined(ALLSTATIC) && defined(OPTION_TENORDLL)
#error ALLSTATIC and OPTION_TENORDLL are mutually exclusive
#endif

#if defined(ALLSTATIC) || defined(OPTION_TENORSTATIC)
#undef  OPTION_TENORPTR
#define OPTION_TENORPTR 
#define OPTION_TENORFNC
#elif !defined(OPTION_TENORPTR)
   #ifdef  __NT__
      #ifdef OPTION_TENORDLL
         #define OPTION_TENORPTR __declspec(dllexport)
         #define OPTION_TENORFNC __declspec(dllexport)
      #else
         #define OPTION_TENORPTR __declspec(dllimport)
         #define OPTION_TENORFNC __declspec(dllimport)
      #endif
   #else
      #define OPTION_TENORFNC
      #define OPTION_TENORPTR
   #endif
#endif

#ifdef __GNUC__
  #ifdef  OPTION_TENORFNC
    #undef OPTION_TENORFNC
  #endif
  #define OPTION_TENORFNC
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
#include "option_tenor_element.h"
#include "binomial_greeks_option.h"

class OPTION_TENORPTR option_tenor {
  protected:
    //key attributes
      double _S;
      double _alpha;
      double _r;
      double _tau;

    //functional attributes
      double _K_min;
      double _K_inc;
      size_t _N_strikes;
      size_t _N_binomial;

    //derived attributes
      mutable double _K_max;
      mutable std::vector<option_tenor_element> _elements;
      mutable binomial_greeks_option _op;
      mutable double _min_straddle_price;
      mutable size_t _min_straddle_offset;

    //static data for error handling
      static const char *_error_msg;
      static int _erno;

  public:
    //static functions for getting error messages
      static const char *get_error_msg() { return _error_msg;}
      static int get_erno() { return _erno;}
    static int check_attributes(
      double S,
      double Alpha,
      double R,
      double Tau,
      double K_min,
      double K_inc,
      size_t N_strikes,
      size_t N_binomial);

    void local_set_null() {
      _S = 0;
      _alpha = 0;
      _r = 0;
      _tau = 0;
      _K_min = 0;
      _K_inc = 0;
      _N_strikes = 0;
      _N_binomial = 50;
      _K_max = 0;
      _elements.clear();
      _op.set_null();
      _min_straddle_price = 0;
      _min_straddle_offset = 0;
    }

    void set_null() {
      local_set_null();
    }

    void init_calc_derived_attributes() const;

    void init( const option_tenor& rhs) {
      _S = rhs._S;
      _alpha = rhs._alpha;
      _r = rhs._r;
      _tau = rhs._tau;
      _K_min = rhs._K_min;
      _K_inc = rhs._K_inc;
      _N_strikes = rhs._N_strikes;
      _N_binomial = rhs._N_binomial;
      _K_max = rhs._K_max;
      _elements = rhs._elements;
      _op = rhs._op;
      _min_straddle_price = rhs._min_straddle_price;
      _min_straddle_offset = rhs._min_straddle_offset;
    }

    option_tenor& operator = ( const option_tenor& rhs) {
        init( rhs);
        return *this;
    }

    option_tenor( const option_tenor& rhs) { 
        init( rhs);
    }

    void init(
      double S,
      double Alpha,
      double R,
      double Tau,
      double K_min = 0,
      double K_inc = 0,
      size_t N_strikes = 0,
      size_t N_binomial = 50)
    {
      const char* funcname = "option_tenor::init";

      check_attributes(
        S,
        Alpha,
        R,
        Tau,
        K_min,
        K_inc,
        N_strikes,
        N_binomial);

      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      //copy all_key attributes
      _S = S;
      _alpha = Alpha;
      _r = R;
      _tau = Tau;

      //copy all_functional attributes
      _K_min = K_min;
      _K_inc = K_inc;
      _N_strikes = N_strikes;
      _N_binomial = N_binomial;
      init_calc_derived_attributes();
    }

    explicit option_tenor(
      double S = 0,
      double Alpha = 0,
      double R = 0,
      double Tau = 0,
      double K_min = 0,
      double K_inc = 0,
      size_t N_strikes = 0,
      size_t N_binomial = 50)
    {
      set_null();

      init(
        S,
        Alpha,
        R,
        Tau,
        K_min,
        K_inc,
        N_strikes,
        N_binomial);
    }

    ~option_tenor() { }

    //data access functions

    double get_S() const { return _S;}
    double get_alpha() const { return _alpha;}
    double get_r() const { return _r;}
    double get_tau() const { return _tau;}
    double get_K_min() const { return _K_min;}
    double get_K_inc() const { return _K_inc;}
    size_t get_N_strikes() const { return _N_strikes;}
    size_t get_N_binomial() const { return _N_binomial;}
    double get_K_max() const { return _K_max;}
    const std::vector<option_tenor_element>& get_elements() const { return _elements;}
    const binomial_greeks_option& get_op() const { return _op;}
    double get_min_straddle_price() const { return _min_straddle_price;}
    size_t get_min_straddle_offset() const { return _min_straddle_offset;}

    //change data functions

    void put_S(double S) {
      const char* funcname = "put_S";

      check_attributes(
        S,
        _alpha,
        _r,
        _tau,
        _K_min,
        _K_inc,
        _N_strikes,
        _N_binomial);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _S = S;

      init_calc_derived_attributes();
    }

    void put_alpha(double Alpha) {
      const char* funcname = "put_alpha";

      check_attributes(
        _S,
        Alpha,
        _r,
        _tau,
        _K_min,
        _K_inc,
        _N_strikes,
        _N_binomial);
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
        _alpha,
        R,
        _tau,
        _K_min,
        _K_inc,
        _N_strikes,
        _N_binomial);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _r = R;

      init_calc_derived_attributes();
    }

    void put_tau(double Tau) {
      const char* funcname = "put_tau";

      check_attributes(
        _S,
        _alpha,
        _r,
        Tau,
        _K_min,
        _K_inc,
        _N_strikes,
        _N_binomial);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _tau = Tau;

      init_calc_derived_attributes();
    }

    void put_K_min(double K_min) {
      const char* funcname = "put_K_min";

      check_attributes(
        _S,
        _alpha,
        _r,
        _tau,
        K_min,
        _K_inc,
        _N_strikes,
        _N_binomial);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _K_min = K_min;

      init_calc_derived_attributes();
    }

    void put_K_inc(double K_inc) {
      const char* funcname = "put_K_inc";

      check_attributes(
        _S,
        _alpha,
        _r,
        _tau,
        _K_min,
        K_inc,
        _N_strikes,
        _N_binomial);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _K_inc = K_inc;

      init_calc_derived_attributes();
    }

    void put_N_strikes(size_t N_strikes) {
      const char* funcname = "put_N_strikes";

      check_attributes(
        _S,
        _alpha,
        _r,
        _tau,
        _K_min,
        _K_inc,
        N_strikes,
        _N_binomial);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _N_strikes = N_strikes;

      init_calc_derived_attributes();
    }

    void put_N_binomial(size_t N_binomial) {
      const char* funcname = "put_N_binomial";

      check_attributes(
        _S,
        _alpha,
        _r,
        _tau,
        _K_min,
        _K_inc,
        _N_strikes,
        N_binomial);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _N_binomial = N_binomial;

      init_calc_derived_attributes();
    }

    void init_simple_attributes( const option_tenor& rhs) {
       _S = rhs._S;
       _alpha = rhs._alpha;
       _r = rhs._r;
       _tau = rhs._tau;
       _K_min = rhs._K_min;
       _K_inc = rhs._K_inc;
       _N_strikes = rhs._N_strikes;
       _N_binomial = rhs._N_binomial;
       _K_max = rhs._K_max;
       _min_straddle_price = rhs._min_straddle_price;
       _min_straddle_offset = rhs._min_straddle_offset;
    }

    size_t calculate_strike_offset( double K);
    void add_option_tenor_element( double K, double call_price, double put_price);
    void add_call( double K, double call_price);
    void add_put( double K, double put_price);
    void find_at_the_money();
    void calculate_vol_curve();
    void print( std::ostream& os);
    void print2( std::ostream& os);
    void calculate_prices( double actual_sigma);
    void print_differences( option_tenor& ot2, std::ostream& os);
    void print_differences( option_tenor& rhs,  std::ostream& os, bool print_call);
};


#endif
