// File: sweepercell.cpp
// Implementation of minesweeper cells
// -----------------------------------------------------------
// Assignment: Project 4
// Date Assigned: February 29, 2016

#include <iostream>
#include "sweepercell.h"
using namespace std;

// Constructor
// Cell always starts out empty, covered, unmarked, and with 0
// adjacent bombs
SweeperCell::SweeperCell ()
{
   bomb = false;
   covered = true;
   marked = false;
   rep = '#';
   numAdjacent = 0;
}  // end default constructor

// Function: HasBomb
// Return true if cell contains a bomb
bool SweeperCell::HasBomb () const
{
   return bomb;
}  // end HasBomb

// Function: IsCovered
// Return true if cell is covered
bool SweeperCell::IsCovered () const
{
   return covered;
}  // end IsCovered

// Function: IsMarked
// Return true if cell is marked
bool SweeperCell::IsMarked () const
{
   return marked;
}  // end IsMarked

// Function: GetNumAdjacent
// Returns number of adjacent cells containing bombs
int SweeperCell::GetNumAdjacent() const
{
   return numAdjacent;
}  // end GetNumAdjacent

// Function: IncrementNumAdjacent
// Increases number of adjacent cells containing bombs by 1
void SweeperCell::IncrementNumAdjacent()
{
   numAdjacent++;
   if (!bomb && !covered) // no bomb and not covered, so show numAdjacent
      rep = '0' + numAdjacent;
}  // end IncrementNumAdjacent

// Function: DecrementNumAdjacent
// Decreases number of adjacent cells containing bombs by 1
void SweeperCell::DecrementNumAdjacent()
{
   numAdjacent--;
   if (!bomb && !covered)  // no bomb and not covered, so show numAdjacent
      rep = '0' + numAdjacent;
}  // end DecrementNumAdjacent

// Function: PlaceBomb
// Places a bomb in the cell if covered
void SweeperCell::PlaceBomb ()
{
   bomb = true;
   if (!covered)  // not covered, so show bomb
      rep = '*';
}  // end PlaceBomb

// Function: RemoveBomb
// Removes a bomb from the cell if covered
void SweeperCell::RemoveBomb ()
{
   bomb = false;
   if (!covered)  // not covered, so show numAdjacent
      rep = '0' + numAdjacent;
}  // end RemoveBomb

// Function: Uncover
// Uncovers the cell, returns true if uncovers a bomb
bool SweeperCell::Uncover ()
{
   covered = false;
   if (bomb)  // has a bomb, so show it
      rep = '*';
   else       // no bomb, so show numAdjacent
      rep = '0' + numAdjacent;
   return (bomb);
}  // end Uncover

// Function: Mark
// Marks the cell
void SweeperCell::Mark ()
{
   marked = true;
   if (covered)  // covered, so show the mark
      rep = 'f';
}  // end Mark

// Function: Unmark
// Unmarks the cell
void SweeperCell::Unmark ()
{
   marked = false;
   if (covered)  // covered, so remove the mark
      rep = '#';
}  // end Unmark

// Function: operator<<
// Outputs the character representation of the cell:
//   # - when covered and unmarked
//   f - when covered and marked
//   * - when uncovered containing a bomb
//   n - when uncovered and no bomb, where n is 
//       number of adjacent cells (all 8 directions) 
//       containing bombs
ostream& operator<< (ostream & out, const SweeperCell & cell)
{
   out << cell.rep;
   return out;
}  // end operator<<
