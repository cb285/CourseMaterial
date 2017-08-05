// Clayton Brutus
// homework 2: Change

#include <iostream>
#include <fstream>  // for reading and writing files
#include <cstdlib>  // atoi function
#include <iomanip>  // for setting field width in output file (setw)

using namespace std;

void ComputeChange (int cents, int& dollars, int& quarters, int& dimes, int& pennies);

int main (int argc, char* argv[]) { // argc = # of arguments, argv = array of arguments
  
    if(argc != 3) {	// check for right amount of arguments
	cerr << "USAGE: " << argv[0] << " inputfile outputfile\n";   // if incorrect number of args, then display error and exit program.
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

  const int FIELD_WIDTH = 10;
  
  out << setw(FIELD_WIDTH) << "Amount";       // write table headers to file
  out << setw(FIELD_WIDTH) << "Dollars";
  out << setw(FIELD_WIDTH) << "Quarters";
  out << setw(FIELD_WIDTH) << "Dimes";
  out << setw(FIELD_WIDTH) << "Pennies";
  out << endl << endl;

  int cents, dollars, quarters, dimes, pennies;   // make variables for change amounts

  while(in >> cents) {		// read a number from the input file.
      ComputeChange (cents, dollars, quarters, dimes, pennies);
      
      out << setw(FIELD_WIDTH) << cents;
      out << setw(FIELD_WIDTH) << dollars;
      out << setw(FIELD_WIDTH) << quarters;
      out << setw(FIELD_WIDTH) << dimes;
      out << setw(FIELD_WIDTH) << pennies;
      out << endl;
  }

  in.close();		// close the input file.
  out.close();		// close the output file.
  return 0;		// exit the program
}

void ComputeChange (int cents, int& dollars, int& quarters, int& dimes, int& pennies) {
    dollars = cents / 100;  // calculate dollars

    cents = cents % 100;
    quarters = cents / 25; // calculate quarters

    cents = cents % 25;
    dimes = cents / 10;    // calculate dimes
  
    cents = cents % 10;
    pennies = cents;       // calculate pennies
}
