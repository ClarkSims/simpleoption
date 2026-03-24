/// \file inverse_cumulative_normal_distribution.h
/// \brief This file has the code for a class which creates pseudorandom normally distributed
/// variables.
#ifndef inverse_cumulative_normal_distribution_h
#define inverse_cumulative_normal_distribution_h

#include <stdexcept>

/// \class inverse_cumulative_normal_distribution
/// \brief This class creates pseudorandom normally distrubuted variables. 
///
/// This class assumes that the range of the functor r, is much greater than
/// Number_Table_Rows_, or equivalently that range r % Number_Table_Rows == Number_Table_Rows-1.
/// Also, r must be uniformly distributed.
class inverse_cumulative_normal_distribution {
  static const int Number_Table_Rows_;
  static const int Number_Virtual_Table_Rows_; // = 2 * Number_Table_Rows_
  static const int Length_Function_Values_; // = 3 * Number_Table_Rows_
  static const double Function_Values_[];
  
 public:

  static inline double Table_Lookup( int i) {
    if (i<Number_Table_Rows_) {
      return -Function_Values_[3*(Number_Table_Rows_-i)];
    } else if (i<2*Number_Table_Rows_) {
      return Function_Values_[3*(i-Number_Table_Rows_)];
    } else {
      throw std::runtime_error( "i is too large in inverse_cumulative_normal_distribution::Table_Lookup");
    }
  }

  /// \brief Creates a pseudorandom normally distributed variable.
  ///
  /// This function assumes that the range of the functor r, is much greater than
  /// Number_Table_Rows_, or equivalently that range r % Number_Table_Rows==Number_Table_Rows-1.
  /// Also, r must be uniformly distributed.
  template< typename Rand>
    static double PseudoRandomNormalSample( Rand r) {
    int j = r() % Number_Virtual_Table_Rows_;
    return Table_Lookup( j);
  }

  /// \brief Creates a pseudorandom normally distributed variable.
  ///
  /// This function assumes that the range of the functor r, is much greater than
  /// Number_Table_Rows_, or equivalently that range r % Number_Table_Rows==Number_Table_Rows-1.
  /// Also, r must be uniformly distributed.
  template< typename Rand>
    static void PseudoRandomNormalSample( Rand r, const int N, double* Z) {
    //std::generate( Z, Z+N, Table_Lookup(r() % (Number_Virtual_Table_Rows_)));   
    int i, j;
    for (i=0; i<N; ++i) {
      j = r() % (Number_Virtual_Table_Rows_);
      Z[i] = Table_Lookup(j);      
    }
  };
};

#endif
    
