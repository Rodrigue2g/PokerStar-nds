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
#include "asClub.h"
#include "tenClub.h"

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
	//Activate and configure VRAM bank to work in background mode
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	//BG0 configuration for the background
	BGCTRL_SUB[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_32x32;

	//Copy data to display background (tiles, palette and map)
	swiCopy(bottomTiles, BG_TILE_RAM_SUB(1), bottomTilesLen/2);
	swiCopy(bottomPal, BG_PALETTE_SUB, bottomPalLen/2);
	swiCopy(bottomMap, BG_TILE_RAM_SUB(0), bottomMapLen/2);
}

subSprite hand_c1;
subSprite hand_c2;
void configureSprites_Bottom() {
	//Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE;
	//Initialize sprite manager and the engine
	oamInit(&oamSub, SpriteMapping_1D_32, false);
	/*
	hand_c1.x = 40;
	hand_c1.y = 65;
	hand_c2.x = 75;
	hand_c2.y = 65;

	//Allocate space for the graphic to show in the sprite
	hand_c1.gfx = oamAllocateGfx(&oamSub, SpriteSize_32x64, SpriteColorFormat_256Color);
	hand_c2.gfx = oamAllocateGfx(&oamSub, SpriteSize_32x64, SpriteColorFormat_256Color);

	//Copy data for the graphic (palette and bitmap)
	swiCopy(asClubPal, SPRITE_PALETTE_SUB, asClubPalLen/2);
	swiCopy(asClubTiles, hand_c1.gfx, asClubTilesLen/2);
	swiCopy(tenClubPal, SPRITE_PALETTE_SUB, tenClubPalLen/2);
	swiCopy(tenClubTiles, hand_c2.gfx, tenClubTilesLen/2);
	*/
}

void newCard(int card) {}

void fold() {}

void rmCards(){}


#define  SPRITE_MAX 128
subSprite sprites[52];

void updateGraphics_Bottom() {
	for(int i = 0; i < 2; i++)
		createSprite(&sprites[i], rand() % 256, rand() % 192, 0, SpriteSize_32x64, SpriteColorFormat_256Color, rand() % 4 - 2, rand() % 4 - 2);
	for(int i = 0; i < 256; i++) {
      //SPRITE_PALETTE[i] = i;
      //SPRITE_PALETTE_SUB[i] = 256;
	}

	updateSprites();
    swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void createSprite(subSprite* s, int x, int y, int z, SpriteSize size, SpriteColorFormat format, int dx, int dy) {
	s->alive = true;
	s->x = x;
	s->y = y;
	s->z = z; 
	s->dx = dx;
	s->dy = dy;
	s->size = size;
	s->format = format;
    
	//api: allocate a chunk of sprite graphics memory
	s->gfx = oamAllocateGfx(&oamSub, size, format);
}

void killSprite(subSprite *s) {
	s->alive = false;  
	if(s->gfx) oamFreeGfx(&oamSub, s->gfx);
	s->gfx = 0;
}

void updateSprites(void) {
	//set oam to values required by my sprite
	for(int i = 0; i < 2; i++) { //SPRITE_MAX
		//an api function: void oamSet(OamState* oam, int id,  int x, int y, int priority, int palette_alpha, SpriteSize size, SpriteColorFormat format, const void* gfxOffset, int affineIndex, bool sizeDouble, bool hide);
		oamSet(
			&oamSub, 
			i, 
			sprites[i].x, sprites[i].y, 
			0, 
			0,
			sprites[i].size,
			sprites[i].format, 
			sprites[i].gfx, 
			-1, 
			false, 
			!sprites[i].alive,
			false,
			false, 
			false
		);
	}
}


int getNumberOfPlayers() {
	// Wait for user input to set the number of players
    int numPlayers = 2;
	bool isok = false;
	consoleDemoInit();
	while(!isok) {
		swiWaitForVBlank();
		consoleClear();
		scanKeys();
		int keys = keysDown();
		iprintf("\nNumber of playrs: %u", numPlayers);
		if(keys & KEY_UP) {
			numPlayers++;
			if(numPlayers > 6) numPlayers = 6;
		}
		if(keys & KEY_DOWN) {
			numPlayers--;
			if(numPlayers < 2) numPlayers = 2;
		}
		if(keys & KEY_A) {
			isok = true;
		}
		if(keys & KEY_B) {
			numPlayers = 2;
			isok = true;
		}
	}
	return numPlayers;
}
