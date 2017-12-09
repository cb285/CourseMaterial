// File: editor2.cpp
// line by line text file editor using Document ADT
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 5           Date assigned: 3/23/16
// Programmer: Clayton Brutus              Date completed: 3/30/16

#include <iostream>   // cin, cout
#include <fstream>    // ifstream, ofstream
#include "document2.h" // Document ADT

using namespace std;

int main (int argc, char* argv[]) { // argc = # of arguments, argv = array of arguments
   
   if(argc != 2) {	// check for right amount of arguments
       cerr << "USAGE: " << argv[0] << " filename\n";
       return 1;
    }

  ifstream in(argv[1]);   // open input file
  
  // check if input file is valid
  if (!in) {
     cerr << "Could not open " << argv[1] << endl;
     return 2;
  }

  // create Document from input file
  Document doc(in);
  
  char command;
  string input;
  int n;

  // get command from user
  cout << "> ";
  cin >> command;

  // repeat until user enters Q:
  while (toupper(command) != 'Q') {

     // insert
     if (toupper(command) == 'I') {
	cin.ignore();
	getline(cin, input);
	doc.Insert(input);
     }

     // append
     if (toupper(command) == 'A') {
	cin.ignore();
	getline(cin, input);
	doc.Append(input);
     }

     // replace
     if (toupper(command) == 'R') {
	cin.ignore();
	getline(cin, input);
	try {
	doc.Replace(input);
	}
	catch (const domain_error & re)
	{
	   cout << re.what() << "  Try again!" << endl;
	}
     }

     // delete
     if (toupper(command) == 'D') {
	try {
	   doc.Delete();
	}
	catch (const domain_error & re)
	{
	   cout << re.what() << "  Try again!" << endl;
	}
     }

     // find
     if (toupper(command) == 'F') {
	cin.ignore();
	getline(cin, input);
	if (!doc.Find(input))
	   cout << "Couldn't find it." << endl;
     }

     // set
     if (toupper(command) == 'S') {
	cin.ignore();
	cin >> n;
	try {
	   doc.SetCurrent(n);
	}
	catch (const out_of_range & re)
	{
	   cout << re.what() << "  Try again!" << endl;
	}
     }

     // move
     if (toupper(command) == 'M') {
	cin.ignore();
	cin >> n;
	try {
	   doc.MoveCurrent(n);
	}
	catch (const out_of_range & re)
	{
	   cout << re.what() << "  Try again!" << endl;
	}
     }
     
     // display current line
     if (toupper(command) == 'C') {
	try {
	doc.WriteLine(cout);
	}
	catch (const domain_error & re) {
	   cout << re.what() << "  Try again!" << endl;
	}
     }

     // display entire document
     if (toupper(command) == 'L') {
	doc.WriteAll(cout);
     }

     // write document to file
     if (toupper(command) == 'W') {
	cin.ignore();
	getline(cin, input);
	ofstream out(input.c_str());  // open output file
	if (!out) {
	   cerr << "Could not open " << input << endl;
	   return 3;
	}
	doc.WriteAll(out);
	out.close();
     }

     // help menu
     if (toupper(command) == 'H') {
	cout << "Commands:" << endl;
	cout << "I line    - insert a line in front of current line" << endl;
	cout << "A line    - append line to end of document" << endl;
	cout << "R line    - replace current line" << endl;
	cout << "D         - delete the current line" << endl;
	cout << "F target  - makes line containing target current line" << endl;
	cout << "S n       - set current line to nth line" << endl;
	cout << "M n       - move current line n places" << endl;
	cout << "C         - display current line to screen" << endl;
	cout << "L         - displace entire document to screen" << endl;
	cout << "W file    - write document to file" << endl;
	cout << "H         - display this list of commands" << endl;
	cout << "Q         - quit the editor (does not save changes)" << endl;
	cout << endl;
     }

     // get next command from user
     cout << "> ";
     cin >> command;
  };

  in.close();
}
