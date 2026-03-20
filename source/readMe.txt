This is the root directory for the portion of the supplemental material that is written in C.

There are three executables to be built: 
 1. cargo/cargo.out, 
 2. coin/coin.out, 
 3. portfolio/portfolio.out. 

See their respective readMe.txt files for instructions and remarks. All programs were built and tested on the following platforms:

 1. macOS 26.3.1
    - Clang 17.0.0
    - CMake 3.29.2
    - GNU Make 3.81

 2. RHEL 10.1 virtual machine on Parallels Desktop 26.2.2
    - Clang 20.1.8
    - CMake 3.30.5
    - GNU Make 4.4.1
    - Valgrind 3.25.1

All three executables built on RHEL with compiler optimization disabled passed Valgrind's Memcheck.
When building on Linux, you may get compiler warnings about printing long int vs. long long int. 

I found the following books helpful for learning C:
 - "Extreme C" by Amini
 - "21st Century C" by Klemens
 - "C Interfaces and Implementations" by Hanson
 - "Effective C" by Seacord
 - "The C Programming Language" by Kernighan and Ritchie
 - "Pthreads Programming" by Nichols, Buttlar, and Farrell

Honorable mention to the following books on C++:
 - "Large-Scale C++ Software Design" by Lakos
 - "C++20 STL Cookbook" by Weinman

