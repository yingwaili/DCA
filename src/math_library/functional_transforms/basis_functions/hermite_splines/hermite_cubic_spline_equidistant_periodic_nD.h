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

#ifndef MATH_LIBRARY_FUNCTIONAL_TRANSFORMS_BASIS_FUNCTIONS_HERMITE_SPLINES_HERMITE_CUBIC_SPLINE_EQUIDISTANT_PERIODIC_ND_H
#define MATH_LIBRARY_FUNCTIONAL_TRANSFORMS_BASIS_FUNCTIONS_HERMITE_SPLINES_HERMITE_CUBIC_SPLINE_EQUIDISTANT_PERIODIC_ND_H

#include "math_library/functional_transforms/basis_functions/hermite_splines/hermite_cubic_spline_template.h"
#include "math_library/functional_transforms/basis_functions/hermite_splines/hermite_spline.h"
#include "math_library/typedefs.hpp"

namespace math_algorithms {
namespace functional_transforms {
// math_algorithms::functional_transforms::

template <typename lh_dmn_type, typename rh_dmn_type, int DIMENSION>
class hermite_cubic_spline<lh_dmn_type, rh_dmn_type, EQUIDISTANT, PERIODIC, DIMENSION> {
private:
  typedef typename lh_dmn_type::dmn_specifications_type lh_spec_dmn_type;
  typedef typename rh_dmn_type::dmn_specifications_type rh_spec_dmn_type;

  typedef typename lh_spec_dmn_type::scalar_type lh_scalar_type;
  typedef typename rh_spec_dmn_type::scalar_type rh_scalar_type;

  typedef typename lh_spec_dmn_type::element_type lh_element_type;
  typedef typename rh_spec_dmn_type::element_type rh_element_type;

  typedef lh_scalar_type f_scalar_type;

public:
  static f_scalar_type execute(int i, int j) {
    const static rh_scalar_type a = -0.5;

    lh_element_type x = lh_dmn_type::get_elements()[i];
    rh_element_type y = rh_dmn_type::get_elements()[j];

    rh_scalar_type* super_basis = rh_dmn_type::get_super_basis();

    rh_scalar_type* inv_basis = rh_dmn_type::get_inverse_basis();
    rh_scalar_type* inv_super_basis = rh_dmn_type::get_inverse_super_basis();

    rh_element_type delta(DIMENSION, 0.);
    rh_element_type delta_affine(DIMENSION, 0.);

    f_scalar_type result = 0;

    {
      for (int li = 0; li < DIMENSION; li++)
        delta[li] = (y[li] - x[li]);

      {
        for (int li = 0; li < DIMENSION; li++)
          delta_affine[li] = 0.;

        for (int li = 0; li < DIMENSION; li++)
          for (int lj = 0; lj < DIMENSION; lj++)
            delta_affine[li] += inv_super_basis[li + lj * DIMENSION] * delta[lj];
      }

      for (int li = 0; li < DIMENSION; li++) {
        while (delta_affine[li] > 0.5 - 1.e-6)
          delta_affine[li] -= 1.;

        while (delta_affine[li] < -0.5 - 1.e-6)
          delta_affine[li] += 1.;
      }

      {
        for (int li = 0; li < DIMENSION; li++)
          delta[li] = 0.;

        for (int li = 0; li < DIMENSION; li++)
          for (int lj = 0; lj < DIMENSION; lj++)
            delta[li] += super_basis[li + lj * DIMENSION] * delta_affine[lj];
      }

      {
        for (int li = 0; li < DIMENSION; li++)
          delta_affine[li] = 0.;

        for (int li = 0; li < DIMENSION; li++)
          for (int lj = 0; lj < DIMENSION; lj++)
            delta_affine[li] += inv_basis[li + lj * DIMENSION] * delta[lj];
      }

      for (int li = 0; li < DIMENSION; li++)
        delta[li] = (delta_affine[li] > -2. and delta_affine[li] < 2.)
                        ? hermite_spline::cubic(0., delta_affine[li], 1., a)
                        : 0.;

      f_scalar_type t_result = 1;
      for (int li = 0; li < DIMENSION; li++)
        t_result *= delta[li];

      result += t_result;
    }

    return result;
  }
};

}  // functional_transforms
}  // math_algorithms

#endif  // MATH_LIBRARY_FUNCTIONAL_TRANSFORMS_BASIS_FUNCTIONS_HERMITE_SPLINES_HERMITE_CUBIC_SPLINE_EQUIDISTANT_PERIODIC_ND_H
