/// \file NewtonRaphson.cpp
/// \brief This is a demonstration of how to use NewtonRaphson.h<br />
/// This test the algorithm in 3 situations:<br />
///    1) calculating df at bouth points, to find the average df, for each iteration, with no boundaries <br />
///    2) calculating d2f to get second order approximation at each iteration, with no boundaries<br />
///    3) calculating d2f to get second order approximation at each iteration, and a boundary solution<br />

// to compile:  g++ -I../include -lm NewtonRaphson.cpp<br />

/***************************************************************************
 *   Copyright (C) 2009 by Clark Sims                                      *
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

#include "simpleoption/NewtonRaphson.h"
#include <math.h>

/// \brief This functor is provided as a demonstration of how to create a functor for the template class NetwonRaphson0. It encapsulates the function: exp(x) - offset.  The function variable is x. Offset is a hidden variable. This class is defined in the demonstration file NewtonRaphson.cpp.
class Newton_functor {
public:
  /// This is the hidden variable for class Newton_functor, which makes this class a functor, as opposed to a group of function pointers.
  double _offset;
  
  /// the creator for Newton_functor which assigns the hidden variable offset
  Newton_functor( double offset) : _offset(offset) { }

  /// the function to be solved
  double f( double x) {
    return exp( x) - _offset;
  }
  
  /// the derivative of the function to be solved
  double df( double x) {
    return exp( x);
  }
};

int main() {
  Newton_functor functor( 5);

  {
    NewtonRaphsonSolve0< Newton_functor, double > 
      newton( 
	     1.2,                  // X0
	     1e-8,                 // Epsilon
	     true,                 // Twice_df
	     100,                  // max_iter
	     functor,              // F
	     &Newton_functor::f,   // f
	     &Newton_functor::df); // df

    cout << "running iteration with f and df defined, over all real numbers" << endl;
    newton.do_iteration( &cout);

    cout << "running iteration with f, df and d2f defined, over all real numbers" << endl;
    newton.set_d2f( &Newton_functor::df);
    newton.do_iteration( &cout);

    cout << "running iteration with f, df and d2f defined, over the interval [1.6, infinity)" << endl;
    newton.set_check_boundary( true);
    newton.set_max_x( 1.6);
    newton.do_iteration( &cout);
  }

  cout << "running a numerically equivalent example, where functor._offset = 0, and newton._fsolve = 5.0" 
       << endl;

  functor._offset = 0;
  {
    NewtonRaphsonSolve0< Newton_functor, double > 
      newton( 
	     1.2,                  // X0
	     1e-8,                 // Epsilon
	     true,                 // Twice_df
	     100,                  // max_iter
	     functor,              // F
	     &Newton_functor::f,   // f
	     &Newton_functor::df,  // df
	     0,                    // d2f
	     5.0);                 // Fsolve

    cout << "running iteration with f and df defined, over all real numbers" << endl;
    newton.do_iteration( &cout);

    cout << "running iteration with f, df and d2f defined, over all real numbers" << endl;
    newton.set_d2f( &Newton_functor::df);
    newton.do_iteration( &cout);

    cout << "running iteration with f, df and d2f defined, over the interval [1.6, infinity)" << endl;
    newton.set_check_boundary( true);
    newton.set_max_x( 1.6);
    newton.do_iteration( &cout);
  }

  return 0;
}

