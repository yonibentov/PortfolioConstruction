This is the supplemental material for Sec. 11: Numerical Solution for Mean Reversion.

You can build the program by calling CMake to generate the build script, then calling Make to build the target executable, portfolio.out.

The main() function is in portfolio.c. Just as for coin.out, the CMakeLists.txt file contains an instruction to write a header file called "basePath.h" into this directory. That header will then be used when compiling portfolio.c to read a string literal that contains the I/O base path for main() tailored to your system. 

Unlike coin.out, portfolio.out is programmed to take user input. If portfolio.out is run without inputs, it will use the following default values:

 - volatility (sigma) = 1.00, 
 - asymptotic mean (m) = 0.00, 
 - mean-reversion speed (theta) = 0.01,
 - interest rate (r) = 0.01. 

To reproduce the results in the paper, run portfolio.out with the following command:

 % ./portfolio.out 1 0.31 0.26 0.02

(The % is the zsh prompt, not part of the call.)

The other simulation parameters like T and dt are hardcoded. 