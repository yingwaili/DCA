// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Giovanni Balduzzi (gbalduzz@itp.phys.ethz.ch)
//
// This file implement the conversion of FourPointType to and from string.

#include "dca/phys/four_point_type.hpp"

#include <stdexcept>

namespace dca {
namespace phys {
// dca::phys::

FourPointType stringToFourPointMode(const std::string& name) {
  if (name == "NONE")
    return NONE;
  else if (name == "PARTICLE_PARTICLE_UP_DOWN")
    return PARTICLE_PARTICLE_UP_DOWN;
  else if (name == "PARTICLE_HOLE_TRANSVERSE")
    return PARTICLE_HOLE_TRANSVERSE;
  else if (name == "PARTICLE_HOLE_MAGNETIC")
    return PARTICLE_HOLE_MAGNETIC;
  else if (name == "PARTICLE_HOLE_CHARGE")
    return PARTICLE_HOLE_CHARGE;
  else
    throw std::logic_error("Invalid four point mode " + name);
}

std::string toString(const FourPointType type) {
  switch (type) {
    case NONE:
      return "NONE";
    case PARTICLE_PARTICLE_UP_DOWN:
      return "PARTICLE_PARTICLE_UP_DOWN";
    case PARTICLE_HOLE_TRANSVERSE:
      return "PARTICLE_HOLE_TRANSVERSE";
    case PARTICLE_HOLE_MAGNETIC:
      return "PARTICLE_HOLE_MAGNETIC";
    case PARTICLE_HOLE_CHARGE:
      return "PARTICLE_HOLE_CHARGE";
    default:
      throw std::logic_error("Invalid four point mode.");
  }
}

}  // phys
}  // dca
