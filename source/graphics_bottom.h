/*
 * graphics_sub.h
 *
 *  Created on: Jan 7, 2017
 *      Author: nds
 */

#include <nds.h>
#include <stdio.h>

#ifndef GRAPHICS_BOTTOM_H_
#define GRAPHICS_BOTTOM_H_


enum SPRITES {
	TWO_CLUB, TWO_SPADE, TWO_HEART, TWO_DIAMOND,
	THREE_CLUB, THREE_SPADE, THREE_HEART, THREE_DIAMOND,
	FOUR_CLUB, FOUR_SPADE, FOUR_HEART, FOUR_DIAMOND,
	FIVE_CLUB, FIVE_SPADE, FIVE_HEART, FIVE_DIAMOND,
	SIX_CLUB, SIX_SPADE, SIX_HEART, SIX_DIAMOND,
	SEVEN_CLUB, SEVEN_SPADE, SEVEN_HEART, SEVEN_DIAMOND,
	EIGHT_CLUB, EIGHT_SPADE, EIGHT_HEART, EIGHT_DIAMOND,
	NINE_CLUB, NINE_SPADE, NINE_HEART, NINE_DIAMOND,
	TEN_CLUB, TEN_SPADE, TEN_HEART, TEN_DIAMOND,
	JACK_CLUB, JACK_SPADE, JACK_HEART, JACK_DIAMOND,
	QUEEN_CLUB, QUEEN_SPADE, QUEEN_HEART, QUEEN_DIAMOND,
	KING_CLUB, KING_SPADE, KING_HEART, KING_DIAMOND,
	AS_CLUB, AS_SPADE, AS_HEART, AS_DIAMOND, 
	BACK
};


void configGraphics_Bottom();
void configureBG0_Bottom();
void configureSprites_Bottom();


void newCard(int card);

void fold();

void rmCards();


void updateGraphics_Bottom();


typedef struct {
	int x,y,z;
	int dx, dy;
	bool alive;
	u16* gfx;
	SpriteColorFormat format;
	SpriteSize size;
}subSprite;


void createSprite(subSprite* s, int x, int y, int z, SpriteSize size, SpriteColorFormat format, int dx, int dy);
void killSprite(subSprite *s);
void updateSprites(void);

int getNumberOfPlayers();

#endif /* GRAPHICS_BOTTOM_H_ */
