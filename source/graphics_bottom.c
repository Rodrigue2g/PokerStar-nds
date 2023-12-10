/*
 * graphics_sub.c
 *
 *  Created on: Jan 7, 2017
 *      Author: nds
 */
#include <nds.h>
#include <stdio.h>
#include "graphics_bottom.h"
#include "card.h"

#include "bottom.h"
#include "cards.h"

#define BOTTOM_SCREEN_WIDTH	    256
#define	BOTTOM_SCREEN_HEIGHT	192

#define	BOTTOM_SPRITE_WIDTH	    32
#define	BOTTOM_SPRITE_HEIGHT	64


void configGraphics_Bottom() {
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	//REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG0_ACTIVE;
	configureBG0_Bottom();
	configureSprites_Bottom();
	updateGraphics_Bottom();
}

void configureBG0_Bottom() {
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	BGCTRL_SUB[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_32x32;

	swiCopy(bottomTiles, BG_TILE_RAM_SUB(1), bottomTilesLen/2);
	swiCopy(bottomPal, BG_PALETTE_SUB, bottomPalLen/2);
	swiCopy(bottomMap, BG_TILE_RAM_SUB(0), bottomMapLen/2);
}

// Each Player has 2 cards in his hand
CardSpriteBottom hand1, hand2;

static int _count = 0;  // internal counter for sprites

static inline void initCard(CardSpriteBottom *card, u8* gfx, int x, int y)
{
	card->gfx = oamAllocateGfx(&oamSub, SpriteSize_32x64, SpriteColorFormat_256Color);
	card->frame_gfx = (u8*)gfx;
	card->count = _count++;
	card->x = x;
	card->y = y;
}

static inline void updateCard(CardSpriteBottom *card, CardState cardState) 
{
    u8* offset = card->frame_gfx + cardState * 32*64;
    dmaCopy(offset, card->gfx, 32*64);
}

void configureSprites_Bottom() 
{
	//REG_POWERCNT = POWER_LCD | POWER_2D_A;
	VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE;
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	initCard(&hand1, (u8*)cardsTiles, 90, 65);
	initCard(&hand2, (u8*)cardsTiles, 125, 65);

	dmaCopy(cardsPal, SPRITE_PALETTE_SUB, cardsPalLen);
}	

void updateGraphics_Bottom()
{
	//displayHand((CardState[]){AS_CLUB, TEN_CLUB});
	displayCard1(AS_CLUB);
	displayCard2(TEN_CLUB);

	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void displayHand(CardState* cardState) {
	updateCard(&hand1, cardState[0]);
	updateCard(&hand2, cardState[1]);
	displayCard(hand1, false);
	displayCard(hand2, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void displayCard1(CardState cardState) {
	updateCard(&hand1, cardState);
	displayCard(hand1, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void displayCard2(CardState cardState) {
	updateCard(&hand2, cardState);
	displayCard(hand2, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void fold(){
	displayCard(hand1, true);
	displayCard(hand2, true);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void rmCards(){}

void displayCard(CardSpriteBottom card, bool fold) {
	oamSet(
		&oamSub, 		// oam handler
    	card.count,		// Number of sprite
    	card.x, card.y,	// Coordinates
    	0,				// Priority
    	0,				// Palette to use
    	SpriteSize_32x64,			// Sprite size
    	SpriteColorFormat_256Color,	// Color format
		card.gfx,			// Loaded graphic to display
    	-1,				// Affine rotation to use (-1 none)
    	false,			// Double size if rotating
		fold,			// Hide this sprite
    	false, false,	// Horizontal or vertical flip
    	false			// Mosaic
    );
}

int getNbOfPlayers(int numPlayers) {
	// Wait for user input to set the number of players
    int _numPlayers = numPlayers;
	bool isok = false;
	consoleDemoInit();
	while(!isok) {
		swiWaitForVBlank();
		consoleClear();
		scanKeys();
		int keys = keysDown();
		iprintf("\nNumber of playrs: %u", _numPlayers);
		if(keys & KEY_UP) {
			_numPlayers++;
			if(_numPlayers > 6) _numPlayers = 6;
		}
		if(keys & KEY_DOWN) {
			_numPlayers--;
			if(_numPlayers < 2) _numPlayers = 2;
		}
		if(keys & KEY_A) {
			numPlayers = _numPlayers;
			isok = true;
		}
		if(keys & KEY_B) {
			isok = true;
		}
	}
	return numPlayers;
}
