// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Peter Staar (taa@zurich.ibm.com)
//         Raffaele Solca' (rasolca@itp.phys.ethz.ch)
//         Giovanni Balduzzi (gbalduzz@itp.phys.ethz.ch)
//
// This class measures the single-particle functions with an NFFT scheme.

#ifndef DCA_HAVE_CUDA
#error "This file requires CUDA."
#endif

#ifndef DCA_PHYS_DCA_STEP_CLUSTER_SOLVER_SHARED_TOOLS_ACCUMULATION_SP_SP_ACCUMULATOR_GPU_HPP
#define DCA_PHYS_DCA_STEP_CLUSTER_SOLVER_SHARED_TOOLS_ACCUMULATION_SP_SP_ACCUMULATOR_GPU_HPP

#include "dca/phys/dca_step/cluster_solver/shared_tools/accumulation/sp/sp_accumulator.hpp"

#include <cassert>
#include <complex>
#include <memory>
#include <stdexcept>
#include <vector>

#include "dca/linalg/util/cuda_stream.hpp"
#include "dca/math/nfft/dnfft_1d_gpu.hpp"
#include "dca/phys/error_computation_type.hpp"

namespace dca {
namespace phys {
namespace solver {
namespace accumulator {
// dca::phys::solver::accumulator::

template <class Parameters>
class SpAccumulator<Parameters, linalg::GPU> : public SpAccumulator<Parameters, linalg::CPU> {
private:
  using BaseClass = SpAccumulator<Parameters, linalg::CPU>;

  using typename BaseClass::BDmn;
  using typename BaseClass::WDmn;
  using typename BaseClass::RDmn;
  using typename BaseClass::KDmn;
  using typename BaseClass::PDmn;

  using typename BaseClass::Profiler;
  using typename BaseClass::ScalarType;

public:
  SpAccumulator(/*const*/ Parameters& parameters_ref, bool accumulate_m_squared = false);

  void initialize();

  template <class Configuration, typename InpScalar>
  void accumulate(const std::array<linalg::Matrix<InpScalar, linalg::CPU>, 2>& Ms,
                  const std::array<Configuration, 2>& configs, const int sign);

  void finalize();

  void sumTo(SpAccumulator<Parameters, linalg::GPU>& other);

  void synchronizeCopy(){
    cached_nfft_obj_[0].synchronizeCopy();
    cached_nfft_obj_[1].synchronizeCopy();
  }

  const auto& get_streams() const {
    return streams_;
  }

private:
  using BaseClass::oversampling;
  using BaseClass::parameters_;
  using BaseClass::accumulate_m_sqr_;
  using BaseClass::finalized_;
  using BaseClass::M_r_w_;
  using BaseClass::M_r_w_sqr_;

  std::array<linalg::util::CudaStream, 2> streams_;
  using NfftType = math::nfft::Dnfft1DGpu<ScalarType, WDmn, RDmn, oversampling, math::nfft::CUBIC>;
  std::array<NfftType, 2> cached_nfft_obj_;

  bool initialized_ = false;
};

template <class Paramaters>
SpAccumulator<Paramaters, linalg::GPU>::SpAccumulator(/*const*/ Paramaters& parameters_ref,
                                                      const bool accumulate_m_sqr)
    : BaseClass(parameters_ref, accumulate_m_sqr),
      streams_(),
      cached_nfft_obj_{NfftType(parameters_.get_beta(), streams_[0], accumulate_m_sqr),
                       NfftType(parameters_.get_beta(), streams_[1], accumulate_m_sqr)} {
  initialize();
}

template <class Paramaters>
void SpAccumulator<Paramaters, linalg::GPU>::initialize() {
  if (initialized_)
    return;

  for (int s = 0; s < 2; ++s)
    cached_nfft_obj_[s].initialize();
  initialized_ = true;
  finalized_ = false;
}

template <class Paramaters>
template <class Configuration, typename InpScalar>
void SpAccumulator<Paramaters, linalg::GPU>::accumulate(
    const std::array<linalg::Matrix<InpScalar, linalg::CPU>, 2>& Ms,
    const std::array<Configuration, 2>& configs, const int sign) {
  for (int s = 0; s < 2; ++s)
    cached_nfft_obj_[s].accumulate(Ms[s], configs[s], sign);
}

template <class Paramaters>
void SpAccumulator<Paramaters, linalg::GPU>::finalize() {
  if (finalized_)
    return;
  func::function<std::complex<ScalarType>, func::dmn_variadic<WDmn, PDmn>> tmp("tmp");
  const ScalarType normalization = 1. / RDmn::dmn_size();

  using MFunction = typename BaseClass::MFunction;
  // TODO: reuse base class.
  auto finalize_function = [&](std::array<NfftType, 2>& ft_objs, MFunction& function, bool m_sqr) {
    for (int s = 0; s < 2; ++s) {
      ft_objs[s].finalize(tmp, m_sqr);
      for (int w_ind = 0; w_ind < WDmn::dmn_size(); w_ind++)
        for (int r_ind = 0; r_ind < RDmn::dmn_size(); r_ind++)
          for (int b2_ind = 0; b2_ind < BDmn::dmn_size(); b2_ind++)
            for (int b1_ind = 0; b1_ind < BDmn::dmn_size(); b1_ind++)
              function(b1_ind, s, b2_ind, s, r_ind, w_ind) +=
                  tmp(w_ind, b1_ind, b2_ind, r_ind) * normalization;
    }
  };

  M_r_w_.reset(new MFunction("M_r_w"));
  finalize_function(cached_nfft_obj_, *M_r_w_, false);

  if (accumulate_m_sqr_) {
    M_r_w_sqr_.reset(new MFunction("M_r_w_sqr"));
    finalize_function(cached_nfft_obj_, *M_r_w_sqr_, true);
  }

  finalized_ = true;
  initialized_ = false;
}

template <class Paramaters>
void SpAccumulator<Paramaters, linalg::GPU>::sumTo(SpAccumulator<Paramaters, linalg::GPU>& other) {
  for (int s = 0; s < 2; ++s)
    other.cached_nfft_obj_[s] += cached_nfft_obj_[s];
}

}  // accumulator
}  // solver
}  // phys
}  // dca

#endif  // DCA_PHYS_DCA_STEP_CLUSTER_SOLVER_SHARED_TOOLS_ACCUMULATION_SP_SP_ACCUMULATOR_GPU_HPP
