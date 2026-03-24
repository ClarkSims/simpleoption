#include "simpleoption/option_tenor.h"

void option_tenor::init_calc_derived_attributes() const {
  if (_N_strikes == 0) return;

  _elements.resize( _N_strikes);

  size_t i;
  for (i=0; i<_N_strikes; ++i) {
    double K = _K_min + i*_K_inc;
    _elements.at(i)._K = K;
  }
  
  _K_max = _K_min + (_N_strikes - 1) * _K_inc; 
}

size_t option_tenor::calculate_strike_offset( double K) {
  if (K < _K_min) {
    std::ostringstream os;
    os << "K < _K_min in option_tenor::add_option_tenor_element, K = " << K
       << " _K_min = " << _K_min
       << " at " << __FILE__ << ":" << __LINE__;
    throw std::runtime_error( os.str());
  }
  if (K > _K_max) {
    std::ostringstream os;
    os << "K > _K_max in option_tenor::add_option_tenor_element, K = " << K
       << " _K_max = " << _K_max
       << " at " << __FILE__ << ":" << __LINE__;
    throw std::runtime_error( os.str());
  }
  size_t offset = (size_t)((K  - _K_min) / _K_inc + .5);
  double k_rem = K - _K_inc*offset - _K_min;
  if (fabs(k_rem) > 1e-3) {
    std::ostringstream os;
    os << "k_rem > 1e-3 in option_tenor::add_option_tenor_element, K = " << K
       << " _K_min = " << _K_min
       << " _K_inc = " << _K_inc
       << " k_rem = " << k_rem
       << " at " << __FILE__ << ":" << __LINE__;
    throw std::runtime_error( os.str());
  }
  return offset;
}

void option_tenor::add_option_tenor_element( 
                                            double K, 
                                            double call_price, 
                                            double put_price)
{
  size_t offset = calculate_strike_offset( K);

  _elements.at(offset).init( K, call_price, put_price);
}

void option_tenor::add_call( 
                            double K, 
                            double call_price)
{
  size_t offset = calculate_strike_offset( K);

  _elements.at(offset).put_call_price( call_price);
}

void option_tenor::add_put( 
                            double K, 
                            double put_price)
{
  size_t offset = calculate_strike_offset( K);

  _elements.at(offset).put_put_price( put_price);
}

void option_tenor::find_at_the_money() {
  size_t i;
  double straddle_price;

  _min_straddle_price = -1;

  for (i=0; i<_elements.size(); ++i) {
    option_tenor_element& ote = _elements[i];

    straddle_price = ote.get_straddle_price();

    if (straddle_price != -1) {
      if (_min_straddle_price == -1 || straddle_price<_min_straddle_price) {
        _min_straddle_price = straddle_price;
        _min_straddle_offset = i;
      }
    }
  }
  if (_min_straddle_price == -1) {
    throw std::runtime_error( "no valid straddles");
  }
}

void option_tenor::calculate_vol_curve() 
{
  find_at_the_money();
  option_tenor_element& ote = _elements[_min_straddle_offset];

  _op._NumberIterations = _N_binomial;
  _op._S = ote._call_price - ote._put_price + ote._K;
  _op._alpha = 0;
  _op._tau = _tau;
  _op._r = _r;
  _op._K = ote._K;
  _op._sigma = .2;

  _op.solve_forward_and_sigma( ote._call_price, ote._put_price, 0);

  _S = _op.get_S();

  size_t i;
  for (i=0; i<_elements.size(); ++i) {
    option_tenor_element& Ote = _elements[i];
    _op._K = _K_min + _K_inc * i;

    if ((_op._K+_K_inc)<=_S) {
      if (Ote._put_price > 0) {
        try {
          _op.put_implied_sigma( Ote._put_price);
          Ote._put_sigma = _op._sigma;
        } catch (...) {
        }
        try {
          _op.call_implied_sigma( Ote._call_price);
          Ote._call_sigma = _op._sigma;
        } catch (...) {
        }
      }
    } else {
      if (Ote._call_price > 0) {
        try {
          _op.call_implied_sigma( Ote._call_price);
          Ote._call_sigma = _op._sigma;
        } catch (...) {
        }
        try {
          _op.put_implied_sigma( Ote._put_price);
          Ote._put_sigma = _op._sigma;
        } catch (...) {
        }
      }
    }
  }
}

