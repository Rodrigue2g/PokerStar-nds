#include <nds.h>
#include <stdio.h>
#include "graphics_top.h"
#include "card.h"

#include "bkg.h"
#include "top.h"
#include "backCard.h"
#include "cards.h"
#include "loading.h"

#define TOP_SCREEN_WIDTH	256
#define	TOP_SCREEN_HEIGHT	192

#define	TOP_SPRITE_WIDTH	32
#define	TOP_SPRITE_HEIGHT	64

PrintConsole topScreen;

void loadingTop()
{
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	BGCTRL[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_64x32;
	swiCopy(loadingTiles, BG_TILE_RAM(1), loadingTilesLen/2);
	swiCopy(loadingPal, BG_PALETTE, loadingPalLen/2);
	swiCopy(&loadingMap[0], BG_MAP_RAM(0), loadingMapLen/2);
}

void configGraphics_Top() 
{
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;
	vramSetBankA(VRAM_A_MAIN_BG);

	BGCTRL[2] = BG_MAP_BASE(0) | BG_TILE_BASE(3) | BG_32x32 | BG_COLOR_256 | BG_PRIORITY_0;
	BGCTRL[3] = BG_MAP_BASE(1) | BG_TILE_BASE(5) | BG_32x32 | BG_COLOR_256 | BG_PRIORITY_1;

	consoleInit(&topScreen,0, BgType_Text4bpp, BgSize_T_256x256, 4,1, true, true);
	//printf("\x1b[%d;%dH\x1b[5m%s:%d %s", 7, 8, "Total Pot", 0, "BB");

	swiCopy(topTiles, BG_TILE_RAM(3), topTilesLen/2);
   	swiCopy(topPal, BG_PALETTE, topPalLen/2);
    swiCopy(topMap, BG_MAP_RAM(0), topMapLen/2);
	/*
	swiCopy(bkgTiles, BG_TILE_RAM(5), bkgTilesLen/2);
	swiCopy(bkgPal, BG_PALETTE, bkgPalLen/2);
	swiCopy(bkgMap, BG_MAP_RAM(1), bkgMapLen/2); */

	configureSprites_Top();
}

void configureBG1_Top()  //rm
{
	// VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	BGCTRL[3] = BG_MAP_BASE(1) | BG_TILE_BASE(5) | BG_32x32 | BG_COLOR_256 | BG_PRIORITY_1;

	swiCopy(bkgTiles, BG_TILE_RAM(5), bkgTilesLen/2);
	swiCopy(bkgPal, BG_PALETTE, bkgPalLen/2);
	swiCopy(bkgMap, BG_MAP_RAM(1), bkgMapLen/2);
}

void configureBG0_Top()  //rm
{
   	//VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	
	BGCTRL[2] = BG_MAP_BASE(0) | BG_TILE_BASE(3) | BG_32x32 | BG_COLOR_256 | BG_PRIORITY_0;

	swiCopy(topTiles, BG_TILE_RAM(3), topTilesLen/2);
   	swiCopy(topPal, BG_PALETTE, topPalLen/2);
    swiCopy(topMap, BG_MAP_RAM(0), topMapLen/2);
}

// Max 6 players --> init+display the number needed
PlayerSprite player1, player2, player3,  player4,  player5,  player6;
// Max 5 cards --> display flop, turn and river
CardSpriteTop card1, card2, card3, card4, card5;

static int _count = 0;  // internal counter for sprites

static inline void initPlayer(PlayerSprite *player, int color, int x, int y)
{
	player->gfx =  oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	player->count = _count++;
	player->color = color;
	player->x = x;
	player->y = y;
}

static inline void initCardTop(CardSpriteTop *card, u8* gfx, int x, int y)
{
	card->gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
	card->frame_gfx = (u8*)gfx;
	card->count = _count++;
	card->x = x;
	card->y = y;
}

static inline void updateCardTop(CardSpriteTop *card, CardState cardState) 
{
    u8* offset = card->frame_gfx + cardState * 32*64;
    dmaCopy(offset, card->gfx, 32*64);
}

static void configureSprites_Top() 
{
	//REG_POWERCNT = POWER_LCD | POWER_2D_A;
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE;
	oamInit(&oamMain, SpriteMapping_1D_128, false);  //32

	initCardTop(&card1, (u8*)cardsTiles, 40, 67);
	initCardTop(&card2, (u8*)cardsTiles, 75, 67);
	initCardTop(&card3, (u8*)cardsTiles, 110, 67);
	initCardTop(&card4, (u8*)cardsTiles, 145, 67);
    initCardTop(&card5, (u8*)cardsTiles, 180, 67);

	dmaCopy(cardsPal, SPRITE_PALETTE, cardsPalLen);

	initPlayer(&player1, ARGB16(1, 31, 0, 0), -5, 0);
	initPlayer(&player2, ARGB16(1, 0, 31, 0), 97, -15);
	initPlayer(&player3, ARGB16(1, 0, 0, 31), 200, 0);
	initPlayer(&player4, ARGB16(1, 0, 0, 31), 0, 0);
	initPlayer(&player5, ARGB16(1, 0, 0, 31), 10, 20);
	initPlayer(&player6, ARGB16(1, 0, 0, 31), 20, 20);

	//displayPlayer(player1);
	//swiWaitForVBlank();
	//oamUpdate(&oamMain);
}


void displayFlop(CardState *cardState) 
{
	updateCardTop(&card1, cardState[0]);
	updateCardTop(&card2, cardState[1]);
	updateCardTop(&card3, cardState[2]);
	displayCardTop(card1, true);
	displayCardTop(card2, true);
	displayCardTop(card3, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}
void displayFlop1(CardState cardState) 
{
	updateCardTop(&card1, cardState);
	displayCardTop(card1, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}
void displayFlop2(CardState cardState) 
{
	updateCardTop(&card2, cardState);
	displayCardTop(card2, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}
void displayFlop3(CardState cardState) 
{
	updateCardTop(&card3, cardState);
	displayCardTop(card3, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}


void displayTurn(CardState cardState) 
{
	updateCardTop(&card4, cardState);
	displayCardTop(card4, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}
void displayRiver(CardState cardState) 
{
	updateCardTop(&card5, cardState);
	displayCardTop(card5, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}

void cleanTop() 
{
	oamClear(&oamMain, 0, 0);
	displayCardTop(card1, false);
	displayCardTop(card2, false);
	displayCardTop(card3, false);
	displayCardTop(card4, false);
	displayCardTop(card5, false);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}

static void displayCardTop(CardSpriteTop card, bool reveal) 
{
	if (!reveal) {
		//swiCopy(backCardPal, SPRITE_PALETTE, backCardPalLen/2);
		//swiCopy(backCardTiles, card.gfx, backCardTilesLen/2);
	} else {
		//swiCopy(onecPal, SPRITE_PALETTE, onecPalLen/2);
		//swiCopy(onecTiles, card.gfx, onecTilesLen/2);
	} 
	oamSet(
		&oamMain, 		// oam handler
    	card.count,		// Number of sprite
    	card.x, card.y,	// Coordinates
    	0,				// Priority
    	0,				// Palette to use
    	SpriteSize_32x64,			// Sprite size
    	SpriteColorFormat_256Color,	// Color format
		card.gfx,			// Loaded graphic to display
    	-1,				// Affine rotation to use (-1 none)
    	false,			// Double size if rotating
		!reveal,			// Hide this sprite
    	false, false,	// Horizontal or vertical flip
    	false			// Mosaic
    );
}

static void displayPlayer(PlayerSprite player) // OamState* screen
{
	dmaFillHalfWords(player.color, player.gfx, 32*32*2);
	oamSet(
		&oamMain, //is it upper screen of bottom?
		player.count, // the oam entry to set
		player.x, player.y, // where should be positioned (x,y)?
		0, // priority
		0, // palette for 16 color sprite or alpha for bmp sprite
		SpriteSize_32x32, // size
		SpriteColorFormat_256Color, // color type
		player.gfx, // the oam gfx
		0, //affine index
		true, //double the size of rotated sprites
		false, //hide the sprite
		false, false, //vflip, hflip
		false //apply mosaic
	);
}

void updateGraphics_Top(const std::vector<Player*> players, const int total_pot, const int current_bet) 
{
	consoleSelect(&topScreen);
	consoleClear();
	
	iprintf("\x1b[%d;%dH\x1b[37m%s:%d %s", 5, 9, "Total Pot", total_pot, "BB");
	iprintf("\x1b[%d;%dH\x1b[37m%s:%d %s", 17, 7, "Current Bet", current_bet, "BB");

	for(auto player : players) {
		// if(player->id == 0) continue;
		if(player->isPlaying) {
			iprintf("\x1b[%d;%dH\x1b[37m%s %d %s", 21, 7, "Player", player->id, "is playing");
		}
	}
	printf("\x1b[%d;%dH\x1b[5m%s:%d", 1, 0, "Player", players[1]->id);
	printf("\x1b[%d;%dH\x1b[5m%d %s", 2, 0, players[1]->bankroll, "BB");
	if(!players[1]->hasFolded){
		printf("\x1b[%d;%dH\x1b[5m%d %s", 3, 1, players[1]->currentBet, "BB");
	}
	
	printf("\x1b[%d;%dH\x1b[5m%s:%d", 1, 24, "Player", players[2]->id);
	printf("\x1b[%d;%dH\x1b[5m%d %s", 2, 24, players[2]->bankroll, "BB");
	if(!players[2]->hasFolded){
		printf("\x1b[%d;%dH\x1b[5m%d %s", 3, 25, players[2]->currentBet, "BB");
	}

	swiWaitForVBlank();
	oamUpdate(&oamMain);
}