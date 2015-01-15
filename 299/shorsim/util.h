#include "qureg.h"

#ifndef UTIL_H
#define UTIL_H

//This function takes an integer input and returns 1 if it is a prime
//number, and 0 otherwise.
unsigned long long int TestPrime(unsigned long long int n);

//This function takes an integer input and returns 1 if it is equal to
//a prime number raised to an integer power, and 0 otherwise.
unsigned long long int TestPrimePower(unsigned long long int n);

//This function computes the greatest common denominator of two integers.
//Since the modulus of a number mod 0 is not defined, we return a -1 as
//an error code if we ever would try to take the modulus of something and
//zero.
unsigned long long int GCD(unsigned long long int a, unsigned long long int b);

//This function takes and integer argument, and returns the size in bits
//needed to represent that integer.
unsigned long long int RegSize(unsigned long long int a);

//q is the power of two such that n^2 <= q < 2n^2.
unsigned long long int GetQ(unsigned long long int n);

//This function takes three integers, x, a, and n, and returns x^a mod
//n.  This algorithm is known as the "Russian peasant method," I
//believe, and avoids overflow by never calculating x^a directly.
unsigned long long int modexp(unsigned long long int x, unsigned long long int a, unsigned long long int n);

// This function finds the denominator q of the best rational
// denominator q for approximating p / q for c with q < qmax.
unsigned long long int denominator(double c, unsigned long long int qmax);

//This function takes two integer arguments and returns the greater of
//the two.
unsigned long long int max(unsigned long long int a, unsigned long long int b);

//This function computes the discrete Fourier transformation on a register's
//0 -> q - 1 entries. 
void DFT(QuReg * reg, unsigned long long int q);

#endif
