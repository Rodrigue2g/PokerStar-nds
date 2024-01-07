// - nds/c++ libraries
#include <nds.h>
#include <stdio.h>
// - headers
#include "graphics_top.h"
#include "card.h"
// - grit headers
#include "loading.h"
#include "bkg.h"
#include "top.h"
#include "backCard.h" //rm
#include "cards.h"

/*
#define TOP_SCREEN_WIDTH	256  //rm
#define	TOP_SCREEN_HEIGHT	192

#define	TOP_SPRITE_WIDTH	32
#define	TOP_SPRITE_HEIGHT	64
*/

//using namespace graphics;

/**
 * @internal
 * @brief Console "pointer" --> used in consoleSelect(&screen) in order to select the top (or bottom) screen
 * 
 */
static PrintConsole topScreen;

/** 
 * @internal
 * @brief Max 6 players --> init+display the number needed
 */
static graphics::top::PlayerSprite player1, player2, player3,  player4,  player5,  player6;

/**
 * @internal
 * @brief Max 5 cards --> display flop, turn and river
 */
static graphics::CardSprite card1, card2, card3, card4, card5;

/**
 *	@internal
 *  @brief internal counter to keep track of sprites
 */
static int _count = 0;

/**
 * @internal
 * 
 * @brief Init a sprite representing a Player
 * 
 * @param player 
 * @param color 
 * @param x 
 * @param y 
 */
static inline void initPlayer(graphics::top::PlayerSprite *player, int color, int x, int y)
{
	player->gfx =  oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	player->count = _count++;
	player->color = color;
	player->x = x;
	player->y = y;
}

/**
 * @internal
 * 
 * @brief Init a sprite representing a card
 * 
 * @param card : CardSprite*
 * @param gfx : u8*
 * @param x : int
 * @param y : int
 */
static inline void initCard(graphics::CardSprite* card, u8* gfx, int x, int y)
{
	card->gfx = oamAllocateGfx(&oamMain, SpriteSize_32x64, SpriteColorFormat_256Color);
	card->frame_gfx = (u8*)gfx;
	card->count = _count++;
	card->x = x;
	card->y = y;
}

/**
 * @internal
 * 
 * @brief update the value of a card (change the card)
 * 
 * @param card : CardSprite*
 * @param cardState : CardState
 */
static inline void updateCard(graphics::CardSprite *card, CardState cardState) 
{
    u8* offset = card->frame_gfx + cardState * 32*64;
    dmaCopy(offset, card->gfx, 32*64);
}

/**
 * @internal
 * 
 * @brief Configure Sprites
 * 
 */
