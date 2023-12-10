
#ifndef CARD_H_
#define CARD_H_
typedef enum{ 
    AS_SPADE = 0, 
    TWO_SPADE = 1, 
    THREE_SPADE = 2, 
    FOUR_SPADE = 3, 
    FIVE_SPADE = 4, 
    SIX_SPADE = 5, 
    SEVEN_SPADE = 6,
    EIGHT_SPADE = 7, 
    NINE_SPADE = 8, 
    TEN_SPADE = 9, 
    JACK_SPADE = 10, 
    QUEEN_SPADE = 11, 
    KING_SPADE = 12,
    AS_HEART = 13, 
    TWO_HEART = 14, 
    THREE_HEART = 15, 
    FOUR_HEART = 16, 
    FIVE_HEART = 17, 
    SIX_HEART = 18, 
    SEVEN_HEART = 19, 
    EIGHT_HEART = 20, 
    NINE_HEART = 21, 
    TEN_HEART = 22, 
    JACK_HEART = 23, 
    QUEEN_HEART = 24, 
    KING_HEART = 25,
    AS_DIAMOND = 26, 
    TWO_DIAMOND = 27, 
    THREE_DIAMOND = 28, 
    FOUR_DIAMOND = 29, 
    FIVE_DIAMOND = 30, 
    SIX_DIAMOND = 31, 
    SEVEN_DIAMOND = 32, 
    EIGHT_DIAMOND = 33, 
    NINE_DIAMOND = 34, 
    TEN_DIAMOND = 35, 
    JACK_DIAMOND = 36, 
    QUEEN_DIAMOND = 37, 
    KING_DIAMOND = 38,
    AS_CLUB = 39, 
    TWO_CLUB = 40, 
    THREE_CLUB = 41, 
    FOUR_CLUB = 42, 
    FIVE_CLUB = 43, 
    SIX_CLUB = 44, 
    SEVEN_CLUB = 45, 
    EIGHT_CLUB = 46, 
    NINE_CLUB = 47, 
    TEN_CLUB = 48, 
    JACK_CLUB = 49, 
    QUEEN_CLUB = 50, 
    KING_CLUB = 51 
} CardState;

typedef enum {
    SPADE,
    DIAMOND,
    HEART,
    CLUB
} Suit;

typedef enum {
    AS = 1,
    TWO,
    THREE,
    FOUR,
    FIVE, 
    SIX, 
    SEVEN, 
    EIGHT, 
    NINE, 
    TEN, 
    VALLET, 
    QUEEN, 
    KING
} Rank;

typedef struct {
    Rank rank;
    Suit suit;
} Card;

typedef struct {
    Card *cards[52];
} Deck;

Deck deckInit();
void initDeck(Card *deck);


#endif /* CARD_H_ */