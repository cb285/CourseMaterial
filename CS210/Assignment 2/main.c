/* File: main.c
 * This program allows 2 players to play rock, paper, scissors.
 * Uses stdio.h, stdlib.h, and ctype.h library files.
 * functions are main, print_greeting, get_pick, print_picks, and compute_winner.
 * --------------------------------------------------------------
 * Class: CS 210                   Instructor: Dr. Deborah Hwang
 * Assignment: Assignment 2        Date assigned:  January 29, 2015
 * Programmer: Clayton Brutus      Date completed: February 1, 2015
 */

#include <stdio.h>  /* printf, scanf */
#include <stdlib.h>
#include <ctype.h>  /* tolower */

void print_greeting();
char get_pick(int player_num);
void print_picks(char player1, char player2);
int compute_winner(char player1, char player2);

/* Function: main
   prints greeting, asks players for choices, prints their picks, and displays the winner or tie.
 */
int main()
{
    char player1, player2;  /* player's picks */
    int winner;             /* indicates who won */

    /* 1. Print the greeting using the print_greeting function */
    print_greeting();
    /* 2. Get first user's pick using get_pick function. */
    player1 = get_pick(1);
    /* 3. Get second user's pick using get_pick function */
    player2 = get_pick(2);

    print_picks(player1, player2);

    /* 4. Determine the winner using compute_winner function */
    winner = compute_winner(player1, player2);

    switch (winner)
    {
    /* 5. If winner value is -1, print “Player 1 wins!” */
        case -1:
            printf("Player 1 wins!\n");
            break;
    /* 6. If winner value is 1, print “Player 2 wins!” */
        case 1:
            printf("Player 2 wins!\n");
            break;
    /* 7. Otherwise, print “Tie!” */
        default:
            printf("Tie!\n");
    }
    /* 8. Return 0 to exit the program */
    return 0;
}

/* Function: print_greeting
   prints "Welcome to the RPS Game!" and "Let's play!!"
 */
void print_greeting() /* no arguments and nothing returned. */
{
    printf("Welcome to the RPS Game!\n");
    printf("Let's play!!\n");
}

/* Function: get_pick
   asks player to pick rock, paper, scissors, converts their choice to lower case.
 */

char get_pick(int player_num) /* Received: 1 or 2 for each player. */
{
    char ch;                  /* Passed back: player's choice in lower case */

/* 1. Ask the user by player number for R, P, or S */
    printf("Player %d, Pick (R)ock, (P)aper, or (S)cissors: ", player_num);
    scanf(" %c", &ch);

/* 2. Convert the input to lowercase */
    ch = tolower(ch);

/* 3. Return the input */
    return ch;
}

/* Function: print_picks
   prints what players pick. */

void print_picks(char player1, /* Received: player1's pick */
                 char player2) /* Received: player2's pick */
{
    switch (player1)
    {
        case ('r'):
        printf("\nPlayer 1 chose Rock\n");
            break;
        case 'p':
        printf("\nPlayer 1 chose Paper\n");
            break;
        case 's':
        printf("\nPlayer 1 chose Scissors\n");
            break;
        default:
        printf("\nPlayer 1 chose Unknown\n");
    }

        switch (player2)
    {
        case 'r':
            printf("Player 2 chose Rock\n");
            break;
        case 'p':
            printf("Player 2 chose Paper\n");
            break;
        case 's':
            printf("Player 2 chose Scissors\n");
            break;
        default:
            printf("Player 2 chose Unknown\n");
    }
}

/* Function: compute_winner
   given player's picks, decides which player wins. */

int compute_winner(char player1, /* Received: player1's pick */
                   char player2) /* Received: player2's pick */
{

 switch (player1)
 {
    /* if player 1 picks rock */
    case 'r':
        if (player2 == 'r')
            return 0;
        else if (player2 == 'p')
            return 1;
        else if (player2 == 's')
            return -1;
        else
            return -1;
        break;
    /* if player 1 picks paper */
    case 'p':
        if (player2 == 'r')
            return -1;
        else if (player2 == 'p')
            return 0;
        else if (player2 == 's')
            return 1;
        else
            return -1;
        break;
    /* if player 1 picks scissors */
    case 's':
        if (player2 == 'r')
            return 1;
        else if (player2 == 'p')
            return -1;
        else if (player2 == 's')
            return 0;
        else
            return -1;
        break;
    /* if player 1's pick is invalid */
    default:
        /* if player 2's pick is valid and player 1's is not, then player 2 wins. */
        if (player2 == 'r' || player2 == 'p' || player2 == 's')
            return 1;
        else
            /* if both picks are invalid, then it is a tie. */
            return 0;


 }


    /* check if player 1 wins: */
    if (player1 == 'r' && player2 == 's')
        return -1;
    else if (player1 == 'p' && player2 == 'r')
        return -1;
    else if (player1 == 's' && player2 == 'p')
        return -1;

    /* check if player 2 wins */
    else if (player2 == 'r' && player1 == 's')
        return 1;
    else if (player2 == 'p' && player1 == 'r')
        return 1;
    else if (player2 == 's' && player1 == 'p')
        return 1;


    /* if both players choose the same thing or invalid picks, then it is a tie */
    else
        return 0;

}
