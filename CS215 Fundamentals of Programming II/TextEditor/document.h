// File: document.h
// header file for Document ADT
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 3           Date assigned: 2/12/16
// Programmer: Clayton Brutus              Date completed: 2/20/16

#include <iostream> // istream, ostream
#include <vector>   // vector
#include <string>   // string type

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

class Document {
public:
   // constructors
   Document();
   Document(std::istream& in);

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
   bool SetCurrent(int n);
   bool MoveCurrent(int n);

private:
   // attributes:
   std::vector<std::string> lines; // vector of each line of text file
   size_t curr;               // current line index
};

#endif // DOCUMENT_H_
