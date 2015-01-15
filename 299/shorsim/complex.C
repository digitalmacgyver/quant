#include <math.h>
#include "complex.h"

//Complex constructor, initializes to 0 + i0.
Complex::Complex(): real( 0 ), imaginary( 0 ) {}

//Argument constructor.
Complex::Complex( double r, double i ): real( r ), imaginary( i ) {}

//Complex destructor.
Complex::~Complex() {}

//Overloaded = operator.
Complex & Complex::operator=(const Complex &c) {
  if (&c != this) {
    real = c.Real();
    imaginary = c.Imaginary();
    return *this;
  }
}

//Overloaded + operator.
Complex & Complex::operator+(const Complex &c) { 
  real += c.Real();
  imaginary += c.Imaginary(); 
  return *this;
}

//Overloaded * operator.
Complex & Complex::operator*(const Complex &c) { 
  real = real * c.Real() - imaginary * c.Imaginary();
  imaginary = real * c.Imaginary() + imaginary * c.Real();
  return *this;
}

//Overloaded == operator.  Small error tolerances.
bool Complex::operator==(const Complex &c) const {
  //This is to take care of round off errors.
  if (fabs(c.Real() - real) > pow(10,-14)) {
    return false;
  }
  if (fabs(c.Imaginary()- imaginary) > pow(10,-14)) {
    return false;
  }
  return true;
}

//Sets private data members.
void Complex::Set(double new_real, double new_imaginary) {
  real = new_real;
  imaginary = new_imaginary;
  return;
}

//Returns the real part of the complex number.
double Complex::Real() const {
  return real;
}

//Returns the imaginary part of the complex number.
double Complex::Imaginary() const {
  return imaginary;
}

