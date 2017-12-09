// File: testrational.cpp 
// testing for Rational ADT
// 
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 2           Date assigned: 2/5/16
// Programmer: Clayton Brutus              Date completed: 2/10/16

#include <iostream>    // cin, cout
#include "rational.h"  // Rational class

using namespace std;

int main() {
   Rational r1, r2;
   
   // ask user for two rationals
   cout << "Enter a rational (n/d): ";
   cin >> r1;
      
   cout << "Enter another rational (n/d): ";
   cin >> r2;
      
   cout << "r1 = " << r1 << endl;
   cout << "r2 = " << r2 << endl;
   cout << "r1 = " << r1.asMixedFraction() << endl;
   cout << "r2 = " << r2.asMixedFraction() << endl;
   cout << "r1 reciprocal = " << r1.reciprocal() << endl;
   cout << "r2 reciprocal = " << r2.reciprocal() << endl;
   
   // test comparison functions
   if (r1 > r2)
      cout << "r1 > r2" << endl;
   if (r1 < r2)
      cout << "r1 < r2" << endl;
   if (r1 >= r2)
      cout << "r1 >= r2" << endl;
   if (r1 <= r2)
      cout << "r1 <= r2" << endl;
   if (r1 == r2)
      cout << "r1 == r2" << endl;
   if (r1 != r2)
      cout << "r1 != r2" << endl;
   
   // test +,-,*,/
   cout << "r1+r2 = " << r1+r2 << endl;
   cout << "r1-r2 = " << r1-r2 << endl;
   cout << "r1*r2 = " << r1*r2 << endl;
   cout << "r1/r2 = " << r1/r2 << endl;
   
   return 0;
}
