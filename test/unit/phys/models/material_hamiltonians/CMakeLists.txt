# Material Hamiltonians unit tests

dca_add_gtest(material_lattice_NiO_test
  GTEST_MAIN
  LIBS function ${LAPACK_LIBRARIES} ${DCA_CUDA_LIBS})
