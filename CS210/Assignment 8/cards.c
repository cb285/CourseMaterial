/* File: card.c
 * contains functions useful for games involving cards.
 *
 * --------------------------------------------------------------
 * Class: CS 210                        Instructor: Dr. Deborah Hwang
 * Assignment: Programming Assignment 8 Date assigned:  4/16/15
 * Programmer: Clayton Brutus           Date completed: 4/26/15
 */

#include <stdio.h>      /* printf, scanf */
#include <stdlib.h>     /* calloc */
#include <ctype.h>      /* toupper */
#include <string.h>     /* strcpy */
#include <time.h>       /* time */
#include "cards.h"

#define MAX_SUIT 8        /* define max suit string length */
#define CARDS_PER_DECK 52 /* define cards in a deck as 52  */

/*  Function: create_card
    creates a card given a suit and value
 */
card_t create_card (char suit,  /* Rec'd: (S)pades, (H)earts, (D)iamonds, or (C)lubs. */
                    int value)  /* Rec'd: value of the card (1-13) */
                                /* Returned: the created card */
{
    /* declare card to be created */
    card_t card;

    /* make sure suit is upper case */
    suit = toupper(suit);

    /* if the suit isn't valid, then create a (C)lubs card */
    if (suit != 'S' && suit != 'H' && suit != 'D')
        card.suit = 'C';
    else
        card.suit = suit;

    /* if the card value is invalid, then make it a 2 */
    if (value > 13)
        card.value = 2;
    else
        card.value = value;

    /* return the created card */
    return card;
}

/*  Function: print_card
    prints a card in plain english.
 */
void print_card (card_t card) /* Rec'd: a card */
{
    /* declare string for storing the suit */
    char suit_string[MAX_SUIT];

    /* convert suit character to a string */
    if (card.suit == 'S')
        strcpy(suit_string, "Spades");
    else if (card.suit == 'H')
        strcpy(suit_string, "Hearts");
    else if (card.suit == 'D')
        strcpy(suit_string, "Diamonds");
    else
        strcpy(suit_string, "Clubs");

    /* print the card value and suit */
    if (card.value == 1)
        printf("Ace of %s", suit_string);
    else if (card.value == 11)
        printf("Jack of %s", suit_string);
    else if (card.value == 12)
        printf("Queen of %s", suit_string);
    else if (card.value == 13)
        printf("King of %s", suit_string);
    else
        printf("%d of %s", card.value, suit_string);
}

/*  Function: points
    finds the blackjack value of a card
 */
int points (card_t card)    /* Rec'd: a card */
                            /* Returned: blackjack value of the card */
{
    /* return 10 for jack, queen, or king */
    if (card.value > 10)
        return 10;

    /* return the value of other cards */
    else
        return card.value;
}

/*  Function: create_shoe
    creates a shoe of the specified number of decks then shuffles the shoe.
 */
shoe_t create_shoe (int num_of_decks) /* Rec'd: number of decks to put in the shoe */
                                      /* Returned: the created shoe */
{
    shoe_t result_shoe;
    int i, j, k;

    /* allocate memory for the array */
    result_shoe.shoe = (card_t *) calloc(CARDS_PER_DECK * num_of_decks, sizeof(card_t));
    result_shoe.num_decks = num_of_decks;

    /* fill the shoe array with cards */
    for (i = 0; i < num_of_decks; i++)
        for (j = 0; j < 4; j++)
            for (k = 1; k <= 13; k++)
            {
                if(j == 0)
                    result_shoe.shoe[i*CARDS_PER_DECK + j*13 + k-1] = create_card('S', k);
                else if (j == 1)
                    result_shoe.shoe[i*CARDS_PER_DECK + j*13 + k-1] = create_card('H', k);
                else if (j == 2)
                    result_shoe.shoe[i*CARDS_PER_DECK + j*13 + k-1] = create_card('D', k);
                else
                    result_shoe.shoe[i*CARDS_PER_DECK + j*13 + k-1] = create_card('C', k);
            }

    /* shuffle the shoe */
    reshuffle(&result_shoe);

    /* return the created shoe */
    return result_shoe;
}

