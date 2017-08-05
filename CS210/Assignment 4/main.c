/* File: main.c
 * Plays the game of Nim.
 *
 * --------------------------------------------------------------
 * Class: CS 210 Instructor: Dr. Deborah Hwang
 * Assignment: Programming Assignment 4 Date assigned:  2/19/15
 * Programmer: Clayton Brutus           Date completed: 2/23/15
 */

#include <stdio.h>      /* printf, scanf */
#include <stdlib.h>
#include <ctype.h>      /* tolower */

void print_greeting();
int user_wishes_to_continue();
void print_scoreboard(int comp_wins, int user_wins);
int play_nim(int starting_player);
void get_comp_move(int heap1, int heap2, int heap3, char *chosen_heap, int *num_to_remove);

int main()
{
    /* initialize variables */
    int comp_wins = 0;
    int user_wins = 0;
    int starting_player = 1;
    int winner;

    /* print greeting at beginning of game */
    print_greeting();

    /* play until user wants to stop */
    do
    {
        winner = play_nim(starting_player);
        if (winner == 1)
        {
            /* if human wins. add one to user_wins */
            user_wins++;
            /* make player 2 start the next game */
            starting_player = 2;
        }
        else
        {   /* if computer wins, add one to comp_wins */
            comp_wins++;
            /* make player 1 start the next game */
            starting_player = 1;
        }


        /* print scoreboard at end of each game */
        print_scoreboard(comp_wins, user_wins);

    } /* ask user if they want to keep playing */
    while(user_wishes_to_continue());

    return 0;
}

/*  Function: print_greeting
    Prints the greeting at the beginning of the game.
 */
void print_greeting()
{
    printf("Welcome to the Ancient Game of Nim\n");
    printf("Player 1 is you (the human)\n");
    printf("Player 2 is me (the computer)\n");
}

/*  Function: user_wishes_to_continue
    Asks the user if they want to keep playing Nim.
 */
int user_wishes_to_continue()
{
    char response;
    do
    {
        printf("\nDo you wish to play again? (y/n) ");
        scanf(" %c", &response);
        response = tolower(response);
        if (response != 'y' && response != 'n')
            printf("Bad input! Try again.\n");
    }
    while(response != 'y' && response != 'n');

    return(response == 'y');
}

/*  Function: print_scoreboard
    prints the scoreboard at the end of each game.
 */
void print_scoreboard(int comp_wins, /* Rec'd: times the computer won.  */
                      int user_wins) /* Rec'd: times the user won.      */
{
    printf("* * * * * * * * * * *\n");
    printf("Current Score:\n");
    printf("Player 1 (Human):    %d\n", user_wins);
    printf("Player 2 (Computer): %d\n", comp_wins);
}

/*  Function: play_nim
    plays the game of nim.
 */
int play_nim(int starting_player) /* Rec'd: player number that is starting */
{
    /* initialize variables */
    int current_player;
    int next_player;

    int A_stones = 5;
    int B_stones = 4;
    int C_stones = 3;

    char chosen_heap;
    int num_to_remove;

    int valid_response = 0;

    printf("Player %d goes first this time!\n", starting_player);
    next_player = starting_player;

    /* play until there is no stones left */
    do
    {
        current_player = next_player;
        printf("\nPlayer %d\n", current_player);
        printf("Heaps:\n");
        printf("A: %d\n", A_stones);
        printf("B: %d\n", B_stones);
        printf("C: %d\n", C_stones);

        if (current_player == 1)
        {
            do
            {
                /* ask user for their move */
                printf("Enter the letter of the heap and number of stones to remove: ");
                scanf(" %c%d", &chosen_heap, &num_to_remove);
                /* change response to lower case */
                chosen_heap = tolower(chosen_heap);

                /* check if user's response is valid */
                if (chosen_heap == 'a' && num_to_remove <= A_stones)
                    valid_response = 1;
                else if (chosen_heap == 'b' && num_to_remove <= B_stones)
                    valid_response = 1;
                else if (chosen_heap == 'c' && num_to_remove <= C_stones)
                    valid_response = 1;
                else
                {
                    valid_response = 0;
                    printf("Illegal move! Try again.\n");
                    printf("\nPlayer %d\n", current_player);
                    printf("Heaps:\n");
                    printf("A: %d\n", A_stones);
                    printf("B: %d\n", B_stones);
                    printf("C: %d\n", C_stones);

                }
            }
            while(!valid_response);
            next_player = 2;
        }
        else
        {
            /* get computer's move */
            get_comp_move(A_stones, B_stones, C_stones, &chosen_heap, &num_to_remove);
            next_player = 1;
        }

        /* remove the chosen amount of stones */
        if (chosen_heap == 'a')
            A_stones -= num_to_remove;
        else if (chosen_heap == 'b')
            B_stones -= num_to_remove;
        else
            C_stones -= num_to_remove;

    }
    while(A_stones != 0 || B_stones != 0 || C_stones != 0);

    /* print the winner */
    printf("Player %d wins!\n\n", current_player);

    /* return the winner */
    return current_player;
}

/*  Function: get_comp_move
    Get's computers move using Nim number.
 */
void get_comp_move(int heap1, int heap2, int heap3, /* Rec'd: number of stones in each heap */
                   char *chosen_heap,               /* P'back: chosen heap */
                   int *num_to_remove)              /* P'back: number to remove from chosen heap */
{
    /* calculate Nim number */
    int nim_number;
    nim_number = heap1 ^ heap2 ^ heap3;

    /* if nim num is 0, just take 1 from first stack available */
    if (nim_number == 0)
    {
        if (heap1 >= 1)
        {
            *chosen_heap = 'a';
            *num_to_remove = 1;
        }
        else if (heap2 >= 1)
        {
            *chosen_heap = 'b';
            *num_to_remove = 1;
        }
        else
        {
            *chosen_heap = 'c';
            *num_to_remove = 1;
        }
    }

    /* if Nim num is not 0, choose a move that makes it 0 */
    else
    {
        if ((heap1 ^ nim_number) < heap1)
        {
            *chosen_heap = 'a';
            *num_to_remove = heap1 - (heap1 ^ nim_number);
        }
        else if ((heap2 ^ nim_number) < heap2)
        {
            *chosen_heap = 'b';
            *num_to_remove = heap2 - (heap2 ^ nim_number);
        }
        else if ((heap3 ^ nim_number) < heap3)
        {
            *chosen_heap = 'c';
            *num_to_remove = heap3 - (heap3 ^ nim_number);
        }
    }
    printf("COMPUTER moves %c%d\n", *chosen_heap, *num_to_remove);
}
