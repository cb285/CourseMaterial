// File: document2.cpp
// Implementation of Document ADT
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 5           Date assigned: 3/23/16
// Programmer: Clayton Brutus              Date completed: 3/30/16

#include "document2.h"
#include "dnode2.h"

// Function: Document (default constructor)
// creates a blank document
Document::Document() {
   headPtr = new dnode();
   curr = headPtr; // point to header when empty
}

// Function: Document (explicit value constructor)
// creates a document from a input stream
Document::Document(std::istream& in) { // REC'D/P'BACK: input stream
   headPtr = new dnode();
   
   std::string line;
   dnode* cursor = headPtr;

   // insert each line of text into document
   while (std::getline(in, line)) {
      std::cout << line << std::endl;
      dlist_insert(cursor, line);
      cursor = cursor->next_link;
   }

   // set current to first line
   curr = headPtr->next_link;
}

// Function: Document (copy constructor)
// creates a copy of a document
Document::Document(const Document& source) {   
   // copy data from source to destination
   dlist_copy(source.headPtr, headPtr);

   // move current line to source's current line
   curr = headPtr;
   for (dnode* cursor = source.headPtr; cursor != source.curr; cursor = cursor->next_link)
      curr = curr->next_link;
}

// Function: ~Document (destructor)
// deletes a document
Document::~Document() {
   dlist_clear(headPtr); // delete lines
   delete headPtr;       // delete header
}  

// Function: operator= (assignment)
// copies a document into another document
Document&
Document::operator=(const Document& source) {
   // delete destination's original data
   dlist_clear(headPtr);
   delete headPtr;

   // copy data from source to destination
   dlist_copy(source.headPtr, headPtr);

   // move current line to source's current line
   curr = headPtr;
   dnode* cursor;
   for (cursor = source.headPtr; cursor != source.curr; cursor = cursor->next_link)
      curr = curr->next_link;
   
   return *this;
}

// Function: isEmpty
// Returns: true or false
// checks if document is empty
bool
Document::IsEmpty() const {
   if (dlist_length(headPtr) == 0)
      return true;
   return false;
}

// Function: insert
// inserts a line in front of the current line, then makes it the current line
void
Document::Insert(std::string newLine) { // REC'D: a string
   dlist_insert(curr->prev_link, newLine); // insert in front of current
   if (!IsEmpty())
      curr = curr->prev_link;      // set current line to new line
   else
      curr = headPtr->next_link;
}

// Function: append
// inserts a line at the end of the document, then makes it the current line
void
Document::Append(std::string newLine) { // REC'D: a string
   dlist_insert (headPtr->prev_link, newLine); // insert after last line
   curr = headPtr->prev_link;                  // set current to new line
}

// Function: replace
// replaces the contents of the current line
// if document is empty, then it's ignored
void
Document::Replace(std::string newLine) { // REC'D: a string
   if (!IsEmpty()) {
      dlist_insert(curr->prev_link, newLine);    // insert new line
      curr = curr->prev_link;     // set curr to new line
      dlist_remove(curr->next_link);          // remove the replaced line
   }
   else
      throw (std::domain_error("document is empty"));
}

// Function: delete
// removes the current line from the document
// the line after the deleted line becomes the current line
// if the last line is deleted, then it becomes the current line
void
Document::Delete() {
   if(!IsEmpty()) {
      if (curr == headPtr->prev_link) {
	 dlist_remove (curr);
	 curr = headPtr->prev_link;
      }
      else {
	 curr = curr->next_link;
	 dlist_remove (curr->prev_link);
      }
   }
   else
      throw (std::domain_error("document is empty"));
}

// Function: find
// Returns: true or false
// finds the first line in the document containing the target string,
// then makes that the current line
// if target isn't found then doesn't change current line
// returns true if found, false if not
bool
Document::Find(std::string target) { // REC'D: a string to search for
   if (!IsEmpty()) {
      for (dnode* i = headPtr->next_link; i != headPtr; i = i->next_link)
	 if (i->data.find(target) != std::string::npos) { // check each line
	    curr = i; // change current line to target line
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
void
Document::SetCurrent(int n) { // REC'D: line index
   // check if valid choice
   if (n < 1 || n > dlist_length(headPtr))
      throw (std::out_of_range("invalid line number choice"));

   // set current to nth line
   curr = headPtr->next_link;
   for (int i = 1; i < n; i++)
      curr = curr->next_link;
}

// Function: moveCurrent
// Returns: true or false
// moves the current line by n lines
// if n>0 moves forward, if n<0 moves backward
// if n is invalid, doesn't change current line
// returns true when successful, false otherwise
void
Document::MoveCurrent(int n) {
   // find current line number
   size_t line = 0;
   for (dnode* cursor = headPtr; cursor != curr; cursor = cursor->next_link)
      line++;

   // check if n is valid
   if (line + n > dlist_length(headPtr) || line + n < 1)
      throw (std::out_of_range("invalid number of lines to move"));

   // move current line
   curr = headPtr;
      for (int i = 0; i < line + n; i++)
	 curr = curr->next_link;

   
}

// Function: writeLine
// writes the current line to output stream
// if document is empty, then it's ignored
void
Document::WriteLine(std::ostream& out) const { // REC'D/P'BACK: output stream
   if (!IsEmpty())
      out << curr->data << std::endl; // write line to output stream
   else
      throw (std::domain_error("document is empty"));
}

// Function: writeALL
// writes all the lines in the document to a output stream
void
Document::WriteAll(std::ostream& out) const { // REC'D/P'BACK: output stream
   if (!IsEmpty())
      dlist_write (headPtr, out); // write all lines to output stream
}
