// File: concordance.cpp
// given a list of words, print them to a file in order and without duplicates
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 6           Date assigned: 4/1/16
// Programmer: Clayton Brutus              Date completed: 4/8/16

#include <iostream> // cout, cerr
#include <fstream>  // ifstream, ofstream
#include <list>     // list type
#include <vector>   // vector type

using namespace std;

template <typename T>
void InsertInOrderWithoutDuplicates(list<T>& strList, T word);

// Function: main
// reads from input file, inserts into vector of lists of strings,
// then writes the lists to output file
int main (int argc, char* argv[]) {  // REC'D: number of commandline arguments, strings of arguments
   // check number of commandline arguments
   if (argc != 3) {
      cerr << "Usage: " << argv[0] << " inputfile outputfile" << endl;
      return 1;
   }

   // open input and output files
   ifstream in(argv[1]);
   ofstream out(argv[2]);

   // check that files are valid
   if (!in) {
      cerr << "cannot open: " << argv[1] << endl;
      return 2;
   }

   if (!out) {
      cerr << "cannot open: " << argv[2] << endl;
      return 2;
   }

   // create vector of lists for storing words
   vector <list <string> > vectorOfStringLists(26);
   string word;

   // read from input file and insert words into vector of lists
   while (in >> word) {
      InsertInOrderWithoutDuplicates(vectorOfStringLists[word[0] - 'A'], word);
   }

   // write each word to output file
   for (size_t i = 0; i < vectorOfStringLists.size(); i++)
      for (list<string>::iterator it = vectorOfStringLists[i].begin(); it != vectorOfStringLists[i].end() ; it++)
	 out << *it << endl;

   in.close();
   out.close();
   
   return 0;
}

// Function: InsertInOrderWithoutDuplicates
// inserts a string into a list of strings in order and without duplicates
template <typename T>
void InsertInOrderWithoutDuplicates(list<T>& stlList, T data) {
   // if empty, insert into front of list
   if (stlList.empty()) {
      stlList.push_front(data);
      return;
   }

   // otherwise, loop through list
   for (typename list<T>::iterator it = stlList.begin(); it != stlList.end(); it++) {
      // if word is already in list, then do nothing
      if (*it == data)
	 return;
      
      // otherwise, insert in correct position
      else if (data < *it) {
	 stlList.insert(it, data);
	 return;
      }
   }
   stlList.insert(stlList.end(), data);
}
