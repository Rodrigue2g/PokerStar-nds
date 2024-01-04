#ifndef CARD_H_
#define CARD_H_

#ifdef __cplusplus
#include <iostream>
#include <vector>

enum CardState { 
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
};

enum Suit {
    SPADE = 0,
    DIAMOND = 13,
    HEART = 26,
    CLUB = 39
};

enum Rank {
    AS = 0,
    TWO = 1,
    THREE = 2,
    FOUR = 3,
    FIVE = 4, 
    SIX = 5, 
    SEVEN = 6, 
    EIGHT = 7, 
    NINE = 8, 
    TEN = 9, 
    VALLET = 10, 
    QUEEN = 11, 
    KING = 12
};

struct Card {
    Rank rank;
    Suit suit;
};

/**
 * @brief 
 * 
 */
typedef std::vector<Card*> Deck;

struct Player {
    unsigned int id;
    char *name;

    std::vector<Card*> hole;  //starting cards
    std::vector<Card*> hand;  // best hand with commuinty cards included
    
    bool isDealer;
    bool hasFolded;  //or  bool isIn; ?           
    int *Time; //must be a time var ==> change later

    int bankroll;
    int currentBet;
    bool isAllIn;

    bool isPlaying;
};

enum Action {
    CHECK,
    CALL,
    RAISE,
    ALLIN,
    FOLD
};

struct Move {
    Action action;
    int amount;
};
/*
Deck deckInit();
void initDeck(Card *deck);
*/
#else 
typedef enum { 
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
    SPADE = 0,
    DIAMOND = 13,
    HEART = 26,
    CLUB = 39
} Suit;

typedef enum {
    AS = 0,
    TWO = 1,
    THREE = 2,
    FOUR = 3,
    FIVE = 4, 
    SIX = 5, 
    SEVEN = 6, 
    EIGHT = 7, 
    NINE = 8, 
    TEN = 9, 
    VALLET = 10, 
    QUEEN = 11, 
    KING = 12
} Rank;

typedef struct {
    Rank rank;
    Suit suit;
} Card;

/**
 * @brief 
 * 
 */
// typedef std::vector<Card*> Deck;
typedef struct {
    unsigned int id;
    char *name;

    Card *hole[2];  //starting cards
    Card *hand[5];  // best hand with commuinty cards included
    
    bool isDealer;
    bool hasFolded;  //or  bool isIn; ?           
    int *Time; //must be a time var ==> change later

    int bankroll;
    int currentBet;
    bool isAllIn;

    bool isPlaying;
} Player;

typedef enum {
    CHECK,
    CALL,
    RAISE,
    ALLIN,
    FOLD
} Action;

typedef struct {
    Action action;
    int amount;
} Move;
#endif

#endif /* CARD_H_ */