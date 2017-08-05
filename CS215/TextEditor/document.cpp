// File: document.cpp
// Implementation of Document ADT
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 3           Date assigned: 2/12/16
// Programmer: Clayton Brutus              Date completed: 2/20/16

#include "document.h"

// Function: Document
// creates a blank document
Document::Document() {
   curr = 0; // set current line to first line
}

// Function: Document
// creates a document from a input stream
Document::Document(std::istream& in) { // REC'D/P'BACK: input stream
   curr = 0; // set current line to 0
   
   std::string line;
   
   while (std::getline(in, line))
      lines.push_back(line);
}

// Function: isEmpty
// Returns: true or false
// checks if document has no lines
bool
Document::IsEmpty() const {
   if (lines.empty())
      return true;
   return false;
}

// Function: insert
// inserts a line in front of the current line, then makes it the current line
void
Document::Insert(std::string newLine) { // REC'D: a string
   lines.insert(lines.begin()+curr, newLine); // insert the new line
}

// Function: append
// inserts a line at the end of the document, then makes it the current line
void
Document::Append(std::string newLine) { // REC'D: a string
   // add line to end of document
   lines.push_back(newLine);
   
   if (lines.size() != 1)
      curr = lines.size() - 1;
}

// Function: replace
// replaces the contents of the current line
// if document is empty, then it's ignored
void
Document::Replace(std::string newLine) { // REC'D: a string
   if(!lines.empty())
      lines[curr] = newLine; // replace current line
}

// Function: delete
// removes the current line from the document
// the line after the deleted line becomes the current line
// if the last line is deleted, then it becomes the current line
void
Document::Delete() {
   if(!lines.empty()) {
      lines.erase(lines.begin()+curr); // remove current line

      if (curr == lines.size())
	 curr -= 1;
   }
}

// Function: find
// Returns: true or false
// finds the first line in the document containing the target string,
// then makes that the current line
// if target isn't found then doesn't change current line
// returns true if found, false if not
bool
Document::Find(std::string target) { // REC'D: a string to search for
   if (!lines.empty()) {
      for (size_t i = 0; i < lines.size(); i++)
	 if (lines[i].find(target) != std::string::npos) { // check each line
	    curr = i;
	    return true;
	 }
   }
   return false;
}

// Function: setCurrent
// returns true or false
// sets current line to the nth line of the document
// if n is invalid, doesn't change current line
// returns true when successful, false otherwise
bool
Document::SetCurrent(int n) { // REC'D: line index
   if (n < 1)
      return false;
   
   size_t temp = n;
   
   if (temp > lines.size() || temp < 1) // check if valid choice
      return false;
   
   curr = temp - 1;
   return true;
}

// Function: moveCurrent
// Returns: true or false
// moves the current line by n lines
// if n>0 moves forward, if n<0 moves backward
// if n is invalid, doesn't change current line
// returns true when successful, false otherwise
bool
Document::MoveCurrent(int n) {
   // check if n is valid
   if (curr + n > lines.size() - 1 || curr + n < 0)
      return false;

   // move current line
   curr += n;
   return true;
}

// Function: writeLine
// writes the current line to output stream
// if document is empty, then it's ignored
void
Document::WriteLine(std::ostream& out) const { // REC'D/P'BACK: output stream
   if (!lines.empty())
      out << lines[curr] << std::endl; // write line to output stream
}

// Function: writeALL
// writes all the lines in the document to a output stream
void
Document::WriteAll(std::ostream& out) const { // REC'D/P'BACK: output stream
   if (!lines.empty())
      for (size_t i=0; i < lines.size(); i++)
	 out << lines[i] << std::endl;  // write each line to output stream
}
