//-*-C++-*-

// #include <cuda.h>
// #include <cublas.h>

// #include "magma.h"

// namespace CUDA_GPU
// {
//   /*!
//    *  \author R. Solca, P. Staar
//    */
//   template<typename scalartype>
//   struct GET_CUBLAS_TYPE {
//     typedef scalartype type;
//   };
  
//   template<>
//   struct GET_CUBLAS_TYPE<std::complex<float> > {
//     typedef cuComplex type;
//   };
  
//   template<>
//   struct GET_CUBLAS_TYPE<std::complex<double> > {
//     typedef cuDoubleComplex type;
//   };
  
//   /*!
//    *  \author R. Solca, P. Staar
//    */
//   template<typename scalartype, typename cuda_scalartype>
//   struct CAST_CUBLAS_TYPE 
//   {
//   public:
//     static void execute(scalartype& x, cuda_scalartype& y){
//       y = x;
//     }
//   };

//   template<>
//   struct CAST_CUBLAS_TYPE<std::complex<float>, cuComplex> 
//   {
//   public:
//     static void execute(std::complex<float>& a, cuComplex& b){
//       b.x = a.real();
//       b.y = a.imag();
//     }
//   };

//   template<>
//   struct CAST_CUBLAS_TYPE<std::complex<double>, cuDoubleComplex> 
//   {
//   public:
//     static void execute(std::complex<double>& a, cuDoubleComplex& b){
//       b.x = a.real();
//       b.y = a.imag();
//     }
//   };
  
// }

// #include "allocate_deallocate_cublas.h"

// #include "cublas_gemm.h"

// #include "magma_getrs.h"
