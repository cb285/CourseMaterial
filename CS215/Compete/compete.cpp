// File: concordance.cpp
// plays serveral games of compete card game
// --------------------------------------------------------------------
// Class: CS 215                           Instructor: Dr. Don Roberts
// Assignment: Machine Problem 6           Date assigned: 4/1/16
// Programmer: Clayton Brutus              Date completed: 4/8/16

#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

template <typename T>
void AppendQueue (queue<T> &dest, queue<T> &source);

int main (int argc, char* argv[]) { // REC'D: number of args, strings of args
   if (argc != 2) {
      cerr << "USAGE: " << argv[0] << " inputfile" << endl;
      return 1;
   }

   ifstream in(argv[1]);

   if (!in) {
      cerr << "invalid input file" << endl;
      return 2;
   }

   int numGames;
   int numCards;
   char card;
   int result; // 0 = tie, 1 = player1 won, 2 = player2 won, other = not finished
   int roundNumber;
   int gameNumber = 0;
   in >> numGames;

   queue<int> player1;
   queue<int> player2;
   queue<int> discard;

   // play until specified number of games have been played
   while (gameNumber < numGames) {

      // clear each player's pile
      while (!player1.empty())
	 player1.pop();
      while (!player2.empty())
	 player2.pop();
      
      in >> numCards;

      // add cards to each player 1's pile
      for (int i = 0; i < numCards; i++) {
	 in >> card;
	 // for cards with values 2 to 9, '2' = 2, '3' = 3, ...
	 if (card <= '9')
	    player1.push(card - '0');
	 
	 // otherwise, T = 10, J = 11, Q = 12, K = 13, A = 14
	 else if (card == 'T')
	    player1.push(10);
	 else if (card == 'J')
	    player1.push(11);
	 else if (card == 'Q')
	    player1.push(12);
	 else if (card == 'K')
	    player1.push(13);
	 else
	    player1.push(14);
      }

      // repeat for player 2
      for (int j = 0; j < numCards; j++) {
	 in >> card;

	 if (card <= '9')
	    player2.push(card - '0');
	 
	 else if (card == 'T')
	    player2.push(10);
	 else if (card == 'J')
	    player2.push(11);
	 else if (card == 'Q')
	    player2.push(12);
	 else if (card == 'K')
	    player2.push(13);
	 else
	    player2.push(14);
      }

      // reset round number
      roundNumber = 1;

      // reset result
      result = 3;
      
      // increment game number
      gameNumber++;

      // clear the discard pile
      while (!discard.empty())
	 discard.pop();
      
      // play until someone runs out of cards
      while (true) {
	 
	 // if players have same card then keep turning over cards
	 // until they don't have the same card, or a player is out of cards
	 while (player1.front() == player2.front() && !player1.empty()) {
            // place both cards in discard
	    discard.push(player1.front());
	    player1.pop();
	    discard.push(player2.front());
	    player2.pop();
	 }
	 	 
	 // player1's card is better
	 if (player1.front() > player2.front() && !player2.empty()) {
	    // discard winner's card
	    discard.push(player1.front());
	    player1.pop();
	    
	    // discard loser's card
	    discard.push(player2.front());
	    player2.pop();
	    
	    // add discard to winner's
	    AppendQueue (player1, discard);
	 }
	 
	 // player2's card is better
	 else if (player1.front() < player2.front() && !player1.empty()) {
	    // discard winner's card
	    discard.push(player2.front());
	    player2.pop();
	    
	    // discard loser's card
	    discard.push(player1.front());
	    player1.pop();
	    
	    // add discard to winner's pile
	    AppendQueue (player2, discard);
	 }
	 
	 // only player 1 is out of cards
	 if  (player1.empty() && !player2.empty()) {
	    AppendQueue (player1, discard);
	    result = 2;
	    break; // game is over
	 }
	 
	 // only player 2 is out of cards
	 else if (!player1.empty() && player2.empty()) {
	    AppendQueue (player2, discard);
	    result = 1;
	    break; // game is over
	 }
	 
	 // both are out of cards
	 else if (player1.empty() && player2.empty()) {
	    result = 0;
	    break; // game is over
	 }

	 // if game isn't over, go to next round
	 roundNumber++;
	 
      } // end of round loop

      // end of game
      if (result == 0) {
	 cout << "Game " << gameNumber << ": After " << roundNumber
	      << " round(s), the game ends in a tie" << endl;
      }

      else
	    cout << "Game " << gameNumber << ": After " << roundNumber
	      << " round(s), Player " << result << " wins" << endl;
      
   } // end of game loop

   return 0;
}


// Function: AppendQueue
// adds all items of one queue to another
template <typename T>
void AppendQueue (queue<T> &dest, queue<T> &source) { // REC'D/P'BACK: destination queue, source queue
   // move items until source is empty
   while(!source.empty()) {
      dest.push(source.front());
      source.pop();
   }
}
