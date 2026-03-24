/// \file option_tenor_element.h
#ifndef OPTION_TENOR_ELEMENT_H
#define OPTION_TENOR_ELEMENT_H
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

#if defined(ALLSTATIC) && defined(OPTION_TENOR_ELEMENTDLL)
#error ALLSTATIC and OPTION_TENOR_ELEMENTDLL are mutually exclusive
#endif

#if defined(ALLSTATIC) || defined(OPTION_TENOR_ELEMENTSTATIC)
#undef  OPTION_TENOR_ELEMENTPTR
#define OPTION_TENOR_ELEMENTPTR 
#define OPTION_TENOR_ELEMENTFNC
#elif !defined(OPTION_TENOR_ELEMENTPTR)
   #ifdef  __NT__
      #ifdef OPTION_TENOR_ELEMENTDLL
         #define OPTION_TENOR_ELEMENTPTR __declspec(dllexport)
         #define OPTION_TENOR_ELEMENTFNC __declspec(dllexport)
      #else
         #define OPTION_TENOR_ELEMENTPTR __declspec(dllimport)
         #define OPTION_TENOR_ELEMENTFNC __declspec(dllimport)
      #endif
   #else
      #define OPTION_TENOR_ELEMENTFNC
      #define OPTION_TENOR_ELEMENTPTR
   #endif
#endif

#ifdef __GNUC__
  #ifdef  OPTION_TENOR_ELEMENTFNC
    #undef OPTION_TENOR_ELEMENTFNC
  #endif
  #define OPTION_TENOR_ELEMENTFNC
#endif

#ifdef WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif

#ifdef WIN32
#pragma warning(disable : 4251) //4251=needs to have dll-interface
#endif

#include <sstream>
#include <stdexcept>

class OPTION_TENOR_ELEMENTPTR option_tenor_element {
  protected:
    //key attributes
      double _K;

    //functional attributes
      double _call_price;
      double _put_price;

    //derived attributes
      mutable double _straddle_price;
      mutable double _call_sigma;
      mutable double _put_sigma;
      mutable double _S;

    //static data for error handling
      static const char *_error_msg;
      static int _erno;

  public:
    //static functions for getting error messages
      static const char *get_error_msg() { return _error_msg;}
      static int get_erno() { return _erno;}
    static int check_attributes(
      double K,
      double Call_price,
      double Put_price);

    void local_set_null() {
      _K = 0;
      _call_price = -1;
      _put_price = -1;
      _straddle_price = -1;
      _call_sigma = -1;
      _put_sigma = -1;
      _S = -1;
    }

    void set_null() {
      local_set_null();
    }

    void init_calc_derived_attributes() const;

    void init( const option_tenor_element& rhs) {
      _K = rhs._K;
      _call_price = rhs._call_price;
      _put_price = rhs._put_price;
      _straddle_price = rhs._straddle_price;
      _call_sigma = rhs._call_sigma;
      _put_sigma = rhs._put_sigma;
      _S = rhs._S;
    }

    option_tenor_element& operator = ( const option_tenor_element& rhs) {
        init( rhs);
        return *this;
    }

    option_tenor_element( const option_tenor_element& rhs) { 
        init( rhs);
    }

    void init(
      double K,
      double Call_price = -1,
      double Put_price = -1)
    {
      const char* funcname = "option_tenor_element::init";

      check_attributes(
        K,
        Call_price,
        Put_price);

      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      //copy all_key attributes
      _K = K;

      //copy all_functional attributes
      _call_price = Call_price;
      _put_price = Put_price;
      init_calc_derived_attributes();
    }

    explicit option_tenor_element(
      double K = 0,
      double Call_price = -1,
      double Put_price = -1)
    {
      set_null();

      init(
        K,
        Call_price,
        Put_price);
    }

    ~option_tenor_element() { }

    //data access functions

    double get_K() const { return _K;}
    double get_call_price() const { return _call_price;}
    double get_put_price() const { return _put_price;}
    double get_straddle_price() const { return _straddle_price;}
    double get_call_sigma() const { return _call_sigma;}
    double get_put_sigma() const { return _put_sigma;}
    double get_S() const { return _S;}

    //change data functions

    void put_K(double K) {
      const char* funcname = "put_K";

      check_attributes(
        K,
        _call_price,
        _put_price);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _K = K;

      init_calc_derived_attributes();
    }

    void put_call_price(double Call_price) {
      const char* funcname = "put_call_price";

      check_attributes(
        _K,
        Call_price,
        _put_price);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _call_price = Call_price;

      init_calc_derived_attributes();
    }

    void put_put_price(double Put_price) {
      const char* funcname = "put_put_price";

      check_attributes(
        _K,
        _call_price,
        Put_price);
      if (_erno) {
        std::ostringstream sErr;
        sErr << __FILE__ << ":" << __LINE__ << " in " << funcname
          << _error_msg;
        throw std::runtime_error( sErr.str());
      }

      _put_price = Put_price;

      init_calc_derived_attributes();
    }

    void init_simple_attributes( const option_tenor_element& rhs) {
       _K = rhs._K;
       _call_price = rhs._call_price;
       _put_price = rhs._put_price;
       _straddle_price = rhs._straddle_price;
       _call_sigma = rhs._call_sigma;
       _put_sigma = rhs._put_sigma;
       _S = rhs._S;
    }

    friend class option_tenor;
};


#endif
