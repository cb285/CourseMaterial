/* File: main.c
 * plays a game of blackjack using the card.h and card.c library files.
 *
 * --------------------------------------------------------------
 * Class: CS 210                        Instructor: Dr. Deborah Hwang
 * Assignment: Programming Assignment 8 Date assigned:  4/16/15/home/clayton/School/CS 210/Assignment 8/card.h
 * Programmer: Clayton Brutus           Date completed: 4/26/15
 */

#include <stdio.h>  /* printf, scanf */
#include <stdlib.h>
#include <ctype.h>  /* toupper */
#include "cards.h" /* card functions */

int main()
{
    int bet, stake;
    int num_decks;
    shoe_t shoe;
    hand_t dealer_hand;
    hand_t user_hand;
    char response;

    /* print greeting */
    printf("Welcome to Blackjack!\n\n");

    /* initialize the stake */
    stake = 1000;

    num_decks = 1;

    do
    {
        /* ask for number of decks to be used and validate response */
        if (num_decks <= 0)
            printf("Invalid number. Enter a positive integer: ");

        else
            printf("How many decks do you wish to play with (1-10)? ");

        scanf("%d", &num_decks);
    }
    while (num_decks <= 0);

    /* create shoe */
    shoe = create_shoe(num_decks);

    /* empty the dealer's and user's hands */
    dealer_hand = empty_hand();
    user_hand = empty_hand();

    /* initialize bet to 1 */
    bet = 1;

    /* play while the player has money left */
    while (stake > 0)
    {
        /* print remaining stake and number of cards left in the shoe */
        printf("Your stake: $%d\n", stake);
        printf("Cards left in shoe: %d\n\n", cards_left(shoe));

        do
        {
            /* ask user for bet and validate response */
            if (bet > stake || bet == 0)
                printf("Invalid bet. Enter a positive integer less than your stake (negative to quit): ");

            /* ask user for bet and validate response */
            else
                printf("Enter your bet (negative to quit): ");

            scanf("%d", &bet);

            /* if the bet is negative, then end the program */
            if (bet < 0)
            {
                printf("You ended the night with $%d.\n", stake);
                free_shoe(&shoe);
                exit(1);
            }

        }while (bet > stake || bet == 0);

        /* draw the dealer's and player's hands */
        add_card_to_hand(&dealer_hand, draw_card(&shoe));
        add_card_to_hand(&user_hand, draw_card(&shoe));
        add_card_to_hand(&dealer_hand, draw_card(&shoe));
        add_card_to_hand(&user_hand, draw_card(&shoe));

        /* display only the first card in the dealer's hand */
        printf("The dealer is showing ");
        print_card(dealer_hand.cards[0]);
        printf("\n\n");


        response = 'H';

        /* play this hand while user has not busted or stood */
        while (blackjack_points(user_hand) < 21 && response == 'H')
        {

            /* display the entire player's hand */
            printf("Your cards:\n");
            print_hand(user_hand);
            printf("Score: %d\n", blackjack_points(user_hand));

            /* ask player to hit or stand and validate response */
            do
            {
                if (response != 'H' && response != 'S')
                    printf("Invalid response. Enter H or S: ");
                else
                    printf("Do you want to (H)it or (S)tand? ");

                scanf(" %c", &response);
                response = toupper(response);
            }
            while (response != 'H' && response != 'S');

                /* if player hits, then draw a card */
                if (response == 'H')
                    add_card_to_hand(&user_hand, draw_card(&shoe));
        }

        /* if the player stands, then dealer plays */
        if (response == 'S')
        {
            /* draw cards until score is at least 17 */
            while (blackjack_points(dealer_hand) < 17)
                add_card_to_hand(&dealer_hand, draw_card(&shoe));

            /* if the dealer busts, then the player wins */
            if (blackjack_points(dealer_hand) > 21)
            {
                printf("Dealer cards:\n");
                print_hand(dealer_hand);
                printf("Score: %d\n", blackjack_points(dealer_hand));
                printf("Dealer busts! You win $%d!\n", bet);
                stake +=bet;
                bet = 1;
            }
        }

        /* if the player busts, then they loose */
        else if (blackjack_points(user_hand) > 21)
        {
            printf("Your cards:\n");
            print_hand(user_hand);
            printf("Score: %d\n", blackjack_points(user_hand));
            printf("You busted. You lose $%d!\n", bet);
            stake -= bet;
            bet = 1;
        }

        /* if neither bust and the dealer has a better score, then the dealer wins */
        if (blackjack_points(dealer_hand) > blackjack_points(user_hand) && blackjack_points(dealer_hand) <= 21)
        {
            printf("Dealer cards:\n");
            print_hand(dealer_hand);
            printf("Dealer wins with score of %d. You lost!\n", blackjack_points(dealer_hand));
            stake -= bet;
        }

        /* if neither bust and the player has a better score, then the player wins */
        else if (blackjack_points(user_hand) > blackjack_points(dealer_hand) && blackjack_points(user_hand) <= 21)
        {
            printf("Your cards:\n");
            print_hand(user_hand);
            printf("Dealer cards:\n");
            print_hand(dealer_hand);
            printf("You win with a score of %d\n", blackjack_points(user_hand));

            /* add the bet to player's stake */
            stake += bet;
            bet = 1;
        }

        /* if neither bust and their scores are equal, then it is a tie */
        else if (blackjack_points(user_hand) == blackjack_points(dealer_hand))
        {
            printf("Dealer cards:\n");
            print_hand(dealer_hand);
            printf("Score: %d\n", blackjack_points(dealer_hand));
            printf("Tie!\n");
            bet = 1;
        }

        /* empty player and dealer's hands */
        dealer_hand = empty_hand();
        user_hand = empty_hand();

    } /* keep playing if there is money left in the stake */

    /* free the shoe from memory */
    free_shoe(&shoe);

    /* tell user they lost the entire stake */
    printf("You've lost your entire stake!\n");

    return 0;
}
