// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This file tests the SS-CT-HYB specialization of the McSolverParameters class.
//
// TODO: Add tests for get_buffer_size, pack, unpack and writing.

#include "dca/phys/parameters/mc_solver_parameters.hpp"
#include "gtest/gtest.h"
#include "dca/io/json/json_reader.hpp"

TEST(McSolverParametersSsCtHybTest, DefaultValues) {
  dca::phys::params::McSolverParameters<dca::phys::solver::SS_CT_HYB> pars;

  EXPECT_EQ(0, pars.get_Sigma_tail_cutoff());
  EXPECT_EQ(0.4, pars.get_steps_per_sweep());
  EXPECT_EQ(0.1, pars.get_swaps_per_sweep());
  EXPECT_EQ(0.4, pars.get_shifts_per_sweep());
}

TEST(McSolverParametersSsCtHybTest, ReadAll) {
  dca::io::JSONReader reader;
  dca::phys::params::McSolverParameters<dca::phys::solver::SS_CT_HYB> pars;

  reader.open_file(DCA_SOURCE_DIR
                   "/test/unit/phys/parameters/mc_solver_parameters/input_read_all_ss_ct_hyb.json");
  pars.readWrite(reader);
  reader.close_file();

  EXPECT_EQ(42, pars.get_Sigma_tail_cutoff());
  EXPECT_EQ(1.23, pars.get_steps_per_sweep());
  EXPECT_EQ(4.56, pars.get_swaps_per_sweep());
  EXPECT_EQ(7.89, pars.get_shifts_per_sweep());
}