static void configureSprites() 
{
	//REG_POWERCNT = POWER_LCD | POWER_2D_A;
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_SPRITE;
	oamInit(&oamMain, SpriteMapping_1D_128, false);  //32

	initCard(&card1, (u8*)cardsTiles, 40, 67);
	initCard(&card2, (u8*)cardsTiles, 75, 67);
	initCard(&card3, (u8*)cardsTiles, 110, 67);
	initCard(&card4, (u8*)cardsTiles, 145, 67);
    initCard(&card5, (u8*)cardsTiles, 180, 67);

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

/**
 * @internal
 * 
 * @brief Display (or not) a given card
 * 
 * @param card : graphics::CardSprite
 * @param reveal : bool
 */
static void displayCard(graphics::CardSprite card, bool reveal) 
{
	oamSet(
		&oamMain, 						// oam handler
    	card.count,						// Number of sprite
    	card.x, card.y,					// Coordinates
    	0,								// Priority
    	0,								// Palette to use
    	SpriteSize_32x64,				// Sprite size
    	SpriteColorFormat_256Color,		// Color format
		card.gfx,						// Loaded graphic to display
    	-1,								// Affine rotation to use (-1 none)
    	false,							// Double size if rotating
		!reveal,						// Hide this sprite
    	false, false,					// Horizontal or vertical flip
    	false							// Mosaic
    );
}

/*
static void displayPlayer(graphics::top::PlayerSprite player) // OamState* screen
{
	dmaFillHalfWords(player.color, player.gfx, 32*32*2);
	oamSet(&oamMain, player.count, player.x, player.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, player.gfx, 0, true, false, false, false, false);
}
*/

/**
 * @brief Top Loading Screen
 * 
 */
void graphics::top::loading()
{
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	BGCTRL[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_64x32;
	swiCopy(loadingTiles, BG_TILE_RAM(1), loadingTilesLen/2);
	swiCopy(loadingPal, BG_PALETTE, loadingPalLen/2);
	swiCopy(&loadingMap[0], BG_MAP_RAM(0), loadingMapLen/2);
}

/**
 * @brief Configuration of top screen
 * 
 */
void graphics::top::configGraphics() 
{
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D;;
	vramSetBankA(VRAM_A_MAIN_BG);

	BGCTRL[2] = BG_MAP_BASE(0) | BG_TILE_BASE(3) | BG_32x32 | BG_COLOR_256 | BG_PRIORITY_0;
	BGCTRL[3] = BG_MAP_BASE(1) | BG_TILE_BASE(5) | BG_32x32 | BG_COLOR_256 | BG_PRIORITY_1;

	topScreen.font.pal[0] = RGB15(31,31,31);
	topScreen.font.convertSingleColor = true;
	consoleInit(&topScreen,0, BgType_Text4bpp, BgSize_T_256x256, 4,1, true, true);
	//printf("\x1b[%d;%dH\x1b[5m%s:%d %s", 7, 8, "Total Pot", 0, "BB");

	swiCopy(topTiles, BG_TILE_RAM(3), topTilesLen/2);
   	swiCopy(topPal, BG_PALETTE, topPalLen/2);
    swiCopy(topMap, BG_MAP_RAM(0), topMapLen/2);

	/*
	swiCopy(bkgTiles, BG_TILE_RAM(5), bkgTilesLen/2);
	swiCopy(bkgPal, BG_PALETTE, bkgPalLen/2);
	swiCopy(bkgMap, BG_MAP_RAM(1), bkgMapLen/2); 
	*/

	::configureSprites();
}

/**
 * @brief Display the flop (3 first community cards)
 * 
 * @param cardState 
 */
void graphics::top::displayFlop(CardState *cardState) 
{
	updateCard(&card1, cardState[0]);
	updateCard(&card2, cardState[1]);
	updateCard(&card3, cardState[2]);
	::displayCard(card1, true);
	::displayCard(card2, true);
	::displayCard(card3, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}
void graphics::top::displayFlop1(CardState cardState) 
{
	updateCard(&card1, cardState);
	::displayCard(card1, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}
void graphics::top::displayFlop2(CardState cardState) 
{
	updateCard(&card2, cardState);
	::displayCard(card2, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}
void graphics::top::displayFlop3(CardState cardState) 
{
	updateCard(&card3, cardState);
	::displayCard(card3, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}

/**
 * @brief Display the turn (4th card)
 * 
 * @param cardState 
 */
void graphics::top::displayTurn(CardState cardState) 
{
	updateCard(&card4, cardState);
	::displayCard(card4, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}
/**
 * @brief Display the river (5th/last card)
 * 
 * @param cardState 
 */
void graphics::top::displayRiver(CardState cardState) 
{
	updateCard(&card5, cardState);
	::displayCard(card5, true);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}


/**
 * @brief Clean the Top Screen (remove the cards)
 * 
 */
void graphics::top::clean() 
{
	oamClear(&oamMain, 0, 0);
	::displayCard(card1, false);
	::displayCard(card2, false);
	::displayCard(card3, false);
	::displayCard(card4, false);
	::displayCard(card5, false);
	swiWaitForVBlank();
	oamUpdate(&oamMain);
}

/**
 *  @brief Update Game informations on the top screen 
 *  + Total Pot 
 *  + Current bet informations
 *  + Each player's details (id, bankroll, current/ongoing bet)
 * 
 * @param players 
 * @param total_pot 
 * @param current_bet 
 */
void graphics::top::updateGraphics(const std::vector<Player*> players, const int total_pot, const int current_bet) 
{
 	consoleSelect(&topScreen);
	consoleClear();

	printf("\x1b[%d;%dH\x1b[7m%s:%d %s", 5, 9, "Total Pot", total_pot, "BB");
	printf("\x1b[%d;%dH\x1b[7m%s:%d %s", 17, 7, "Current Bet", current_bet, "BB");

	for(auto player : players) {
		// if(player->id == 0) continue;
		if(player->isPlaying) {
			iprintf("\x1b[%d;%dH\x1b[7m%s %d %s", 21, 7, "Player", player->id, "is playing");
		}
	}
	printf("\x1b[%d;%dH\x1b[2m%s:%d", 1, 0, "Player", players[1]->id);
	printf("\x1b[%d;%dH\x1b[2m%d %s", 2, 0, players[1]->bankroll, "BB");
	if(!players[1]->hasFolded){
		printf("\x1b[%d;%dH\x1b[2m%d %s", 3, 1, players[1]->currentBet, "BB");
	}
	
	printf("\x1b[%d;%dH\x1b[2m%s:%d", 1, 24, "Player", players[2]->id);
	printf("\x1b[%d;%dH\x1b[2m%d %s", 2, 24, players[2]->bankroll, "BB");
	if(!players[2]->hasFolded){
		printf("\x1b[%d;%dH\x1b[2m%d %s", 3, 25, players[2]->currentBet, "BB");
	}
	
	//oamUpdate(&oamMain);
	swiWaitForVBlank();
}