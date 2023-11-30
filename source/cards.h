
#ifndef CARDS_H_
#define CARDS_H_

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


#endif /* CARDS_H_ */