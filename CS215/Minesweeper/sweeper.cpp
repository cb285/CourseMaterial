#include <iostream>
#include <cstdlib>
#include <cctype>
#include <stdexcept>
#include "sweepergrid.h"
#include "sweepercell.h"

using namespace std;

int main (int argc, char *argv[]) {
   // check num of arguments
   if (argc != 4) {
      cerr << "USAGE: " << argv[0] << " rows columns density" << endl;
      return 1;
   }
   
   // convert c-string arguments to numbers
   int numRows = atoi (argv[1]);
   int numCols = atoi (argv[2]);
   int density = atoi (argv[3]);

   // check if choices are valid
   if (numRows < 5 || numCols < 5) {
      cerr << "Grid size must be at least 5 x 5." << endl;
      return 2;
   }
      
   if (density > 75 || density < 25) {
      cerr << "Density must be between 25 to 75, inclusively." << endl;
      return 2;
   }
   
   // create sweeper grid
   SweeperGrid grid(numRows, numCols, density);
   
   // display number of bombs
   cout << "There are " << grid.GetBombs() << " bombs in the grid." << endl;

   char command = 'C';
   int row, col;
   
   while (command != 'Q' && !grid.GameWon()) {
      // display grid
      grid.Write(cout);
      cout << endl;
      
      cout << "Enter u to uncover, m to mark, k to unmark, q to quit: ";
      cin >> command;
      
      command = toupper(command);
      
      if (command == 'U') {
	 cout << "Enter a location (row col) to uncover: ";

	 try
	 {
	    cin >> row >> col;
	    
	    if(grid.Uncover(row, col)) {
	       cout << "You've uncovered a mine! Game over!!" << endl;
	       
	       for (int r = 0; r < numRows; r++)
		  for (int c = 0; c < numCols; c++)
		     grid.Uncover(r, c);
	       
	       grid.Write(cout);
	       return 0;
	    }
	 } // end try

	 catch (const out_of_range & re)
	 {
	    cout << re.what() << "  Try again!" << endl;
	 }
	 
      }
      
      else if (command == 'M') {
	 cout << "Enter a location (row col) to mark: ";
	 try {
	    cin >> row >> col;
	    grid.Mark(row, col);
	 }
	 
	 catch (const out_of_range & re) {
	    cout << re.what() << "  Try again!" << endl;
	 }
      }
      
      else if (command == 'K') {
	 cout << "Enter a location (row col) to unmark: ";
	 try {
	    cin >> row >> col;
	    grid.Mark(row, col);
	 }

	 catch (const out_of_range & re)
	 {
	    cout << re.what() << "  Try again!" << endl;
	 }
	 
      }
      
   }
   
   return 0;
}
