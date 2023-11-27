
#ifndef CARDS_H_
#define CARDS_H_

typedef enum Color Color;
enum Color {
  SPADE,
  DIAMOND,
  HEART,
  CLUB
};

typedef enum Number Number;
enum Number {
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

typedef struct Card Card;
struct Card;

typedef struct Deck Deck;
struct Deck;


#endif /* CARDS_H_ */