// File: document2.h
// header file for Document ADT
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 5           Date assigned: 3/23/16
// Programmer: Clayton Brutus              Date completed: 3/30/16

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <iostream> // istream, ostream
#include <vector>   // vector
#include <string>   // string type
#include <stdexcept> // domain_error
#include "dnode2.h"

class Document {
public:
   // constructors
   Document();
   Document(std::istream& in);
   Document(const Document& source);
   ~Document();
   
   Document& operator=(const Document& source);

   // accessor
   bool IsEmpty() const;
   void WriteLine(std::ostream& out) const;
   void WriteAll(std::ostream& out) const;

   // mutators
   void Insert(std::string newLine);
   void Append(std::string newLine);
   void Replace(std::string newLine);
   void Delete();
   bool Find(std::string target);
   void SetCurrent(int n);
   void MoveCurrent(int n);

private:
   // attributes:
   dnode* headPtr;
   dnode* curr;               // pointer to current line
};

#endif // DOCUMENT_H_