/*  Function: draw_card
    draws the next card in a shoe.
 */
card_t draw_card (shoe_t *a_shoe) /* Rec'd/P'back: a shoe of cards */
                                  /* Returned: a card */
{
    /* reshuffle the deck if no cards are left */
    if (cards_left(*a_shoe) == 0)
        reshuffle(a_shoe);

    /* draw and return the next card */
        a_shoe->next++;
        return a_shoe->shoe[a_shoe->next - 1];
}

/*  Function: cards_left
    returns however many cards are left in the shoe.
 */
int cards_left (shoe_t a_shoe) /* Rec'd: a shoe of cards */
                               /* Returned: number of cards left */
{
    /* calculate and return the number of cards left */
    return (a_shoe.num_decks*CARDS_PER_DECK - a_shoe.next);
}

/*  Function: reshuffle
    randomly switches the positions of the cards in a shoe.
 */
void reshuffle (shoe_t *a_shoe) /* Rec'd/P'back: a shoe of cards */
{
    /* declare variables for indexes of cards to be switched */
    int i, n, random_position;

    /* declare a temporary card variable */
    card_t temp;

    /* seed the random number generator */
    srand(time(0));

    /* tell the player that the deck is being shuffled */
    printf("\nSHUFFLING!\n");

    /* set n to total number of cards */
    n = a_shoe->num_decks * CARDS_PER_DECK;

    /* randomly swap cards' positions */
    for (i=0; i < n; i++)
    {
        random_position = i + rand() % (n-i);
        temp = a_shoe->shoe[random_position];
        a_shoe->shoe[random_position] = a_shoe->shoe[i];
        a_shoe->shoe[i] = temp;
    }

    /* set the next card to 0 */
    a_shoe->next = 0;
}

/*  Function: free_shoe
    frees a shoe from memory.
 */
void free_shoe (shoe_t *a_shoe) /* Rec'd/P'back: a shoe */
{
    /* free the shoe */
    free (a_shoe->shoe);
}

/*  Function: empty_hand
    sets the number of cards in a hand to 0.
 */
hand_t empty_hand ()
                     /* Returned: an empty hand */
{
    /* create the hand to be be returned */
    hand_t hand;
    /* set number of cards to 0 */
    hand.num_cards = 0;
    /* return the empty hand */
    return hand;
}

/*  Function: print_hand
    prints all the cards in a hand.
 */
void print_hand (hand_t a_hand) /* Rec'd: a hand of cards */
{
    /* initialize lcv */
    int i;

    /* print all the cards */
    for (i = 0; i < a_hand.num_cards; i++)
    {
        print_card(a_hand.cards[i]);
        printf("\n");
    }
}

/*  Function: add_card_to_hand
    adds a card to a player's hand.
 */
void add_card_to_hand (hand_t *a_hand, /* Rec'd/P'back: a hand */
                       card_t card)    /* Rec'd: a card */
{
    /* put the card in the shoe's array of cards */
    a_hand->cards[a_hand->num_cards] = card;

    /* add 1 the number of cards in the hand */
    a_hand->num_cards++;
}

/*  Function: blackjack_points
    calculates the total points for a hand.
 */
int blackjack_points (hand_t a_hand) /* Rec'd: a hand */
                                     /* Returned: total points */
{
    /* initialize lcv and the sum of card points */
    int i, sum = 0;

    /* add the points of each card to the total */
    for(i = 0; i < a_hand.num_cards; i++)
    {
        sum += points(a_hand.cards[i]);
        /* add 11 if the card is an ace it wont bust the player */
        if (sum <= 11 && points(a_hand.cards[i]) == 1)
            sum += 10;
    }

    /* return the sum of the points */
    return sum;
}
