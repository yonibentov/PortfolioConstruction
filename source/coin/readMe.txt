This is part of the supplemental material for Sec. 5: Betting on a Coin Toss. 

You can build the program by calling CMake to generate the build script, then calling Make to build the target executable, coin.out.

The CMakeLists.txt file contains an instruction to write a header file called "basePath.h" into this directory. That header will then be used when compiling coin.c to read a string literal that contains the I/O base path for main() tailored to your system.

All parameters for this program have also been hardcoded: 100 coins, probability to win 75%. (I have also chosen to hard-code a particular utility function instead of using the library of utility functions that I use for the portfolio executable of Sec. 11.) If you want to change those, you can do so and rebuild the program.

While running, the program will write three files to storage: A text file that contains the parameters used, a CSV file that contains the optimal policy, and a CSV file that contains the approximate optimal policy after one iteration of approximation in policy space. Those files are to be read into the pertinent Wolfram notebook to generate the figures in Secs. 5.4 and 5.9.  
