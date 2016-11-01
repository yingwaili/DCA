// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This file tests the tight-binding model specialization of the ModelParameters class.
//
// TODO: Add tests for get_buffer_size, pack, unpack and writing.

#include "dca/phys/parameters/model_parameters.hpp"
#include "gtest/gtest.h"
#include "dca/io/json/json_reader.hpp"
#include "dca/phys/domains/cluster/symmetries/point_groups/2d/2d_square.hpp"
#include "dca/phys/models/analytic_hamiltonians/square_lattice.hpp"

using PointGroup = dca::phys::domains::D4;
using Lattice = dca::phys::models::square_lattice<PointGroup>;

TEST(ModelParametersTightBindingTest, DefaultValues) {
  dca::phys::params::ModelParameters<dca::phys::models::TightBindingModel<Lattice>> pars;

  EXPECT_EQ(1., pars.get_t());
  EXPECT_EQ(0., pars.get_t_prime());
  EXPECT_EQ(4., pars.get_U());
  EXPECT_EQ(0., pars.get_V());
  EXPECT_EQ(0., pars.get_V_prime());
}

TEST(ModelParametersTightBindingTest, ReadAll) {
  dca::io::JSONReader reader;
  dca::phys::params::ModelParameters<dca::phys::models::TightBindingModel<Lattice>> pars;

  reader.open_file(DCA_SOURCE_DIR
                   "/test/unit/phys/parameters/model_parameters/input_read_all_tight_binding.json");
  pars.readWrite(reader);
  reader.close_file();

  EXPECT_EQ(10., pars.get_t());
  EXPECT_EQ(2., pars.get_t_prime());
  EXPECT_EQ(3., pars.get_U());
  EXPECT_EQ(4., pars.get_V());
  EXPECT_EQ(5., pars.get_V_prime());
}