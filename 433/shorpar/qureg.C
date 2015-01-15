#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "complex.h"
#include "qureg.h"

using namespace std;

QuReg::QuReg(): reg_size( 0 ), State( NULL ) {}

//Constructor.
QuReg::QuReg(unsigned long long int size) {
  reg_size = size;
  State = new Complex[(unsigned long long int)pow(2, reg_size)];
  srand(time(NULL));
}

//Copy Constructor
QuReg::QuReg(const QuReg & old) {
  reg_size = old.reg_size;
  unsigned long long int reg_length = (unsigned long long int) pow(2, reg_size);
  State = new Complex[reg_length];
  for (unsigned int i = 0 ; i < reg_length ; i++) {
    State[i] = old.State[i];
  }
}

//Destructor.
QuReg::~QuReg() {
  if ( State ) {
    delete [] State;
  }
}

//Return the probability amplitude of the state'th state.
Complex QuReg::GetProb(unsigned long long int state) const {
  if (state >= pow(2, reg_size)) {
    cout << "Invalid state index " << state << " requested!"
	 << endl << flush;
    throw -1;
  } else {
    return(State[state]);
  }
}

//Normalize the probability amplitude, this ensures that the sum of
//the sum of the squares of all the real and imaginary components is
//equal to one.
void QuReg::Norm() {
  double b = 0;
  double f, g;
  for (unsigned long long int i = 0; i < pow(2, reg_size) ; i++) {
    b += pow(State[i].Real(), 2) + pow(State[i].Imaginary(), 2);
  }
  b = pow(b, -.5);
  for (unsigned long long int i = 0; i < pow(2, reg_size) ; i++) {
    f = State[i].Real() * b;
    g = State[i].Imaginary() * b;
    State[i].Set(f, g);
  }
}

//Returns the size of the register.
int QuReg::Size() const {
  return reg_size;
}

//Measure a state, and return the decimal value measured.  Collapse
//the state so that the probability of measuring the measured value in
//the future is 1, and the probability of measuring any other state is
//0.
unsigned long long int QuReg::DecMeasure() {
  int done = 0;
  int DecVal = -1; //-1 is an error, we did not measure anything.
  double rand1, a, b;
  rand1 = rand()/(double)RAND_MAX;
  a = b = 0;
  for (unsigned long long int i = 0 ; i < pow(2, reg_size)  ;i++) {
    if (!done ){
      b += pow(State[i].Real(), 2) + pow(State[i].Imaginary(), 2);
      if (b > rand1 && rand1 > a) {
	//We have just measured the i state.
	for (unsigned long long int j = 0; j < pow(2, reg_size) ; j++) {
	  State[j].Set(0,0);
	}
	State[i].Set(1,0);
	DecVal = i;
	done = 1;
      }
      a += pow(State[i].Real(), 2) + pow(State[i].Imaginary(), 2);
    }
  }
  return DecVal;
}

//For debugging, output information about the register.
void QuReg::Dump(int verbose) const {
  for (unsigned long long int i = 0 ; i < pow(2, reg_size) ; i++) {
    if (verbose || fabs(State[i].Real()) > pow(10,-14) 
	|| fabs(State[i].Imaginary()) > pow(10,-14)) {
      cout << "State " << i << " has probability amplitude " 
	   << State[i].Real() << " + i" << State[i].Imaginary() 
	   << endl << flush;
    }
  }
}

//Set the states to those given in the new_state array.
void QuReg::SetState(Complex *new_state) {
  //Beware, this function will cause segfaults if new_state is too
  //small.
  for (unsigned long long int i = 0 ; i < pow(2, reg_size) ; i++) {
    State[i].Set(new_state[i].Real(), new_state[i].Imaginary());
  }
}

//Set the State to an equal superposition of the integers 0 -> number
//- 1
void QuReg::SetAverage(unsigned long long int number) {
  if (number >= pow(2, reg_size)) {
    cout << "Error, initializing past end of array in qureg::SetAverage.\n";
  } else {
    double prob = pow(number, -.5);
    for (unsigned long long int i = 0 ; i <= number ; i++) {
      State[i].Set(prob, 0);
    }
  }
}

       
