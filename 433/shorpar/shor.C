#include <iostream>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "complex.h"
#include "util.h"
#include "timer.C"
#include "range.h"
#include "barrier.h"

using namespace std;

unsigned long long int num_fact;
pthread_t * thread_array;

void *Shor_Sim(void *my_thread_id);

int main(int argc, char * argv[]) {
  double start_time, end_time;

  if(argc <= 2) {
    cout << "Usage: " << argv[0] << " <num to be factored> <num threads>\n";
    exit(0);
  }
  
  num_fact = atoi(argv[1]);
  num_threads = atoi(argv[2]);
 
  //The i'th element of q_range_lower is the lower limit that the i'th
  //process element should process, and q_range_upper's i'th element is
  //the upper limit on what the i'th thread should process.
  //The can be used in for loops like this:
  //for(int i = q_range_lower[proc_rank] ; i <= q_range_upper[proc_rank] ; i++)
  //Please note the <= in the boolean portion.
  
  q_range_lower = new int[num_threads];
  q_range_upper = new int[num_threads];

  //Just like q_range for n, the number to be factored.
  n_range_lower = new int[num_threads];
  n_range_upper = new int[num_threads];

  pthread_mutex_init(&global_barrier_mutex, 0);
  pthread_cond_init(&global_barrier_cond, NULL);

  thread_array = (pthread_t *) malloc(sizeof(pthread_t) * num_threads);

  for (unsigned int i = 0 ; i < num_threads ; i++) {
    pthread_create(&thread_array[i], 0, Shor_Sim, (void*)(intptr_t)i);
  }

  final_barrier(&final_barrier_counter, num_threads+1, &final_barrier_mutex, 
		&final_barrier_cond);
  
  return 0;
}

//n is the number we are going to factor, get n.
unsigned long long int n;

//Now we must pick a random integer x, coprime to n.  Numbers are
//coprime when their greatest common denominator is one.  One is not
//a useful number for the algorithm.
unsigned long long int x = 0;

//Now we must figure out how big a quantum register we need for our
//input, n.  We must establish a quantum register big enough to hold
//an equal superposition of all integers 0 through q - 1 where q is
//the power of two such that n^2 <= q < 2n^2.
unsigned long long int q;

//This array will remember what values of q produced for x^q mod n.
//It is necessary to retain these values for use when we collapse
//register one after measuring register two.  In a real quantum
//computer these registers would be entangled, and thus this extra
//bookkeeping would not be needed at all.  The laws of quantum
//mechanics dictate that register one would collapse as well, and
//into a state consistent with the measured value in resister two.
unsigned long long int * modex;     

//This array holds the probability amplitudes of the collapsed state
//of register one, after register two has been measured it is used
//to put register one in a state consistent with that measured in
//register two.
Complex *collapse;

//This is a temporary value.
Complex tmp;

//This is a new array of probability amplitudes for our second
//quantum register, that populated by the results of x^a mod n.
unsigned long long int array_size;

Complex *mdx; 

// This is the second register.  It needs to be big enough to hold
// the superposition of numbers ranging from 0 -> n - 1.
QuReg *reg2; 

//This is a temporary value.
unsigned long long int tmpval;
  
//This is a temporary value.
unsigned long long int value;
  
//c is some multiple lambda of q/r, where q is q in this program,
//and r is the period we are trying to find to factor n.  m is the
//value we measure from register one after the Fourier
//transformation.
double c,m; 

//This is used to store the denominator of the fraction p / den where
//p / den is the best approximation to c with den <= q.
unsigned long long int den;

//This is used to store the numerator of the fraction p / den where
//p / den is the best approximation to c with den <= q.
unsigned long long int p;

//The integers e, a, and b are used in the end of the program when
//we attempts to calculate the factors of n given the period it
//measured.
//Factor is the factor that we find.
unsigned long long int e,a,b, factor;

//Shor's algorithm can sometimes fail, in which case you do it
//again.  The done variable is set to 0 when the algorithm has
//failed.  Only try a maximum number of tries.
unsigned int done = 0;
unsigned int tries = 0;

//START TIME HERE!
double startTime;

//Create the register.
QuReg * reg1;

