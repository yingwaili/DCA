// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Peter Staar (peter.w.j.staar@gmail.com)
//
// Description

#ifndef MATH_LIBRARY_FUNCTIONAL_TRANSFORMS_BASIS_FUNCTIONS_HERMITE_SPLINES_HERMITE_SPLINE_H
#define MATH_LIBRARY_FUNCTIONAL_TRANSFORMS_BASIS_FUNCTIONS_HERMITE_SPLINES_HERMITE_SPLINE_H

#include <cmath>

namespace math_algorithms {
namespace functional_transforms {
// math_algorithms::functional_transforms::

struct hermite_spline {
public:
  //  u( 0 < |s| < 1 ) = 1-s
  //  u( 1 < |s| )     = 0
  template <typename lh_scalar_type, typename rh_scalar_type>
  static lh_scalar_type linear(lh_scalar_type x, rh_scalar_type y, rh_scalar_type d) {
    rh_scalar_type delta = std::abs((y - x) / d);

    if (delta >= 0 - 1.e-6 and delta < 1)
      return 1 - delta;

    return 0;
  }

  // u( 0 < |s| < 1 ) = (a+2)*s^3-(a+3)*s^2+1
  // u( 1 < |s| < 2 ) = a*s^3-5as^2+8as-4a
  // u( 2 < |s| )     = 0
  //
  // f[s_, a_] := Piecewise[{{0 = (a + 2)*s^3 - (a + 3)*s^2 + 1, Abs[s] >= 2},
  //                         {a*s^3 - 5 a s^2 + 8 a s - 4 a    , 2 >= Abs[s] >= 1},
  // 	                     {(a + 2)*s^3 - (a + 3)*s^2 + 1    , 1 >= Abs[s] >= 0}}]
  template <typename lh_scalar_type, typename rh_scalar_type>
  static lh_scalar_type cubic(lh_scalar_type x, rh_scalar_type y, rh_scalar_type d, rh_scalar_type a) {
    rh_scalar_type delta = std::abs((y - x) / d);

    if (delta >= 0 - 1.e-6 and delta < 1)
      return (a + 2) * std::pow(delta, 3) - (a + 3) * std::pow(delta, 2) + 1;

    if (delta >= 1 and delta < 2)
      return a * std::pow(delta, 3) - 5 * a * std::pow(delta, 2) + 8 * a * std::pow(delta, 1) - 4 * a;

    return 0;
  }
};

}  // functional_transforms
}  // math_algorithms

#endif  // MATH_LIBRARY_FUNCTIONAL_TRANSFORMS_BASIS_FUNCTIONS_HERMITE_SPLINES_HERMITE_SPLINE_H
