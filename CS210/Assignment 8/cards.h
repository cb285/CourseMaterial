#ifndef CARDS_H_INCLUDED
#define CARDS_H_INCLUDED

#define MAX_HAND 21

/* declare structs */
typedef struct {
    char suit;
    int value;
}card_t;

typedef struct {
    card_t *shoe;
    int num_decks;
    int next;
}shoe_t;

typedef struct {
    card_t cards[MAX_HAND];
    int num_cards;
}hand_t;

/* function headers: */
extern card_t create_card (char suit, int value);
extern void print_card (card_t card);
extern int points (card_t card);
extern shoe_t create_shoe (int num_of_decks);
extern card_t draw_card (shoe_t *a_shoe);
extern int cards_left (shoe_t a_shoe);
extern void reshuffle (shoe_t *a_shoe);
extern void free_shoe (shoe_t *a_shoe);
extern hand_t empty_hand ();
extern void print_hand (hand_t a_hand);
extern void add_card_to_hand (hand_t *a_hand, card_t card);
extern int blackjack_points (hand_t a_hand);

#endif // CARDS_H_INCLUDED