void *Shor_Sim(void *my_thread_id) {
  unsigned int thread_id = (unsigned int)(intptr_t)my_thread_id;
  //  cout << "My thread is " << thread_id << endl;
  
  //  cout << "My number to be factored is " << num_fact << endl;

  if (0 == thread_id) {
    //Establish a random seed.
    srand(time(NULL));
    
    n = num_fact;
    
    //Test to see if n is factorable by Shor's algorithm.
    //Exit if the number is even.
    if (n%2 == 0) {
      cout << "Error, the number must be odd!" << endl << flush;
      exit(0);
    } 
    //Exit if the number is prime.
    if (TestPrime(n)) {
      cout << "Error, the number must not be prime!" << endl << flush;
      exit(0);
    }
    //Prime powers are prime numbers raised to integral powers.
    //Exit if the number is a prime power.
    if (TestPrimePower(n)) {
      cout << "Error, the number must not be a prime power!" << endl << flush;
      exit(0);
    }
    
    //Now we must pick a random integer x, coprime to n.  Numbers are
    //coprime when their greatest common denominator is one.  One is not
    //a useful number for the algorithm.
    x = 1+ (unsigned long long int)((n-1)*(double)rand()/(double)RAND_MAX);
    while (GCD(n,x) != 1 || x == 1) {
      x = 1 + (unsigned long long int)((n-1)*(double)rand()/(double)RAND_MAX);
    }
    cout << "Found x to be " << x << "." << endl << flush;
    
    //Now we must figure out how big a quantum register we need for our
    //input, n.  We must establish a quantum register big enough to hold
    //an equal superposition of all integers 0 through q - 1 where q is
    //the power of two such that n^2 <= q < 2n^2.
    q = GetQ(n);
    cout << "Found q to be " << q << "." << endl << flush;
    
    init_ranges(num_threads, n, q);
    
    //Create the register.
    reg1 = new QuReg(RegSize(q) - 1);
    cout << "Made register 1 with register size = " << RegSize(q) << endl 
	 << flush;  
    
    //This array will remember what values of q produced for x^q mod n.
    //It is necessary to retain these values for use when we collapse
    //register one after measuring register two.  In a real quantum
    //computer these registers would be entangled, and thus this extra
    //bookkeeping would not be needed at all.  The laws of quantum
    //mechanics dictate that register one would collapse as well, and
    //into a state consistent with the measured value in resister two.
    modex = new unsigned long long int [q];     
    
    //This array holds the probability amplitudes of the collapsed state
    //of register one, after register two has been measured it is used
    //to put register one in a state consistent with that measured in
    //register two.
    collapse = new Complex[q];
  
    //This is a new array of probability amplitudes for our second
    //quantum register, that populated by the results of x^a mod n.
    array_size = (unsigned long long int) pow(2,RegSize(n));
    
    mdx = new Complex[array_size]; 
    
    // This is the second register.  It needs to be big enough to hold
    // the superposition of numbers ranging from 0 -> n - 1.
    reg2 = new QuReg(RegSize(n)); 
    cout << "Created register 2 of size " << RegSize(n) << endl << flush;
    
    //Shor's algorithm can sometimes fail, in which case you do it
    //again.  The done variable is set to 0 when the algorithm has
    //failed.  Only try a maximum number of tries.
    done = 0;
    tries = 0;
    
    //START TIME HERE!
    startTime = get_clock();
    
  } //Everything up to this point is pre processing done by thread 0.

  // Wait for everyone to finish.
  barrier(&global_barrier_counter1, num_threads, &global_barrier_mutex1, 
	  &global_barrier_cond1);

  while (!done) {
    
    if (0 == thread_id) {
      
      if (tries >= 5) {
	cout << "There have been five failures, giving up." << endl << flush;
	exit(0);
      }
      //Now populate register one in an even superposition of the
      //integers 0 -> q - 1.
      reg1->SetAverage(q - 1);
      
      //Now we preform a modular exponentiation on the superposed
      //elements of reg 1.  That is, perform x^a mod n, but exploiting
      //quantum parallelism a quantum computer could do this in one
      //step, whereas we must calculate it once for each possible
      //measurable value in register one.  We store the result in a new
      //register, reg2, which is entangled with the first register.
      //This means that when one is measured, and collapses into a base
      //state, the other register must collapse into a superposition of
      //states consistent with the measured value in the other..  The
      //size of the result modular exponentiation will be at most n, so
      //the number of bits we will need is therefore less than or equal
      //to log2 of n.  At this point we also maintain a array of what
      //each state produced when modularly exponised, this is because
      //these registers would actually be entangled in a real quantum
      //computer, this information is needed when collapsing the first
      //register later.
      
      //This counter variable is used to increase our probability amplitude.
      tmp.Set(1,0);
    }

    // Wait for all threads.
    barrier(&global_barrier_counter2, num_threads, &global_barrier_mutex2, 
	    &global_barrier_cond2);

    //This is the parallel version
    for (int i = q_range_lower[thread_id] ; i <= q_range_upper[thread_id] ; i++) {
      //We must use this version of modexp instead of c++ builtins as
      //they overflow when x^i > 2^31.
      tmpval = modexp(x,i,n);
      modex[i] = tmpval;
      mdx[tmpval] = mdx[tmpval] + tmp;
    }
    //END PARALLEL

    // Wait for other threads.
    barrier(&global_barrier_counter3, num_threads, &global_barrier_mutex3, 
	    &global_barrier_cond3);

    if (0 == thread_id) {
      //Set the state of register two to what we calculated it should be.
      reg2->SetState(mdx);
      
      //Normalize register two, so that the probability of measuring a
      //state is given by summing the squares of its probability
      //amplitude.
      reg2->Norm();
      
      //Now we measure reg1. 
      value = reg2->DecMeasure();
    }  

    // Wait for other threads.
    barrier(&global_barrier_counter4, num_threads, &global_barrier_mutex4, 
	    &global_barrier_cond4);

    //This is a parallelized version of this loop, which assumes the
    //code ad the top of this file has been appropriately
    //integrated.
    for (int i = q_range_lower[thread_id] ; i <= q_range_upper[thread_id] ; i++) {
      if (modex[i] == value) {
	collapse[i].Set(1,0);
      } else {
	collapse[i].Set(0,0);
      }
    }
    
    // Wait for other threads.
    barrier(&global_barrier_counter5, num_threads, &global_barrier_mutex5, 
	    &global_barrier_cond5);
    
    if (0 == thread_id) {
      //Now we set the state of register one to be consistent with what
      //we measured in state two, and normalize the probability
      //amplitudes.
      reg1->SetState(collapse);
      reg1->Norm();
      
      //Here we do our Fourier transformation.  
      cout << "Begin Discrete Fourier Transformation!" << endl << flush;
    }    

    // Wait for other threads.
    barrier(&global_barrier_counter6, num_threads, &global_barrier_mutex6, 
	    &global_barrier_cond6);

    DFT(reg1, q, thread_id);

    // Wait for other threads.
    barrier(&global_barrier_counter7, num_threads, &global_barrier_mutex7, 
	    &global_barrier_cond7);

    if (0 == thread_id) {
      //Next we measure register one, due to the Fourier transform the
      //number we measure, m will be some multiple of lambda/r, where
      //lambda is an integer and r is the desired period.
      m = reg1->DecMeasure();
      
      //If nothing goes wrong from here on out we are done.
      done = 1;
      
      //If we measured zero, we have gained no new information about the
      //period, we must try again.
      if (m == 0) {
	cout << "Measured, 0 this trial a failure!" << endl << flush;
	done = 0;
      }
      
      //The DecMeasure subroutine will return -1 as an error code, due
      //to rounding errors it will occasionally fail to measure a state.
      if (m == -1) {
	cout << "We failed to measure anything, this trial a failure!" 
	     << "  Trying again." << endl	<< flush;
	done = 0;
      }
      
      //If nothing has gone wrong, try to determine the period of our
      //function, and get factors of n.
      if (done) {
	//Now c =~ lambda / r for some integer lambda.  Borrowed with
	//modifications from Berhnard Ohpner.
	c = (double)m  / (double)q;
	
	//Calculate the denominator of the best rational approximation
	//to c with den < q.  Since c is lambda / r for some integer
	//lambda, this will provide us with our guess for r, our period.
	den = denominator(c, q);
	
	//Calculate the numerator from the denominator.
	p = (int)floor(den * c + 0.5);
	
	//Give user information.
	cout << "measured " << m <<", approximation for " << c << " is "
	     << p << " / " << den << endl << flush;
	
	//The denominator is our period, and an odd period is not
	//useful as a result of Shor's algorithm.  If the denominator
	//times two is still less than q we can use that.
	if (den % 2 == 1 && 2 * den < q ){
	  cout << "Odd denominator, expanding by 2\n";
	  p = 2 * p; 
	  den = 2 * den;
	}
	
	//Initialize helper variables.
	e = a = b = factor = 0;
	
	// Failed if odd denominator.
	if (den % 2 == 1)  {          
	  cout <<  "Odd period found.  This trial failed." 
	       << "  Trying again." << endl << flush;
	  done = 0;
	} else {
	  //Calculate candidates for possible common factors with n.
	  cout <<  "possible period is " << den << endl << flush;
	  e = modexp(x, den / 2, n);    
	  a = (e + 1) % n;     
	  b = (e + n - 1) % n;          
	  cout << x << "^" << den / 2 << " + 1 mod " << n << " = " << a 
	       << "," << endl 
	       << x << "^" << den / 2 << " - 1 mod " << n << " = " << b 
	       << endl << flush;
	  factor = max(GCD(n,a),GCD(n,b));
	}
      } //if done

      //GCD will return a -1 if it tried to calculate the GCD of two
      //numbers where at some point it tries to take the modulus of a
      //number and 0.
      if (factor == -1) {
	cout << "Error, tried to calculate n mod 0 for some n.  Trying again."
	     << endl << flush;
	done = 0;
      }
      
      if ((factor == n || factor == 1) && done == 1) {
	cout << "Found trivial factors 1 and " << n 
	     << ".  Trying again." << endl << flush;
	done = 0;
      }
      
      //If nothing else has gone wrong, and we got a factor we are
      //finished.  Otherwise start over.
      if (factor != 0 && done == 1) {
	cout << n << " = " << factor << " * " << n / factor << endl << flush;
      } else if (done == 1) {
	cout << "Found factor to be 0, error.  Trying again." << endl 
	     << flush;
	done = 0;
      }
      tries++;
    }//if 0 is thread id

    // Wait for other threads.
    barrier(&global_barrier_counter8, num_threads, &global_barrier_mutex8, 
	    &global_barrier_cond8);

  }//While not done

  if (0 == thread_id) {
    double endTime = get_clock();
    cout << "Runtime = " << endTime - startTime << " seconds" << endl;
  }

  final_barrier(&final_barrier_counter, num_threads+1, &final_barrier_mutex, 
	  &final_barrier_cond);

  pthread_exit(NULL);
}
