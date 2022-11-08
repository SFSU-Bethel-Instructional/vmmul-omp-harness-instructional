cmake_minimum_required(VERSION 3.14)
project(hw2 LANGUAGES C CXX)

message(STATUS "compiler = " ${CMAKE_C_COMPILER_ID})

set(CMAKE_BUILD_TYPE "Release")

find_package(BLAS REQUIRED)

# We require the Intel compiler for this assignment.
if (NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
#    set(Prg-Intel "PrgEnv-intel")
#    set(Prg-Clang "PrgEnv-cray")
    message(WARNING
            "Must use Intel Compiler for building, testing, and submission.. Make sure you ran:\n"
            "module swap ${Prg-${CMAKE_C_COMPILER_ID}} PrgEnv-intel")
endif ()

# Common library target for benchmarking.
add_library(benchmark OBJECT benchmark.cpp)
target_compile_features(benchmark PRIVATE cxx_std_11)

add_executable(benchmark-basic dgemv-basic.cpp)
target_link_libraries(benchmark-basic PRIVATE benchmark ${BLAS_LIBRARIES})
target_compile_features(benchmark-basic PRIVATE c_std_11 c_restrict)
target_compile_options(benchmark-basic PRIVATE -Wall -pedantic -march=native -O1)
#set(BENCHMARK "basic")
#configure_file(job.in job-${BENCHMARK})

add_executable(benchmark-vectorized dgemv-vectorized.cpp)
target_link_libraries(benchmark-vectorized PRIVATE benchmark ${BLAS_LIBRARIES})
target_compile_features(benchmark-vectorized PRIVATE c_std_11 c_restrict)
# the following line is for GCC.
#target_compile_options(benchmark-vectorized PRIVATE -Wall -pedantic -O3 -march=knl -mprefer-vector-width=512 -mavx512f -mavx512cd -fopt-info-vec-all=report.txt)
# the following line is for ICC, the Intel compiler. It does a better job of generating AVX512 instructions than GCC. this line will generate a vectorization report that lands in a file named dgemv-vectorized.cpp.optrpt
target_compile_options(benchmark-vectorized PRIVATE -Wall -pedantic -O2 -xMIC-AVX512 -qopt-report=2 -qopt-report-phase=vec)

#set(BENCHMARK "vectorized")
#configure_file(job.in job-${BENCHMARK})

add_executable(benchmark-blas dgemv-blas.cpp)
target_link_libraries(benchmark-blas PRIVATE benchmark ${BLAS_LIBRARIES})
target_compile_features(benchmark-blas PRIVATE c_std_11 c_restrict)
target_compile_options(benchmark-blas PRIVATE -Wall -pedantic -march=native)
#set(BENCHMARK "blas")
#configure_file(job.in job-${BENCHMARK})

add_executable(benchmark-openmp dgemv-openmp.cpp)
target_link_libraries(benchmark-openmp PRIVATE benchmark ${BLAS_LIBRARIES})
target_link_options(benchmark-openmp PRIVATE -fopenmp)
target_compile_features(benchmark-openmp PRIVATE c_std_11 c_restrict)
target_compile_options(benchmark-openmp PRIVATE -fopenmp -Wall -pedantic -march=native -O1)
set(BENCHMARK "openmp")
configure_file(job.in job-${BENCHMARK})

# EOF

