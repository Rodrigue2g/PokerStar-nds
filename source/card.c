
#include "card.h"
#include <stdio.h>
#include <stdlib.h>


Deck deckInit() {
    Deck deck;
    int i = 0;

    for (int suit = SPADE; suit <= CLUB; suit++) {
        for (int rank = AS; rank <= KING; rank++) {
            Card *card = malloc(sizeof(Card));
            card->suit = suit;
            card->rank = rank;
            deck.cards[i++] = card;
        }
    }
    return deck;
}

void initDeck(Card *deck) {
    int i = 0;
    for (int suit = SPADE; suit <= CLUB; suit++) {
        for (int rank = AS; rank <= KING; rank++) {
            deck[i].suit = suit;
            deck[i].rank = rank;
            i++;
        }
    }
}