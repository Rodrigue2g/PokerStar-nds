#include <nds.h>
#include <stdio.h>
#include "graphics_top.h"

#include "bkg.h"
#include "top.h"
#include "backCard.h"
#include "asClub.h"
#include "kingClub.h"
#include "queenClub.h"
#include "jackClub.h"
#include "tenClub.h"
#include "tenHeart.h"
#include "nineHeart.h"
#include "eightHeart.h"
#include "sevenHeart.h"


#define TOP_SCREEN_WIDTH	256
#define	TOP_SCREEN_HEIGHT	192

#define	TOP_SPRITE_WIDTH	32
#define	TOP_SPRITE_HEIGHT	64


void configGraphics_Top() {
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG1_ACTIVE | DISPLAY_BG0_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_1D_SIZE_64;  
	configureBG1_Top();
	configureSprites_Top();
}

void configureBG0_Top() {
	//Activate and configure VRAM bank to work in background mode
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	//BGCTRL[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_64x32;
	BGCTRL[1] = BG_MAP_BASE(1) | BG_TILE_BASE(2) | BG_32x32 | BG_COLOR_256;

	//Copy data to display background (tiles, palette and map)
	swiCopy(bkgTiles, BG_TILE_RAM(2), bkgTilesLen/2);
	swiCopy(bkgPal, BG_PALETTE, bkgPalLen/2);
	swiCopy(&bkgMap, BG_MAP_RAM(1), bkgMapLen/2);
}

void configureBG1_Top() {
	//Activate and configure VRAM bank to work in background mode
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_MAIN_BG;

	BGCTRL[0] = BG_MAP_BASE(3) | BG_TILE_BASE(4) | BG_64x32 | BG_COLOR_256;

	swiCopy(topTiles, BG_TILE_RAM(4), topTilesLen/2);
    swiCopy(topPal, BG_PALETTE, topPalLen/2);  // Offset the palette if needed
    swiCopy(&topMap[0], BG_MAP_RAM(3), topMapLen/2);
}

Sprite player1 = {0, ARGB16(1, 31, 0, 0), 15, 20};
Sprite player2 = {0, ARGB16(1, 0, 31, 0), 110, 3};
Sprite player3 = {0, ARGB16(1, 0, 0, 31), 210, 20};

Sprite card1;
Sprite card2;
Sprite card3;
Sprite card4;
Sprite card5;

void configureSprites_Top() {
	//Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	//REG_POWERCNT = POWER_LCD | POWER_2D_A;
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE;
	//Initialize sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_32, false);

	card1.x = 40;
	card1.y = 65;
	card2.x = 75;
	card2.y = 65;
	card3.x = 110;
	card3.y = 65;
	card4.x = 145;
	card4.y = 65;
	card5.x = 180;
	card5.y = 65;
	//Allocate space for the graphic to show in the sprite
	card1.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color); //SpriteColorFormat_256Color);
	card2.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
	card3.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
	card4.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
	card5.gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);

	swiCopy(queenClubPal, SPRITE_PALETTE, queenClubPalLen/2);
	swiCopy(queenClubTiles, card1.gfx, queenClubTilesLen/2);

	swiCopy(jackClubPal, SPRITE_PALETTE, jackClubPalLen/2);
	swiCopy(jackClubTiles, card2.gfx, jackClubTilesLen/2);
	
	swiCopy(nineHeartPal, SPRITE_PALETTE, nineHeartPalLen/2);
	swiCopy(nineHeartTiles, card3.gfx, nineHeartTilesLen/2);

	swiCopy(eightHeartPal, SPRITE_PALETTE, eightHeartPalLen/2);
	swiCopy(eightHeartTiles, card4.gfx, eightHeartTilesLen/2);

	swiCopy(kingClubPal, SPRITE_PALETTE, kingClubPalLen/2);
	swiCopy(kingClubTiles, card5.gfx, kingClubTilesLen/2);

	//dmaCopy(asClubPal, SPRITE_PALETTE, asClubPalLen/2);
	//dmaCopy(kingClubPal, &SPRITE_PALETTE[asClubPalLen/2], kingClubPalLen/2);
	//dmaCopy(queenClubPal, &SPRITE_PALETTE[(asClubPalLen+kingClubPalLen)/2], queenClubPalLen/2);

	//dmaCopy(asClubTiles, card1.gfx, asClubTilesLen);
	//dmaCopy(kingClubTiles, card2.gfx, kingClubTilesLen);
	//dmaCopy(queenClubTiles, card3.gfx, queenClubTilesLen);

	//player1.gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
	//player2.gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
	//player3.gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
}

//int x = 0, y = 0, keys;
void updateGraphics_Top() {
	displayCard(card1, true, 0);
	displayCard(card2, true, 1);
	displayCard(card3, true, 2);
	displayCard(card4, true, 3);
	displayCard(card5, true, 4);

	//createPlayer(player1, &oamMain, 1);
	//createPlayer(player2, &oamMain, 2);
	//createPlayer(player3, &oamMain, 3);
    
	swiWaitForVBlank();
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
    	0,				// Palette to use
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
		false, //hide the sprite
		false, false, //vflip, hflip
		false //apply mosaic
	);
}
