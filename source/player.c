#include "player.h"
#include "cards.h"
#include <stdio.h>
#include <stdbool.h>

struct Player {
    Card *hole[2];  //starting cards
    Card *hand[5];  // best hand with commuinty cards included
    
    bool isDealer;
    bool hasFolded;
    int *Time; //must be a time var ==> change later

    double bankroll;
    double currentBet;
    bool isAllIn;

};