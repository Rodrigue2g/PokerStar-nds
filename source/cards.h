
#ifndef CARDS_H_
#define CARDS_H_

typedef enum Suit Suit;
enum Suit {
  SPADE,
  DIAMOND,
  HEART,
  CLUB
};

typedef enum Rank Rank;
enum Rank {
    AS = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5, 
    SIX = 6, 
    SEVEN = 7, 
    EIGHT = 8, 
    NINE = 9, 
    TEN = 10, 
    VALLET = 11, 
    QUEEN = 12, 
    KING = 13
};

typedef struct {
    Rank rank;
    Suit suit;
    /*
    char getCard() {
        return rank + suit
    };
    */
} Card;

typedef struct {
    Card *cards[52];
} Deck;

/*
typedef struct Card Card;
struct Card;

typedef struct Deck Deck;
struct Deck;
*/

#endif /* CARDS_H_ */