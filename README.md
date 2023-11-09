# vmmul omp instructional test harness

This directory contains a benchmark harness for testing different implementations of
vector-matrix multiply (VMM) for varying problem sizes.

The main code is benchmark.cpp, which sets up the problem, iterates over problem
sizes, sets up the vector and matrix, executes the vmmul call, and tests the
result for accuracy by comparing your result against a reference implementation (CBLAS).

Note that cmake needs to be able to find the CBLAS package. For CSC 746/656 Fall 2023,
this condition is true on Perlmutter@NERSC and on the class VM. It is also true for some
other platforms, but you are on your own if using a platform other than Perlmutter@NERSC
or the class VM.

<br></br>

# Build instructions for Perlmutter and the VM

Perlmutter only: After logging in to Perlmutter, first set up your environment by typing this command:

    module load cpu  

On the VM: no special environment setup is needed


Then, build the code. First, cd into the main source directory (vmmul-omp-harness-instructional) and then enter the following commands:

    mkdir build  
    cd build  
    cmake ../  
    make  

<br></br>

# Running the codes on Perlmutter

After building the codes, it is ok to do very brief runs on login nodes for debug purposes.
Here, "brief" means < 10 second runs.

When you are ready to do builds/runs on a Perlmutter CPU node, use the salloc command to hop onto a CPU node:

    salloc --nodes=1 --qos=interactive --time=00:15:00 --constraint=cpu --account=m3930

There is a sample job script provided in the code harness for running the OpenMP code at 4 levels of concurrency: 1, 4, 16, 64 threads. You may launch that script either as a batch job using the sbatch command, or you may run it as a shell script from an interactive node (preferred).

From the build directory, run as a shell script on an interactive CPU node:

    bash ./job-openmp

For the other codes -- benchmark-blas, benchmark-basic, and benchmark-vectorized -- it is easiest to run these from the command line from an interactive node from your build directoy by typing:

    ./benchmark-basic  

or  

    ./benchmark-vectorized   

or  

    ./benchmark-blas  

# Build peculiarities for MacOSX platforms:

1. Compiler version. The default version of g++ shipped with the the development library on MacOS 12.6.8 (Monterey) is clang version 12.0.5 (clang-1205.0.22.9) and this version of the compiler WILL NOT WORK with this assignment because it does not support OpenMP. The simplest fix is to install a new compiler: brew install gcc, which will install the most current version of gcc/g++, which is 12.2.0 (for MacOSC 12.6.8, Monterey) as of the time of this writing.

There may be a way to force Apple's clang to enable OpenMP. See this thread, which Prof. Bethel has not tried: https://stackoverflow.com/questions/44380459/is-openmp-available-in-high-sierra-llvm/47230419#47230419

2. Setting the CXXFLAGS to point to the directory containing cblas.h.
On Prof. Bethel's laptop, which is an intel-based Macbook Pro running MacOS 12.6.8 (Monterey), with
Xcode installed, cmake (version 3.20.1) can find the BLAS package, but then the build fails with
an error about g++ not being able to find cblas.h.

The workaround is to tell cmake where cblas.h lives by using an environment variable:

    export CXXFLAGS="-I /path/to/headers"  

then clean your build directory (rm -rf * inside build) and run cmake again.  Note: you  need to replace /path/to/headers with
the full path to the directory containing the cblas.h header on your machine.

Note you will need to "locate cblas.h" on your machine and replace the path to cblas.h
in the CXXFLAGS line above with the path on your specific machine.

Run the command:  

    locate cblas.h  

which on Prof. Bethel's laptop produces the following output:

    /Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers/cblas.h  
    /Library/Developer/CommandLineTools/SDKs/MacOSX11.3.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers/cblas.h  
    /usr/local/Cellar/openblas/0.3.23/include/cblas.h 



Use the path to the newest headers, here the MacOSX11.3.sdk version:
    
    export CXXFLAGS = "-I /Library/Developer/CommandLineTools/SDKs/MacOSX11.3.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers/"   

Then clean your build directory, and rerun cmake then make.

<br></br>

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

<br></br>

#eof
