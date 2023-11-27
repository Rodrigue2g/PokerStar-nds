/*
 * graphics_sub.c
 *
 *  Created on: Jan 7, 2017
 *      Author: nds
 */
#include <nds.h>
#include <stdio.h>
#include "graphics_bottom.h"

#include "bottom.h"
#include "onec.h"

#define BOTTOM_SCREEN_WIDTH	    256
#define	BOTTOM_SCREEN_HEIGHT	192

#define	BOTTOM_SPRITE_WIDTH	    32
#define	BOTTOM_SPRITE_HEIGHT	64

//Pointer to the graphic buffer where to store the sprite
u16* gfx_bottom;

void configGraphics_Bottom() {
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	//REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG0_ACTIVE;
	configureBG0_Bottom();
	configureSprites_Bottom();
}

void configureBG0_Bottom() {
	//Activate and configure VRAM bank to work in background mode
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	//BG0 configuration for the background
	BGCTRL_SUB[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_32x32;

	//Copy data to display background (tiles, palette and map)
	swiCopy(bottomTiles, BG_TILE_RAM_SUB(1), bottomTilesLen/2);
	swiCopy(bottomPal, BG_PALETTE_SUB, bottomPalLen/2);
	swiCopy(bottomMap, BG_TILE_RAM_SUB(0), bottomMapLen/2);
}

void configureSprites_Bottom() {
	//Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE;
	//Initialize sprite manager and the engine
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	//Allocate space for the graphic to show in the sprite
	gfx_bottom = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);

	//Copy data for the graphic (palette and bitmap)
	swiCopy(onecPal, SPRITE_PALETTE, onecPalLen/2);
	swiCopy(onecTiles, gfx_bottom, onecTilesLen/2);
}

void newCard(int card) {}

void fold() {}

void rmCards(){}



//int x = 0, y = 0, keys;
void updateGraphics_Bottom() {
    /*
    	//Read held keys
    	scanKeys();
    	keys = keysHeld();

    	//Modify position of the sprite accordingly
    	if((keys & KEY_RIGHT) && (x < (TOP_SCREEN_WIDTH - TOP_SPRITE_WIDTH))) x+=2;
    	if((keys & KEY_DOWN) && (y < (TOP_SCREEN_HEIGHT - TOP_SPRITE_HEIGHT))) y+=2;
    	if((keys & KEY_LEFT) && (x  > 0)) x-=2;
    	if((keys & KEY_UP) && (y  > 0)) y-=2;

    	oamSet(&oamMain, 	// oam handler
    		0,				// Number of sprite
    		x, y,			// Coordinates
    		0,				// Priority
    		0,				// Palette to use
    		SpriteSize_32x64,			// Sprite size
    		SpriteColorFormat_256Color,	// Color format
    		gfx,			// Loaded graphic to display
    		-1,				// Affine rotation to use (-1 none)
    		false,			// Double size if rotating
    		false,			// Hide this sprite
    		false, false,	// Horizontal or vertical flip
    		false			// Mosaic
    		);
    	swiWaitForVBlank();
    	//Update the sprites
		oamUpdate(&oamMain);
    */
}
