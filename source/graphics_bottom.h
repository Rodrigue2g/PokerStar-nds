
#ifndef GRAPHICS_BOTTOM_H_
#define GRAPHICS_BOTTOM_H_

#include <nds.h>
#include <stdio.h>
#include "card.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	int x, y;

	u16* gfx;
	u8*  frame_gfx;

	int state;
    int count;
} CardSpriteBottom;

void configGraphics_Bottom();
void configureBG0_Bottom();
void configureSprites_Bottom();

void displayHand(CardState* cardState);
void displayCard1(CardState cardState);
void displayCard2(CardState cardState);

void fold();
void rmCards();

void displayCard(CardSpriteBottom card, bool fold);

/**
 * @brief Trick to enable default arg value for getNumberOfPlayers();
 * 
 * ie: int getNumberOfPlayers(int numPlayers = 2);
 * 
 * @param numPlayers Defaults to 2 players if no argument is passed
 */
#define GET_NBP_WITH_ARGS(numPlayers) getNbOfPlayers(numPlayers)
#define GET_NBP_WITHOUT_ARGS() getNbOfPlayers(2)

#define GET_MACRO(_1, NAME, ...) NAME
#define getNumberOfPlayers(...) GET_MACRO(__VA_ARGS__, GET_NBP_WITH_ARGS, GET_NBP_WITHOUT_ARGS)(__VA_ARGS__)

int getNbOfPlayers(int numPlayers); //= 3

void printI(int i); // = 0

//void updateGraphics_Bottom(const Player* player, const int current_bet);
void updateGraphics_Bottom();
//Move waitForLocalPlayerMove(const Player* player, const int current_bet);
Move waitForLocalPlayerMove(const int current_bet, const int player_bet, const int player_bankroll);


#ifdef __cplusplus
}
#endif

#endif /* GRAPHICS_BOTTOM_H_ */