void option_tenor::print( std::ostream& os) {
  size_t i;
  os << "S = " << _S << std::endl;
  for (i=0; i<_elements.size(); ++i) {
    option_tenor_element& Ote = _elements[i];
    if ((Ote._K+_K_inc)<=_S) {
      if (Ote._put_sigma > 0) {
        os << Ote._K << " " << Ote._put_price << " " << Ote._put_sigma << std::endl;
      }
    } else {
      if (Ote._call_sigma > 0) {
        os << Ote._K << " " << Ote._call_price << " " << Ote._call_sigma << std::endl;
      }
    }
  }
}

void option_tenor::print2( std::ostream& os) {
  size_t i;
  os << "S = " << _S << std::endl;
  for (i=0; i<_elements.size(); ++i) {
    option_tenor_element& Ote = _elements[i];
    if (Ote._put_sigma > 0 || Ote._call_sigma > 0) {
      os << Ote._K << " ";
      if (Ote._put_sigma > 0) {
        os << Ote._put_price << " " << Ote._put_sigma << " ";
      } else {
        os << "N/A N/A "; 
      }
      if (Ote._call_sigma > 0) {
        os << Ote._call_price << " " << Ote._call_sigma << std::endl;
      } else {
        os << "N/A N/A" << std::endl; 
      }
    }
  }
}

void option_tenor::calculate_prices( double actual_sigma) {
  _op._NumberIterations = _N_binomial;
  _op._S = _S;
  _op._alpha = _alpha;
  _op._tau = _tau;
  _op._r = _r;

  for (size_t i=0; i<_elements.size(); ++i) {
    option_tenor_element& Ote = _elements[i];
    _op._K = _K_min + _K_inc * i;
    _op.put_sigma( actual_sigma);
    Ote._put_sigma = Ote._call_sigma = actual_sigma;
    Ote._put_price = _op._P;
    Ote._call_price = _op._C;
  }
}

void option_tenor::print_differences( option_tenor& rhs,  std::ostream& os) {
  os << "S = " << _S << std::endl;
  for (size_t i=0; i<_elements.size(); ++i) {
    option_tenor_element& ote_lhs = _elements[i];
    option_tenor_element& ote_rhs = rhs._elements[i];

    if ((ote_lhs._K+_K_inc)<=_S) {
      if (ote_lhs._put_sigma > 0) {
        os << "P " << ote_lhs._K << " " << ote_lhs._put_price << " " << ote_rhs._put_price << std::endl;
      }
    } else {
      if (ote_lhs._call_sigma > 0) {
        os << "C " << ote_lhs._K << " " << ote_lhs._call_price << " " << ote_rhs._call_price << std::endl;
      }
    }
  }
}

void option_tenor::print_differences( option_tenor& rhs,  std::ostream& os, bool print_call) {
  os << "S = " << _S << std::endl;
  for (size_t i=0; i<_elements.size(); ++i) {
    option_tenor_element& ote_lhs = _elements[i];
    option_tenor_element& ote_rhs = rhs._elements[i];

    if (print_call) {
      if (ote_lhs._call_sigma > 0) {
        os << "C " << ote_lhs._K << " " << ote_lhs._call_price << " " << ote_rhs._call_price << std::endl;
      }
    } else {
      if (ote_lhs._put_sigma > 0) {
        os << "P " << ote_lhs._K << " " << ote_lhs._put_price << " " << ote_rhs._put_price << std::endl;
      }
    }

  }
}
