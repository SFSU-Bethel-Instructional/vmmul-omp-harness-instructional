# vmmul instructional test harness

This directory contains a benchmark harness for testing different implementations of
vector-matrix multiply (VMM) for varying problem sizes.

The main code is benchmark.cpp, which sets up the problem, iterates over problem
sizes, sets up the vector and matrix, executes the vmmul call, and tests the
result for accuracy by comparing your result against a reference implementation (CBLAS).

Note that cmake needs to be able to find the CBLAS package. For CSC 746/656 Fall 2022,
this condition is true on Cori@NERSC and on the class VM. It is also true for some
other platforms, but you are on your own if using a platform other than Cori@NERSC
or the class VM.

# Build instructions - general

After downloading, cd into the main source directly, then:

% mkdir build  
% cd build  
% cmake ../  

Before these commands actually work, you may need to make some adjustments to your environment.
Below is information for Cori@NERSC, and for MacOS systems.

# Build peculiarities on Cori@NERSC

When building on Cori, make sure you are on a KNL node when doing the compilation. The
Cori login nodes are *not* KNL nodes, the Cori login nodes have Intel Xeon E5-2698
processors, not the Intel Xeon Phi 7250 (KNL) processors.  The simplest way to do this is
grab an interactive KNL node:  

salloc --nodes 1 --qos interactive --time 01:00:00 --constraint knl --account m3930

Also on Cori:

- cmake version: you need to use cmake/3.14 or higher. By default, Cori's cmake is cmake/3.17.0, 
which should be fine for this project.

- Programming environment. You need to use the Cray-Gnu compilers for this assignment. To access
them, please type "module swap PrgEnv-intel PrgEnv-gnu" on the command line.

# Build peculiarities for MacOSX platforms:

1. Compiler version. The default version of g++ shipped with the the development library is Apple clang version 12.0.5 (clang-1205.0.22.9) and this version of the compiler WILL NOT WORK with this assignment because it does not support OpenMP. The simplest fix is to install a new compiler: brew install gcc, which will install the most current version of gcc/g++, which is 11.2.0 as of the time of this writing.

There may be a way to force Apple's clang to enable OpenMP. See this thread, which Prof. Bethel has not tried: https://stackoverflow.com/questions/44380459/is-openmp-available-in-high-sierra-llvm/47230419#47230419

2. Setting the CXXFLAGS to point to the directory containing cblas.h.
On Prof. Bethel's laptop, which is an intel-based Macbook Pro running Big Sur (MacOS 11.6), and
with Xcode installed, cmake (version 3.20.1) can find the BLAS package, but then the build fails with
an error about not being able to find cblas.h.

The workaround is to tell cmake where cblas.h lives by using an environment variable:
export CXXFLAGS="-I /Library/Developer/CommandLineTools/SDKs/MacOSX11.3.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers"
then clean your build directory (rm -rf * inside build) and run cmake again. 

Note you will need to "locate cblas.h" on your machine and replace the path to cblas.h
in the CXXFLAGS line above with the path on your specific machine.



# Adding your code

For timing:

You will need to modify the benchmark.cpp code to add timing instrumentation, to 
report FLOPs executed, and so forth.


For vector-matrix multiplication:

There are stub routines inside dgemv-basic.cpp, dgemv-vectorized.cpp, and dgemv-openmp.cpp where you can add your code for doing basic, vectorized, and OpenMP-parallel vector-matrix multiply, respectively.

For the OpenMP parallel code, note that you specify concurrency at runtime using
the OMP_NUM_THREADS environment variable. While it is possible to set the number of
concurrent OpenMP threads at compile time, it is generally considered better practice to
specify the number of OpenMP threads via the OMP_NUM_THREADS environment variable.


# Running the codes

There is a sample job script provided for running the OpenMP code at 3 levels of concurrency: 1, 4, 8 threads. You may launch that script either as a batch job using the sbatch command, or you may run it as a shell script from an interactive node (preferred).

For the other codes -- benchmark-blas, benchmark-basic, and benchmark-vectorized -- it is easiest to run these from the command line from an interactive node by typing:

   % srun ./benchmark-basic  
   or  
   % srun ./benchmark-vectorized  
   or  
   % srun ./benchmark-blas

#eof
