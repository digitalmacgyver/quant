#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {
public:
  Complex();                  //Default constructor
  Complex( double, double );  // Argument constructor
  virtual ~Complex();          //Default destructor.
  void Set(double new_real, double new_imaginary); //Set data members.
  double Real() const;              //Return the real part.
  double Imaginary() const;         //Return the imaginary part.
  Complex & operator+(const Complex&); //Overloaded + operator
  Complex & operator*(const Complex&); //Overloaded * operator
  Complex & operator=(const Complex&); //Overloaded = operator
  bool operator==(const Complex&) const;    //Overloaded == operator
private:
  double real;
  double imaginary;
};

#endif
