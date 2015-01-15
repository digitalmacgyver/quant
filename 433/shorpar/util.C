#include <iostream>
#include <math.h>
#include "qureg.h"
#include "range.h"
#include "barrier.h"

//This function takes an integer input and returns 1 if it is a prime
//number, and 0 otherwise.
unsigned long long int TestPrime(unsigned long long int n) {
  unsigned long long int i;
  for (i = 2 ; i <= floor(sqrt(n)) ; i++) {
    if (n % i == 0) {
      return(0);
    }
  }
  return(1);
}

//This function takes an integer input and returns 1 if it is equal to a 
//prime number raised to an integer power, and 0 otherwise.
unsigned long long int TestPrimePower(unsigned long long int n) {
  unsigned long long int i,j;
  j = 0;
  i = 2;
  while ((i <= floor(pow(n, .5))) && (j == 0)) {
    if((n % i) == 0) {
      j = i;
    }
    i++;
  }
  for (unsigned long long int i = 2 ; i <= (floor(log(n) / log(j)) + 1) ; i++) { 
    if(pow(j , i) == n) {
      return(1);
    }
  }
  return(0);
}

//This function computes the greatest common denominator of two integers.
//Since the modulus of a number mod 0 is not defined, we return a -1 as
//an error code if we ever would try to take the modulus of something and
//zero.
unsigned long long int GCD(unsigned long long int a, unsigned long long int b) {
  unsigned long long int d;
  if (b != 0) {
    while (a % b != 0) {
      d = a % b;
      a = b;  
      b = d;
    }
  } else {
    return -1;
  }
  return(b);
}

//This function takes and integer argument, and returns the size in bits
//needed to represent that integer.
unsigned long long int RegSize(unsigned long long int a) {
  unsigned long long int size = 0;
  while(a != 0) {
    a = a>>1;
    size++;
  }
  return(size);
}


//q is the power of two such that n^2 <= q < 2n^2.
unsigned long long int GetQ(unsigned long long int n) {
  unsigned long long int power = 8; //265 is the smallest q ever is.
  while (pow(2,power) < pow(n,2)) {
    power = power + 1;
  }
  return((int)pow(2,power));
}

//This function takes three integers, x, a, and n, and returns x^a mod n.
//This algorithm is known as the "Russian peasant method," I believe.
unsigned long long int modexp(unsigned long long int x, unsigned long long int a, unsigned long long int n) {
  unsigned long long int value = 1;
  unsigned long long int tmp;
  tmp = x % n;
  while (a > 0) {
    if (a & 1) {
      value = (value * tmp) % n;
    }
    tmp = tmp * tmp % n;
    a = a>>1;
  }
  return value;
}


// This function finds the denominator q of the best rational
// denominator q for approximating p / q for c with q < qmax.
unsigned long long int denominator(double c, unsigned long long int qmax) {
  double y = c;
  double z;
  unsigned long long int q0 = 0;
  unsigned long long int q1 = 1;
  unsigned long long int q2 = 0;
  while (1) {
    z = y - floor(y);
    if (z < 0.5 / pow(qmax,2)) { 
      return(q1); 
    }
    if (z != 0) {
      //Can't divide by 0.
      y = 1 / z;
    } else {
      //Warning this is broken if q1 == 0, but that should never happen.
      return(q1);
    }
    q2 = (int)floor(y) * q1 + q0;
    if (q2 >= qmax) { 
      return(q1); 
    }
    q0 = q1; 
    q1 = q2;
  }
}

//This function takes two integer arguments and returns the greater of
//the two.
unsigned long long int max(unsigned long long int a, unsigned long long int b) {
  if (a > b) {
    return(a);
  }
  return(b);
}

//BEGIN PARALLEL This is where we need parallelism the most.

Complex * init = NULL;
unsigned long long int count = 0;

void DFT(QuReg * reg, unsigned long long int q, unsigned int thread_id) {
  //The Fourier transform maps functions in the time domain to
  //functions in the frequency domain.  Frequency is 1/period, thus
  //this Fourier transform will take our periodic register, and peak it
  //at multiples of the inverse period.  Our Fourier transformation on
  //the state a takes it to the state: q^(-.5) * Sum[c = 0 -> c = q - 1,
  //c * e^(2*Pi*i*a*c / q)].  Remember, e^ix = cos x + i*sin x.

  if (thread_id == 0) {
    if (init) {
      delete [] init;
    }
    init = new Complex[q];
  }

  // Wait for other threads.
  barrier(&global_barrier_counter, num_threads, &global_barrier_mutex, 
	  &global_barrier_cond);

  Complex tmpcomp( 0, 0 );

  //Here we do things that a real quantum computer couldn't do, such
  //as look as individual values without collapsing state.  The good
  //news is that in a real quantum computer you could build a gate
  //which would what this out all in one step.

  for (unsigned long long int a = 0 ; a < q ; a++) {
    //This if statement helps prevent previous round off errors from
    //propagating further.
    if ((pow(reg->GetProb(a).Real(),2) + 
	 pow(reg->GetProb(a).Imaginary(),2)) > pow(10,-14)) {
      // Limit our attention to the potion of the overall range our
      // thread is responsible for.
      for (unsigned long long int c = q_range_lower[thread_id] ; c <= q_range_upper[thread_id] ; c++) {
	tmpcomp.Set(pow(q,-.5) * cos(2*M_PI*a*c/(double)q),
		    pow(q,-.5) * sin(2*M_PI*a*c/(double)q));
	init[c] = init[c] + (reg->GetProb(a) * tmpcomp);
      }
    }
    
    // Log how thread_id 0 is doing.
    if (thread_id == 0) {
      count++;
      if (count == 1000) {
	cout << "Making progress in Fourier transform, " 
	     << 100*((double)a / (double)(q - 1)) << "% done!" 
	     << endl << flush;
	count = 0;
      }
    }
  }
  
  // Wait for the other threads to get here.
  barrier(&global_barrier_counter, num_threads, &global_barrier_mutex, 
	  &global_barrier_cond);

  if (thread_id == 0) {
    reg->SetState(init);
    reg->Norm();
  }
}

//END PARALLEL
