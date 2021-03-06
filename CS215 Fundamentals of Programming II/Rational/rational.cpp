// File: rational.cpp
// Implementation of Rational ADT
// Invarient: d > 0 and ratio must be in reduced form
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 2           Date assigned: 2/5/16
// Programmer: Clayton Brutus              Date completed: 2/10/16

#include "rational.h" // Rational ATD header
#include <iostream>   // cout, cin
#include <sstream>    // ostringstream
#include <string>     // string
#include <stdlib.h>   // abs
#include <assert.h>   // assert

// Explicit value constructor with default arguments
Rational::Rational(int initialNumerator, int initialDenominator) {
   int m = abs(initialNumerator);
   int n = abs(initialDenominator);
   int storage = m;
   
   // Find LCD
   if (m>n) {
      m = n;
      n = storage;
   }
   while (n > 0) {
      storage = n;
      n = m % n;
      m = storage;
   }
   
   // divide by LCD
   numerator = initialNumerator / m;
   denominator = initialDenominator / m;
   
   if (denominator < 0) {
      denominator *= -1;
      numerator *= -1;
   }
   
   // Check invarient
   assert (denominator > 0);
}

// Function: getNumerator
// Returns:  numerator of rational
int
Rational::getNumerator() const {
   return numerator;
}

// Function: getDenominator
// Returns:  denominator of rational
int
Rational::getDenominator() const {
   return denominator;
}

// Function: reciprocal
// Returns:  the reciprocal of rational
Rational
Rational::reciprocal() const {
   return Rational(denominator, numerator);
}

// Function: asMixedFraction
// Returns:  string containing mixed fraction
std::string
Rational::asMixedFraction() const {
   // Declare an ostringstream
   std::ostringstream ostr;
   // Put the string into ostr
   if (denominator == 1)
      ostr << numerator;
   else if (numerator / denominator != 0)
      ostr << numerator / denominator << "+" << numerator % denominator << "/" << denominator;
   else
      ostr << numerator << "/" << denominator;
   // return the string
   return ostr.str();
}

// Function: operator+
// Returns:  sum of two rationals
Rational operator+(const Rational& l, const Rational& r) { // REC'D: two rationals
   return Rational((l.getNumerator() * r.getDenominator()) + (r.getNumerator() * l.getDenominator()),
		   l.getDenominator() * r.getDenominator());
}

// Function: operator-
// Returns:  difference of two rationals
Rational operator-(const Rational& l, const Rational& r) { // REC'D: two rationals
   return Rational((l.getNumerator() * r.getDenominator()) - (r.getNumerator() * l.getDenominator()),
		   l.getDenominator() * r.getDenominator());
}

// Function: operator*
// Returns:  product of two rationals
Rational operator*(const Rational& l, const Rational& r) { // REC'D: two rationals
   return Rational(l.getNumerator() * r.getNumerator(), l.getDenominator() * r.getDenominator());
}

// Function: operator/
// Returns:  quotient of two rationals
Rational operator/(const Rational& l, const Rational& r) { // REC'D: two rationals
   return Rational(l.getNumerator() * r.getDenominator(), l.getDenominator() * r.getNumerator());
}

// Function: operator<
// Returns: true or false
bool operator<(const Rational& l, const Rational& r) { // REC'D: two rationals
   return (l.getNumerator() * r.getDenominator() < r.getNumerator() * l.getDenominator());
}

// Function: operator<=
// Returns: true or false
bool operator<=(const Rational& l, const Rational& r) { // REC'D: two rationals
   return (l < r || l == r);
}

// Function: operator>
// Returns: true or false
bool operator>(const Rational& l, const Rational& r) { // REC'D: two rationals
   return (r < l);
}

// Function: operator>=
// Returns: true or false
bool operator>=(const Rational& l, const Rational& r) { // REC'D: two rationals
   return (l > r || l == r);
}

// Function: operator==
// Returns: true or false
bool operator==(const Rational& l, const Rational& r) { // REC'D: two rationals
   return (l.getNumerator() * r.getDenominator() == r.getNumerator() * l.getDenominator());
}

// Function: operator!=
// Returns: true or false
bool operator!=(const Rational& l, const Rational& r) { // REC'D: two rationals
   return !(l == r);
}

// Function: operator>>
// Returns: input stream
std::istream& operator>>(std::istream& in,   // REC'D:  input stream
			 Rational& target) { // P'BACK: Rational
   int n, d;
   char t;
   // read in numerator
   in >> n;
   
   // check if there is a denominator
   if(in.peek() == '/') {
      // read in denominator
      in >> t;
      in >> d;
   }
   else
      d = 1; // set denom. to 1 if not present
   
   if (in.fail())  // stream failed or eof
      return in;
   if (d == 0) {   // invarient
      in.setstate (std::ios_base::failbit);
      return in;
   }
   
   // create rational from data
   target = Rational (n, d);
   return in;
}

// Function: operator<<
// Returns:  output stream
std::ostream& operator<<(std::ostream& out,        // REC'D: output stream
			 const Rational& source) { // REC'D: rational
   if (source.getDenominator() == 1)
      out << source.getNumerator();
   else
      out << source.getNumerator() << "/" << source.getDenominator();
   return out;
}
