#include "game.h"
#include "player.h"
#include "cards.h"

#include <stdio.h>
#include <stdbool.h>


struct Game {
    Player *players;
    Player *dealer;

    Card *deck[52];
    Deck *localDeck;  //Or initialize here if without a pointer

    double pot;

    int ante;
    int blind;
    int smallBlind;
    int bigBlind;

    Card *communityCards[5];
    Card *flop[3];
    Card *turn;
    Card *river;

    int *clock;
    int *time; // If a player wants more time ==> Maybe in Player only?

};