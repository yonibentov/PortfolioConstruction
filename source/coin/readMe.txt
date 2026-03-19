This is part of the supplemental material for Sec. 5: Betting on a Coin Toss. 

You can build the program by calling CMake to generate the build script, then calling Make to build the target executable, coin.out.

The CMakeLists.txt file contains an instruction to write a header file called "basePath.h" into this directory. That header will then be used when compiling coin.c to read a string literal that contains the I/O base path for main() tailored to your system.

All parameters for this program have also been hardcoded: 100 coins, probability to win 75%. (I have also chosen to hard-code a particular utility function instead of using the library of utility functions that I use for the portfolio executable of Sec. 11.) If you want to change those, you can do so and rebuild the program.

While running, the program will write 7 files to storage. The first is a text file that contains the parameters used. The next three are CSV files that contain the final decision, penultimate decision, and first decision under the exact dynamic-programming algorithm. The final three are the corresponding CSV files for one iteration of approximation in policy space. Those files are to be read into the pertinent Wolfram notebook to generate the figures in Sec. 5.
