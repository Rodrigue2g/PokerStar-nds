#include <nds.h>
#include <stdio.h>
#include "graphics_top.h"

#include "bkg.h"
#include "top.h"
#include "onec.h"
#include "backCard.h"

#define TOP_SCREEN_WIDTH	256
#define	TOP_SCREEN_HEIGHT	192

#define	TOP_SPRITE_WIDTH	32
#define	TOP_SPRITE_HEIGHT	64

//Pointer to the graphic buffer where to store the sprite
//u16* gfx;

void configGraphics_Top() {
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE;
	//configureBG0_Top();
	configureBG1_Top();
	configureSprites_Top();
}

void configureBG0_Top() {
	//Activate and configure VRAM bank to work in background mode
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	//BG0 configuration for the background
	BGCTRL[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_32x32;

	//Copy data to display background (tiles, palette and map)
	swiCopy(bkgTiles, BG_TILE_RAM(1), bkgTilesLen/2);
	swiCopy(bkgPal, BG_PALETTE, bkgPalLen/2);
	swiCopy(bkgMap, BG_MAP_RAM(0), bkgMapLen/2);
}

void configureBG1_Top() {
	//Activate and configure VRAM bank to work in background mode
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_MAIN_BG_0x06020000;

    BGCTRL[1] = BG_COLOR_256 | BG_BMP_BASE(8) | BG_TILE_BASE(3) | BG_64x32;

	swiCopy(topTiles, BG_TILE_RAM(3), topTilesLen/2);
    swiCopy(topPal, BG_PALETTE, topPalLen/2);  // Offset the palette if needed
    swiCopy(topMap, BG_MAP_RAM(8), topMapLen/2);
}

Sprite player1 = {0, ARGB16(1, 55, 55, 55), 40, 10};
Sprite player2 = {0, ARGB16(1, 31, 31, 31), 40, 30};
Sprite player3 = {0, ARGB16(1, 0, 0, 0), 40, 50};

Sprite card1;
Sprite card2;
Sprite card3;

void configureSprites_Top() {
	//Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE;
	//Initialize sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_128, false);

	//card1.pal = onecPal; 
	//card1.palLen = onecPalLen/2;
	//card1.tiles = onecTiles; 
	//card1.tilesLen = onecTilesLen/2;
	card1.x = 40;
	card1.y = 65;
	card2.x = 75;
	card2.y = 65;
	card3.x = 110;
	card3.y = 65;
	//Allocate space for the graphic to show in the sprite
	card1.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color); //SpriteColorFormat_256Color);
	card2.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
	card3.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
	//Copy data for the graphic (palette and bitmap)
	//swiCopy(card1.pal, SPRITE_PALETTE, card1.palLen);
	//swiCopy(card1.tiles, card1.gfx, card1.palLen);

	swiCopy(onecPal, SPRITE_PALETTE, onecPalLen/2);
	swiCopy(onecTiles, card1.gfx, onecTilesLen/2);

	swiCopy(onecPal, SPRITE_PALETTE, onecPalLen/2);
	swiCopy(onecTiles, card2.gfx, onecTilesLen/2);

	swiCopy(onecPal, SPRITE_PALETTE, onecPalLen/2);
	swiCopy(onecTiles, card3.gfx, onecTilesLen/2);

	player1.gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
	player2.gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
	player3.gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
}

//int x = 0, y = 0, keys;
void updateGraphics_Top() {
    	//Read held keys
    	scanKeys();
    	int keys = keysHeld();

    	//Modify position of the sprite accordingly
    	if((keys & KEY_RIGHT) && (card1.x < (TOP_SCREEN_WIDTH - TOP_SPRITE_WIDTH))) card1.x+=2;
    	if((keys & KEY_DOWN) && (card1.y < (TOP_SCREEN_HEIGHT - TOP_SPRITE_HEIGHT))) card1.y+=2;
    	if((keys & KEY_LEFT) && (card1.x  > 0)) card1.x-=2;
    	if((keys & KEY_UP) && (card1.y  > 0)) card1.y-=2;
	/*
    	oamSet(
			&oamMain, 	// oam handler
    		0,				// Number of sprite
    		card1.x, card1.y,			// Coordinates
    		0,				// Priority
    		0,				// Palette to use
    		SpriteSize_32x64,			// Sprite size
    		SpriteColorFormat_256Color,	// Color format
    		card1.gfx,			// Loaded graphic to display
    		-1,				// Affine rotation to use (-1 none)
    		false,			// Double size if rotating
    		false,			// Hide this sprite
    		false, false,	// Horizontal or vertical flip
    		false			// Mosaic
    	);
		*/

	displayCard(card1, false, 5);
	displayCard(card2, false, 6);
	displayCard(card3, false, 7);

	createPlayer(player1, &oamMain, 2);
	createPlayer(player2, &oamMain, 3);
	createPlayer(player3, &oamMain, 4);
    
	swiWaitForVBlank();
    //Update the sprites
	oamUpdate(&oamMain);
}

void displayCard(Sprite card, bool reveal, int count) {
	if (!reveal) {
		swiCopy(backCardPal, SPRITE_PALETTE, backCardPalLen/2);
		swiCopy(backCardTiles, card.gfx, backCardTilesLen/2);
	} else {
		//swiCopy(onecPal, SPRITE_PALETTE, onecPalLen/2);
		//swiCopy(onecTiles, card.gfx, onecTilesLen/2);
	} 
	oamSet(
		&oamMain, 	// oam handler
    	count,				// Number of sprite
    	card.x, card.y,			// Coordinates
    	0,				// Priority
    	128,				// Palette to use
    	SpriteSize_32x64,			// Sprite size
    	SpriteColorFormat_256Color,	// Color format
		card.gfx,			// Loaded graphic to display
    	-1,				// Affine rotation to use (-1 none)
    	false,			// Double size if rotating
		false,			// Hide this sprite
    	false, false,	// Horizontal or vertical flip
    	false			// Mosaic
    );
}

void createPlayer(Sprite sprite, OamState* screen, int count) {
	dmaFillHalfWords(sprite.color, sprite.gfx, 16*16*2); // this is how to assign the color fill to the oam gfx
	oamSet(
		screen, //is it upper screen of bottom?
		count, // the oam entry to set
		sprite.x, sprite.y, // where should be positioned (x,y)?
		0, // priority
		15, // palette for 16 color sprite or alpha for bmp sprite
		SpriteSize_16x16, // size
		SpriteColorFormat_Bmp, // color type
		sprite.gfx, // the oam gfx
		0, //affine index
		true, //double the size of rotated sprites
		true, //don't hide the sprite
		false, false, //vflip, hflip
		false //apply mosaic
	);
}
