// File: sweepergrid.h
// header file for SweeperGrid ADT
// 
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 4           Date assigned: 2/29/16
// Programmer: Clayton Brutus              Date completed: 3/21/16

#ifndef SWEEPER_H_
#define SWEEPER_H_

#include "sweepercell.h"

class SweeperGrid {
  public:
   // explicit value constructor with default values
   SweeperGrid (int initialRows = 5, int initialCols = 5, int density = 25);

   // copy constructor
   SweeperGrid (const SweeperGrid& grid);

   // destructor
   ~SweeperGrid ();

   SweeperGrid& operator= (const SweeperGrid& source);

   SweeperCell& At (int row, int col) const;
   SweeperCell& AtReal (int row, int col) const;

   int GetRows () const;
   int GetColumns () const;
   int GetBombs () const;

   bool GameWon () const;

   void PlaceBomb (int row, int col);
   void RemoveBomb (int row, int col);

   bool Uncover (int row, int col);
   void Mark (int row, int col);
   void Unmark (int row, int col);

   void Write (std::ostream& out) const;
   
  private:
   int numRows;
   int numColumns;
   int numBombs;
   SweeperCell* grid; // pointer to grid
};

#endif // SWEEPER_H_
