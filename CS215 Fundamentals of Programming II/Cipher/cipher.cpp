// cipher.cpp performs a Caesar shift cipher on a text file and outputs to different text file.
#include <iostream>
#include <fstream> // file stream library
#include <cstdlib> // for atoi function

using namespace std;

int main(int argc, char* argv[]) { // argc = # of arguments, argv = array of arguments.
  if(argc != 4) {	// check for right amount of arguments.
    cerr << "USAGE: " << argv[0] << " inputfile outputfile shift\n";   // if incorrect number of args, then display error and exit program.
    return 1;
  }
  
  ifstream in(argv[1]);   // open input file
  ofstream out(argv[2]);  // open output file

  if (!in) {		  // check if input and output files are valid, if either is not valid then display error and exit.
    cerr << "Could not open " << argv[1] << endl;
    return 2;
  }

  if (!out) {
    cerr << "Could not open " << argv[2] << endl;
    return 3;
  }

  in >> noskipws; // tell the file stream to not skip whitespace.

  int shift = atoi (argv[3]);   // store shift number.
  char ch;
  while(in >> ch) {		// read a character from the input file.
    if (ch >= 'a' && ch <= 'z')
      ch = (((ch - 'a') + shift) % 26) + 'a'; // perform the shift on all upper or lower-case letters.
    else if (ch >= 'A' && ch <= 'Z')
      ch = (((ch - 'A') + shift) % 26) + 'A';
    
    out << ch;                 // write the character to the output file. repeat for all characters in the input file.
  }

    
  in.close();		// close the input file.
  out.close();		// close the output file.
  
  return 0;		// exit the program
  
}
