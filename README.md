# The Power of Hashing with Mersenne Primes
In this paper we show how hashing with Mersenne primes can be made very fast, using a new intermediate representation for modular multiplication.
We further show that the resulting hash values have many useful properties not found with other polynomial hash functions.

![Table of speeds](https://raw.githubusercontent.com/thomasahle/mersenne/master/table.png)

Table: Milliseconds for 10^7 k-universal hashing operations.
The standard deviation is less than ±1ms.
The three CPUs tested are a) Intel Core i7-8850H; b) Intel Core i7-86650U; c) Intel Xeon E5-2690.

## Running the code

To run the carryless.cpp experiemnts, you need to run

Compiling on Mac:

    (clang|gcc) carryless.cpp -std=c++11 -march=native -lc++ -O3

Compiling on Linux:

    clang++ carryless.cpp -std=c++11 -march=native -O3
    
(Note that gcc and g++ have a tendency to mess up the optimization here.)

For the multiply shift experiments run
    
    multshift/runall.sh
    
For the fast Mersenne division algorithm:

    gcc div-test.c -lgmp -Ofast
    ./a.out 2> /dev/null | python3 subnop.py

For this you need GMP:
   http://rstudio-pubs-static.s3.amazonaws.com/493124_a46782f9253a4b8193595b6b2a037d58.html
