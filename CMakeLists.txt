cmake_minimum_required(VERSION 3.14)
project(cp3 LANGUAGES C CXX)

message(STATUS "compiler = " ${CMAKE_C_COMPILER_ID})

set(CMAKE_BUILD_TYPE "Release")

# set default optimization level to -O0, but override in specific cases below
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

find_package(BLAS REQUIRED)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
   message(STATUS "setting vector flags for gcc compiler = " ${CMAKE_C_COMPILER_ID})
# produces best results on perlmutter and AMD 7763 processor
   set(VFLAGS -O3 -DNDEBUG -Wall -march=znver1 -mtune=znver1 -mfma -mavx2 -m3dnow -mprefer-vector-width=256 -fomit-frame-pointer -ftree-vectorize -funroll-loops -ffast-math -fopt-info-vec-all=report.txt)
# alternate for AMD 7763: produces significant speedup over basic   set(VFLAGS -O2 -DNDEBUG -Wall -march=znver1 -mtune=znver1 -mavx2 -ftree-vectorize  -ffast-math -fopt-info-vec-all=report.txt)
# conservative, portable   set(VFLAGS  -O3  -fopt-info-vec-all=report.txt)
#   set(VFLAGS  -O3 -mprefer-vector-width=512 -mavx512f -mavx512cd -fopt-info-vec-all=report.txt)
#   set(VFLAGS  -O2 -march=knl -mprefer-vector-width=512 -mavx512f -mavx512cd -fopt-info-vec-all=report.txt)
#   message(STATUS "vectorization flags for gcc compiler are" ${VFLAGS})
endif ()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
   message(STATUS "setting vector flags for icc compiler = " ${CMAKE_C_COMPILER_ID})
# the following line is for ICC, the Intel compiler. It does a better job of generating AVX512 instructions than GCC. this line will generate a vectorization report that lands in a file named dgemv-vectorized.cpp.optrpt
   set(VFLAGS -O2 -xMIC-AVX512 -qopt-report=2 -qopt-report-phase=vec)
#   message(STATUS "vectorization flags for icc compiler are" ${VectorizationFlags})
endif ()

# clang compiler stuff not ready yet
#if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
#   message(STATUS "setting vector flags for clang++ compiler = " ${CMAKE_C_COMPILER_ID})
#   set(VFLAGS -O3 -fvectorize -ftree-vectorize  -mprefer-vector-width=512 -Rpass-analysis=loop-vectorize  -fsave-optimization-record -ffast-math)
#   set(VFLAGS -O2 -fvectorize -mprefer-vector-width=512 -Rpass=vector)
#
#endif ()

# Common library target for benchmarking.
add_library(benchmark OBJECT benchmark.cpp)
target_compile_features(benchmark PRIVATE cxx_std_11)

add_executable(benchmark-basic dgemv-basic.cpp)
target_link_libraries(benchmark-basic PRIVATE benchmark ${BLAS_LIBRARIES})
target_compile_features(benchmark-basic PRIVATE c_std_11 c_restrict)
target_compile_options(benchmark-basic PRIVATE -Wall -pedantic -march=native -O1)

add_executable(benchmark-vectorized dgemv-vectorized.cpp)
target_link_libraries(benchmark-vectorized PRIVATE benchmark ${BLAS_LIBRARIES})
target_compile_features(benchmark-vectorized PRIVATE c_std_11 c_restrict)
target_compile_options(benchmark-vectorized PRIVATE -Wall -pedantic ${VFLAGS} ) 

add_executable(benchmark-blas dgemv-blas.cpp)
target_link_libraries(benchmark-blas PRIVATE benchmark ${BLAS_LIBRARIES})
target_compile_features(benchmark-blas PRIVATE c_std_11 c_restrict)
target_compile_options(benchmark-blas PRIVATE -Wall -pedantic -march=native)

add_executable(benchmark-openmp dgemv-openmp.cpp)
target_link_libraries(benchmark-openmp PRIVATE benchmark ${BLAS_LIBRARIES})
target_link_options(benchmark-openmp PRIVATE -fopenmp)
target_compile_features(benchmark-openmp PRIVATE c_std_11 c_restrict)
target_compile_options(benchmark-openmp PRIVATE -fopenmp -Wall -pedantic -march=native -O1)
set(BENCHMARK "openmp")
configure_file(job.in job-${BENCHMARK})

# EOF

