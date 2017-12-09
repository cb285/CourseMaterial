// File: boggle.CPO
// given a block of letters and list of words in input file,
// searches for words then writes results to specified output file
// --------------------------------------------------------------------
// Class: CS215                            Instructor: Dr. Don Roberts
// Assignment: Machine Problem 1           Date assigned: 1/29/16
// Programmer: Clayton Brutus              Date completed: 2/2/16

#include <iostream>
#include <fstream>  // read and write files
#include <string>   // handle strings

using namespace std;

struct Tile {
	char letter;
	bool used;
		    	};
const int SIZE = 6; // size of the letters grid

bool wordSearch (Tile grid[SIZE+2][SIZE+2], string word, int row,
		int col);
void resetGrid (Tile grid[SIZE+2][SIZE+2]);

// Function: main
// Returns: 0 if successful
// reads input file, writes whether or not given words are present
int main (int argc,        // Rec'd: # of arguments
	  char* argv[]) {  // Rec'd: arguments

    if(argc != 3) {
	cerr << "USAGE: " << argv[0] << " inputfile outputfile\n";
	return 1;
    }
    
    ifstream in(argv[1]);
    ofstream out(argv[2]);

    if (!in) {
	cerr << "Could not open " << argv[1] << endl;
	return 2;
    }
    
    if (!out) {
	cerr << "Could not open " << argv[2] << endl;
	return 3;
    }
    // create 2d grid of letters
    Tile grid[SIZE+2][SIZE+2];

    for (int x = 0; x < SIZE+2; x++) {
	grid[0][x].letter = '*';
	grid[x][0].letter = '*';
	grid[SIZE-1][x].letter = '*';
	grid[x][SIZE-1].letter = '*';
    }
    // read letters from input file
    for (int y = 1; y < SIZE+1; y++)
	for (int x = 1; x < SIZE+1; x++)
	    in >> grid[y][x].letter;
    
    const int MAX_WORDS = 100;
    string w;
    string words[MAX_WORDS];
    int num_words = 0;

    // read words from input file
    while (in >> w) {
	words[num_words] = w;
	num_words++;
    }
    
    bool found;
    
    // search for each word, write results to output file
    for (int i = 0; i < num_words; i++) {
	found = false;
	for (int x = 1; x < SIZE+1 && !found; x++)
	    for (int y = 1; y < SIZE+1 && !found; y++) {
		resetGrid (grid);
		if (wordSearch (grid, words[i], y, x)) {
		    out << words[i] << " yes" << endl;
		    found = true;
		    break;
		}
		else if (x == SIZE && y == SIZE)
		    out << words[i] << " no" << endl;
	    }
    }
    // close input and output files
    in.close();
    out.close();
    return 0;
}

// Function: wordSearch
// Returns: 1 or 0 (true or false)
// given an array of letters, a target word, and a starting place, finds the word
bool wordSearch (Tile grid[SIZE+2][SIZE+2], // Rec'd/P'back: grid of letters to search in
		string word,                // Rec'd: word to seach for
		int row,                    // Rec'd: location in grid to search from
		int col) {

  // found word
  if (word.length() == 0)
    return true;

  // check current space
  if (word[0] != grid[row][col].letter)
    return false;
  
  // letter has already been used
  if (grid[row][col].used)
    return false;
  
  grid[row][col].used = true;
  
  // recursive cases:
  // check right
  if (wordSearch (grid, word.substr(1, word.length()), row, col+1))
    return true;
  
  // check left
  if (wordSearch (grid, word.substr(1, word.length()), row, col-1)) 
    return true;
    
  // check down
  if (wordSearch (grid, word.substr(1, word.length()), row+1, col)) 
    return true;
  
  // check up
  if (wordSearch (grid, word.substr(1, word.length()), row-1, col))
    return true;

  // check top right
  if (wordSearch (grid, word.substr(1, word.length()), row-1, col+1))
    return true;
    
  // check top left
  if (wordSearch (grid, word.substr(1, word.length()), row-1, col-1))
    return true;
  
  // check bottom right
  if (wordSearch (grid, word.substr(1, word.length()), row+1, col+1))
    return true;
  
  // check bottom left
  if (wordSearch (grid, word.substr(1, word.length()), row+1, col-1))
    return true;
    
  // didn't find next letter
  grid[row][col].used = false;
  return false;
}

// Function: resetGrid
// sets all letters in 2x2 grid to unused
void resetGrid (Tile grid[SIZE+2][SIZE+2]) { // Rec'd: grid of letters
    for (int x = 0; x < SIZE+2; x++)
	for (int y = 1; y < SIZE+2; y++)
	    grid[y][x].used = false;
}
