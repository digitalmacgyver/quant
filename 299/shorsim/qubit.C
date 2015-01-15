#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "complex.h"

using namespace std;

class Qubit {
public:
  Qubit();            //Default constructor.
  virtual ~Qubit();           //Default destructor.
  int Measure();      //Returns zero_state = 0 or one_state = 1 in accordance
                      //with the probabilities of zero_state and one_state.
  void Dump() const;        //Prints our zero_state, and one_state, without 
                      //disturbing anything, this operation has no physical
                      //realization, it is only for information and debugging. 
                      //It should never be used in an algorithm for 
                      //information.
  void SetState(const Complex& zero_prob, const Complex& one_prob);
                      // Takes two complex numbers and sets the states to 
                      //those values.
  void SetAverage();  //Sets the state to 2^(1/2) zero_state, 2^(1/2) 
                      //one_state.  No imaginary/phase component.
  double MCC(int state) const;  //Multiply the zero or one state by its complex
                          //conjugate, and return the value.  This value 
                          //will always be a real number, with no imaginary 
                          //component.   

private:
  Complex zero_state;  
  //The probability of finding the Qubit in the zero or all imaginary 
  //state.  I currently use only the real portion.  

  Complex one_state;   
  //The probability of finding the Qubit in the one or all real state.
  //I currently use only the real portion.

  //|zero_state|^2 + |one_state|^2 should always be 1.
  //This notation means z_s * ComplexConjugate(z_s) + o_s * 
  //ComplexConjugate(o_s) = 1.
};

//Qubit constructor, initially sets things in the zero state.
Qubit::Qubit() {
  zero_state = Complex(1,0); 
  one_state = Complex(0,0);
  srand(time(NULL));
}

//Returns <state>_state * ComplexConjugate(<state>_state).
double Qubit::MCC(int state) const {
  if (state == 0) {
    return (pow(zero_state.Real(), 2) + pow(zero_state.Imaginary(), 2));
  } else {
    return (pow(one_state.Real(), 2) + pow(one_state.Imaginary(), 2));
  }
}

//Measurement operator.  Destructively collapses superpositions.
int Qubit::Measure() {
  double rand1 = rand()/(double)RAND_MAX;

  //This assumes that the sum of the squares of the amplitudes are = 1
  if (MCC(0) > rand1) {
    zero_state.Set(1,0);
    one_state.Set(0,0);
    return 0;
  } else {
    zero_state.Set(0,0);
    one_state.Set(1,0);
    return 1;
  } 
}

//Outputs state info for our qubit.  For debugging purposes.
void Qubit::Dump() const{
  cout << "Amplitude of the zero state is "
       << zero_state.Real() << " +i" << zero_state.Imaginary() << endl 
       << flush;
  cout << "Amplitude of the one state is "
       << one_state.Real() << " +i" << one_state.Imaginary() << endl 
       << flush;
}

//Sets the zero and one states to arbitrary amplitudes.  Outputs
//an error message if the two values MCC'ed != 1 + 0i.
void Qubit::SetState(const Complex& zero_prob, const Complex& one_prob) {
  zero_state = zero_prob;
  one_state = one_prob;
  //Determine if |zero_state|^2 + |one_state|^2 == 1, if not we 
  //are not in a valid state.
  double probab;
  probab = MCC(0) + MCC(1);
  if (fabs(probab - 1) > pow(10, -10)) {  
    //This funny expression 
    //allows us some rounding errors.
    cout << "Warning, total probability for in SetState is different from 1." << endl << flush;
  }
}

//Sets the qubit 1/2 way between the 0 state and the 1 state.  No phase.
void Qubit::SetAverage() {
  zero_state.Set(pow(2, -.5), 0);
  one_state.Set(pow(2, -.5), 0);
}
