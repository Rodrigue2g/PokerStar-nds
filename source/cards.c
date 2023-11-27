
#include "cards.h"
#include <stdio.h>

struct Card {
    Number number;
    Color color;
    /*
    char getCard() {
        return number + color
    };
    */
};

struct Deck {
    Card *cards[52];
};