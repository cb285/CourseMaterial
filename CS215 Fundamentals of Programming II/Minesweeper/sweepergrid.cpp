// File: sweepergrid.cpp
// implementation for SweeperGrid ADT
// 
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 4           Date assigned: 2/29/16
// Programmer: Clayton Brutus              Date completed: 3/21/16

#include "sweepergrid.h"
#include "sweepercell.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

// Function: SweeperGrid
// Explicit value constructor with default values.
SweeperGrid::SweeperGrid (int initialRows, int initialCols, int density) {  // REC'D: number of rows and columns,
                                                                            // REC'D: density of bombs
   numRows = initialRows;
   numColumns = initialCols;
   numBombs = 0;
   
   grid = new SweeperCell[(numRows+2)*(numColumns+2)];
   
   srand(time(0));

   for (int r = 0; r < numRows; r++)
      for (int c = 0; c < numColumns; c++) {
	 if ((rand() % 100 + 1) < density) {
	    PlaceBomb(r, c);
	 }
      }

   // uncover the frame of unused cells
   // top & bottom
   for (int i = 0; i < numColumns+2; i++) {
      AtReal(0, i).Uncover();
      AtReal(numRows+1, i).Uncover();
   }
      
   
   // sides
   for (int j = 0; j < numRows + 2; j++) {
      AtReal(j, 0).Uncover();
      AtReal(j, numColumns+1).Uncover();
   }
}

// Function: SweeperGrid
// Copy constructor
SweeperGrid::SweeperGrid (const SweeperGrid& source) { // REC'D: SweeperGrid object
   numRows = source.numRows;
   numColumns = source.numColumns;
   numBombs = source.numBombs;
   
   grid = new SweeperCell[(numRows+2)*(numColumns+2)];
   
   // uncover the frame of unused cells
   // top & bottom
   for (int i = 0; i < numColumns+2; i++) {
      AtReal(0, i).Uncover();
      AtReal(numRows+1, i).Uncover();
   }
   
   // sides
   for (int j = 0; j < numRows + 2; j++) {
      AtReal(j, 0).Uncover();
      AtReal(j, numColumns+1).Uncover();
   }
   
   for (int r = 0; r < numRows; r++)
      for (int c = 0; c < numColumns; c++) {
	 if (source.At(r, c).HasBomb())
	    PlaceBomb(r, c);
	 
	 if (!source.At(r, c).IsCovered())
	    Uncover(r, c);
	 
	 if (source.At(r, c).IsMarked())
	    Mark(r, c);
      }
}

// Function: ~SweeperGrid
// Destructor
SweeperGrid::~SweeperGrid () {
   std::cout << "deleting!\n";
   delete [] grid;
}

// Function: operator=
// overloaded = operator
SweeperGrid&
SweeperGrid::operator= (const SweeperGrid& source) { // REC'D: SweeperGrid object
   // check for self assignment
   if (this != &source) {
      // delete old data
      delete [] grid;

      // allocate new array
      grid = new SweeperCell[(numRows+2)*(numColumns+2)];

      // set other values
      numRows = source.numRows;
      numColumns = source.numColumns;
      numBombs = source.numBombs;
      
      for (int r = 0; r < numRows; r++)
	 for (int c = 0; c < numColumns; c++) {
	    if (source.At(r, c).HasBomb())
	       PlaceBomb(r, c);
	    
	    if (!source.At(r, c).IsCovered())
	       Uncover(r, c);
	    
	    if (source.At(r, c).IsMarked())
	       Mark(r, c);
	 }
      
      // uncover the frame of unused cells
      // top & bottom
      for (int i = 0; i < numColumns+2; i++) {
	 AtReal(0, i).Uncover();
	 AtReal(numRows+1, i).Uncover();
      }
      
      // sides
      for (int j = 0; j < numRows + 2; j++) {
	 AtReal(j, 0).Uncover();
	 AtReal(j, numColumns+1).Uncover();
      }
      
   }
   return *this;
}

// Function: At
// returns a reference to a cell in the grid
SweeperCell&
SweeperGrid::At (int row, int col) const { // REC'D: location of cell
   return grid[(row+1)*(numColumns+2) + col+1];
}

// Function: AtReal
// returns a reference to a cell in the grid at the "real" location (with the outer frame considered)
SweeperCell&
SweeperGrid::AtReal (int row, int col) const { // REC'D: location of cell
   return grid[(row)*(numColumns+2) + col];
}

