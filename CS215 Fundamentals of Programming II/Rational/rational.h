// File: rational.h
// header file for Rational ADT
// 
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 2           Date assigned: 2/5/16
// Programmer: Clayton Brutus              Date completed: 2/10/16

#ifndef RATIONAL_H_
#define RATIONAL_H_

#include <iostream>
#include <string>

class Rational {
  public:
   // Constructor with default values
   Rational(int initialNumerator=0, int initialDenominator=1);
   
   int getNumerator() const;   // returns numerator
   int getDenominator() const; // returns denominator
   
   Rational reciprocal() const;       // returns rational of reciprocal
   std::string asMixedFraction() const;  // returns string of mixed fraction
   
   // operators for 2 rationals
   friend Rational operator+(const Rational& l, const Rational& r);  // addition
   friend Rational operator-(const Rational& l, const Rational& r);  // subtraction
   friend Rational operator*(const Rational& l, const Rational& r);  // multiplication
   friend Rational operator/(const Rational& l, const Rational& r);  // division
   friend bool operator<(const Rational& l, const Rational& r);      // less than
   friend bool operator<=(const Rational& l, const Rational& r);     // less than or equal to
   friend bool operator>(const Rational& l, const Rational& r);      // greater than
   friend bool operator>=(const Rational& l, const Rational& r);     // greater than or equal to
   friend bool operator==(const Rational& l, const Rational& r);     // equal to
   friend bool operator!=(const Rational& l, const Rational& r);     // not equal to
   friend std::istream& operator>>(std::istream& in, Rational& target);  // input operator
   friend std::ostream& operator<<(std::ostream& out, const Rational& source);  // output operator
   
   
  private:
   int numerator;
   int denominator;
   
};

#endif // RATIONAL_H_
