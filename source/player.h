#ifndef PLAYER_H_
#define PLAYER_H_
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <vector>
#include "cards.h"
/* 
typedef struct Player {
    Card *hole[2];  //starting cards
    Card *hand[5];  // best hand with commuinty cards included
    
    bool isDealer;
    bool hasFolded;
    int *Time; //must be a time var ==> change later

    double bankroll;
    double currentBet;
    bool isAllIn;

} Player;
 */
/*
class Player {
public:
protected:
private:
    unsigned int id;
    char *name;

    std::vector<Card*> hole; //Card *hole[2];  //starting cards
    std::vector<Card*> hand; //Card *hand[5];  // best hand with commuinty cards included
    
    bool isDealer;
    bool hasFolded;  //or  bool isIn; ?           
    int *Time; //must be a time var ==> change later

    double bankroll;
    double currentBet;
    bool isAllIn;
};
*/
#endif /* PLAYER_H_ */