// Function: GetRows
// Returns: number of rows in grid
int
SweeperGrid::GetRows () const {
   return numRows;
}

// Function: GetColumns
// Returns: number of columns in grid
int
SweeperGrid::GetColumns () const {
   return numColumns;
}

// Function: GetColumns
// Returns: number of bombs in grid
int
SweeperGrid::GetBombs () const {
   return numBombs;
}

// Function: GameWon
// Returns: true if game has been won
bool
SweeperGrid::GameWon () const {
   for (int r = 1; r < numRows + 1; r++)
      for (int c = 1; c < numColumns + 1; c++)
	 if (grid[r*numColumns + c].IsMarked() || grid[r*numColumns + c].IsCovered())
	    return false;

   return true;
}

// Function: PlaceBomb
// Places bomb at specified location
void
SweeperGrid::PlaceBomb (int row, int col) { // REC'D: location to place bomb
   // check if position is valid
   if (row < 0 || row >= numRows   ||
       col < 0 || col >= numColumns)
      throw(std::out_of_range("Invalid position for bomb placement."));

   std::cout << "placing at: " << row << " " << col << std::endl;

   // check if already has a bomb
   if (!At(row, col).HasBomb()) {
      // place the bomb
      At(row, col).PlaceBomb();
      numBombs++;
      
      // recalculate adjacent cell's num of bombs
      At(row  , col+1).IncrementNumAdjacent(); // right
      At(row  , col-1).IncrementNumAdjacent(); // left
      At(row+1, col  ).IncrementNumAdjacent(); // down
      At(row-1, col  ).IncrementNumAdjacent(); // up
      At(row+1, col+1).IncrementNumAdjacent(); // bottom right
      At(row+1, col-1).IncrementNumAdjacent(); // bottom left
      At(row-1, col+1).IncrementNumAdjacent(); // top right
      At(row-1, col-1).IncrementNumAdjacent(); // top left
   }
}

// Function: RemoveBomb
// Removes bomb at specified location
void
SweeperGrid::RemoveBomb (int row, int col) { // REC'D: location of bomb
   // check if position is valid
   if (row < 0 || row >= numRows ||
       col < 0 || col >= numColumns)
      throw(std::out_of_range("Invalid position for bomb removal."));

   if (At(row, col).HasBomb()) {
      // remove the bomb
      At(row, col).RemoveBomb();
      numBombs--;
      
      // recalculate adjacent cell's num of bombs
      At(row, col+1).DecrementNumAdjacent(); // right
      At(row, col-1).DecrementNumAdjacent(); // left
      At(row+1, col).DecrementNumAdjacent(); // down
      At(row-1, col).DecrementNumAdjacent(); // up
      At(row+1, col + 1).DecrementNumAdjacent(); // bottom right
      At(row+1, col - 1).DecrementNumAdjacent(); // bottom left
      At(row-1, col + 1).DecrementNumAdjacent(); // top right
      At(row-1, col - 1).DecrementNumAdjacent(); // top left
   }
}

// Function: Uncover
// uncovers cell at location
// Returns: true if a bomb is uncovered
bool
SweeperGrid::Uncover (int row, int col) { // REC'D: location of cell
      // check if position is valid
      if (row < 0 || row >= numRows ||
	  col < 0 || col >= numColumns)
	 throw(std::out_of_range("Invalid position for uncovering cell."));
      
      return At(row, col).Uncover();
   }

// Function: Mark
// marks cell at location
void
SweeperGrid::Mark (int row, int col) { // REC'D: location of cell
   // check if position is valid
   if (row < 0 || row >= numRows ||
       col < 0 || col >= numColumns)
      throw(std::out_of_range("Invalid position for marking cell."));
   
   At(row, col).Mark();
}

// Function: Unmark
// unmarks cell at location
void
SweeperGrid::Unmark (int row, int col) { // REC'D: location of cell
   // check if position is valid
   if (row < 0 || row >= numRows ||
       col < 0 || col >= numColumns)
      throw(std::out_of_range("Invalid position for unmarking cell."));

   At(row, col).Unmark();
}

// Function: Write
// outputs grid to output stream
void
SweeperGrid::Write (std::ostream& out) const { // REC'D: output stream
   for (int r = 0; r < numRows; r++) {
      for(int c = 0; c < numColumns; c++) {
	 out << "  " << At(r, c);
      }
      out << std::endl;
   }
}
