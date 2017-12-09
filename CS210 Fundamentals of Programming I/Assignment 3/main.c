/* File: main.c
 * Reverse Guessing Game:
 * User picks a number, and computer tries to guess it.
 * --------------------------------------------------------------------
 * Class: CS 210                         Instructor: Dr. Deborah Hwang
 * Assignment: Programming Assignment 3  Date assigned: 2/5/15
 * Programmer: Clayton Brutus            Date completed: 2/12/15
 */

#include <stdio.h>  /* printf and scanf */
#include <stdlib.h>
#include <ctype.h>  /* tolower */

#define MIN 1       /* MIN is lowest possible number */
#define MAX 1000    /* MAX is highest possible number */
#define GUESSES 5   /* number of guesses computer has */

void print_greeting(int min, int max, int guesses);
int user_wishes_to_continue();
char promp_user(int guess);
void print_results(int comp_wins, int user_wins);
int play_game(int min, int max, int guesses);

/* Function: main
    prints greeting, plays game until user wants to stop, then prints results.
*/
int main()
{
    /* 1. print greeting */
    print_greeting(MIN, MAX, GUESSES);

    /* 2. initialize variables */
    int comp_wins = 0;  /* # times computer won */
    int user_wins = 0;  /* # times user won */
    int result;         /* 1 if computer won, 0 if user won */

    /* 3. play game until user wants to stop */
    do
    {
        result = play_game(MIN, MAX, GUESSES);

        /* 3.1. keep track of # of wins */
        if (result == 1)
            comp_wins++;
        else
            user_wins++;

      /* repeats until user wants to quit */
    } while(user_wishes_to_continue());

    /* 4. display results */
    print_results(comp_wins, user_wins);

    /* 5. return 0 to exit program */
    return 0;
}

/* Function: print_greeting
    displays the greeting message.
*/
void print_greeting(int min,        /* rec'd: lowest possible number */
                    int max,        /* rec'd: highest possible number */
                    int guesses)    /* rec'd: number of guesses computer gets */
{
    printf("Welcome to the (reverse) guessing game.\n");
    printf("Choose a number between %d and %d inclusive\n", min, max);
    printf("and I will try to guess it in %d or fewer tries\n\n", guesses);
}

/* Function: user_wishes_to_continue
    asks user if they want to keep playing
*/
int user_wishes_to_continue()
{
   char response; /* answer to question about continuing */

   do
    {
        /*ask user until they enter a valid answer */
        printf("\nDo you wish to continue (y/n)? ");
        scanf(" %c", &response);
        response = tolower(response);
        if (response != 'y' && response != 'n')
            printf("Bad Input. Try again.\n");
    }
    while(response != 'y' && response != 'n');

/* return true (1) if the user enters y, false (0) if the user enters n */
    return response == 'y';
}

/* Function: promp_user
    asks user if the computer's guess is too high, too low, or equal
*/
char promp_user(int guess)  /* rec'd: computer's guess */
{
    char response;

    printf("My guess is %d.\n", guess);

    do
    {
        /* ask user until they enter a valid answer */
        printf("Is my guess too (H)igh, too (L)ow, or (E)qual to your number? ");
        scanf(" %c", &response);
        response = tolower(response);
        if (response != 'h' && response != 'l' && response != 'e')
            printf("Bad Input. Try again.\n");
    } while(response != 'h' && response != 'l' && response != 'e');

    /* return the answer in lower case */
    return response;
}

/* Function: print_results
    displays results at end of game.
*/
void print_results(int comp_wins,   /* rec'd: how many times the computer won */
                   int user_wins)   /* rec'd: how many times the user won */
{
    /* display results */
    printf("I won %d game(s).\n", comp_wins);
    printf("You won %d game(s).\n", user_wins);
    printf("Thank you for playing!\n");
}

/* Function: play_game
    plays the reverse guessing game
*/
int play_game(int min,          /* rec'd: lowest possible number */
              int max,          /* rec'd: highest possible number */
              int max_guesses)  /* rec'd: number of guesses computer gets */
{
    /* initialize variables */
    int high = max;
    int low = min;
    int num_guesses = 0;        /* # of guesses computer has used */
    int has_won = 0;            /* becomes 1 if computer wins */

    int guess;                  /* computer's guess */
    char response;              /* user's response (too high, too low, or equal) */

    /* play game until user or computer wins */
    do
    {
        /* compute guess as average of high and low */
        guess = (high + low) / 2;

        /* ask user if the guess is too high, too low, or equal */
        response = promp_user(guess);

        /* if guess is too high */
        if (response == 'h')
            high = guess - 1;

        /* if guess is too low */
        else if (response == 'l')
            low = guess + 1;
        /* if guess is equal */
        else
            has_won = 1;

        /* increment num_guesses by 1 */
        num_guesses++;
    } while (has_won != 1 && num_guesses != 5);

    /* if computer wins: display how many guesses it took */
    if (has_won == 1)
        printf("I win! It took me %d guess(es).\n", num_guesses);

    /* if user wins: display "You win!" */
    else
        printf("You win!\n");

    /* returns 1 if computer wins, returns 0 if user wins */
    return has_won;
}
