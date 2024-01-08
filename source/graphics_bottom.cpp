/**
 * @file graphics_bottom.cpp
 * @author Rodrigue de Guerre
 * @brief 
 * @version 0.1
 * @date 2024-01-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
// - nds/c++ libraries
#include <nds.h>
#include <stdio.h>
// - headers
#include "graphics_bottom.h"
#include "card.h"
// - grit headers
#include "homeBottom.h"
#include "choice.h"
#include "bottom.h"
#include "cards.h"


/**
 * @internal
 * @brief Console "pointer" --> used in consoleSelect(&screen) in order to select the bottom (or top) screen
 * 
 */
static PrintConsole bottomScreen;

/**
 * @internal
 * @brief Each Player has 2 cards in his hand
 */
static graphics::CardSprite hand1, hand2;

/**
 *  @internal
 *  @brief internal counter to keep track of sprites
 */
static int _count = 0;

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
	card->gfx = oamAllocateGfx(&oamSub, SpriteSize_32x64, SpriteColorFormat_256Color);
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
static inline void updateCard(graphics::CardSprite* card, CardState cardState) 
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
	VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE;
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	initCard(&hand1, (u8*)cardsTiles, 30, 80);
	initCard(&hand2, (u8*)cardsTiles, 65, 80);

	dmaCopy(cardsPal, SPRITE_PALETTE_SUB, cardsPalLen);
}	

/**
 * @internal
 * 
 * @brief Display (or not) a given card
 * 
 * @param card : graphics::CardSprite
 * @param reveal : bool
 */
static void displayCard(graphics::CardSprite card, bool fold) 
{
	oamSet(
		&oamSub, 						// oam handler
    	card.count,						// Number of sprite
    	card.x, card.y,					// Coordinates
    	0,								// Priority
    	0,								// Palette to use
    	SpriteSize_32x64,				// Sprite size
    	SpriteColorFormat_256Color,		// Color format
		card.gfx,						// Loaded graphic to display
    	-1,								// Affine rotation to use (-1 none)
    	false,							// Double size if rotating
		fold,							// Hide this sprite
    	false, false,					// Horizontal or vertical flip
    	false							// Mosaic
    );
}

/**
 * @brief Define the methods for the specification of core graphics namespace for the bottom screen
 * 
 */
using namespace graphics;

bool bottom::isOnlineGame()
{
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG2_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	BGCTRL_SUB[2] = BG_COLOR_256 | BG_MAP_BASE(2) | BG_TILE_BASE(5) | BG_32x32;

	consoleInit(&bottomScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 4, 1, false, true);

	swiCopy(homeBottomTiles, BG_TILE_RAM_SUB(5), homeBottomTilesLen/2);
	swiCopy(homeBottomPal, BG_PALETTE_SUB, homeBottomPalLen/2);
	swiCopy(homeBottomMap, BG_MAP_RAM_SUB(2), homeBottomMapLen/2);


	int isOnline = false; //solo
	bool isok = false;
	consoleSelect(&bottomScreen);
	touchPosition touch;
	while(!isok) {
		swiWaitForVBlank();
		scanKeys();
		int keys = keysDown();
		if(keys & KEY_TOUCH) {
			touchRead(&touch);
			if(touch.px>36 && touch.px<99 && touch.py>95 && touch.py<158) {
				isOnline = true;
				isok = true;
			} else if(touch.px>175 && touch.px<205 && touch.py>95 && touch.py<155){
				isOnline = false;
				isok = true;
			}
		}
		if(keys & KEY_LEFT) {
			isOnline = true;
			consoleClear();
			printf("\x1b[%d;%dH\x1b[37m%s", 20, 5, "ONLINE");
		}
		if(keys & KEY_RIGHT) {
			isOnline = false;
			consoleClear();
			printf("\x1b[%d;%dH\x1b[37m%s", 20, 22, "SOLO");
		}
		if(keys & KEY_A) {
			isok = true;
		}
		if(keys & KEY_B) {
			isOnline = false;
			isok = true;
		}
	}
	return isOnline;
}
bool bottom::isHost()
{
	swiCopy(choiceTiles, BG_TILE_RAM_SUB(5), choiceTilesLen/2);
	swiCopy(choicePal, BG_PALETTE_SUB, choicePalLen/2);
	swiCopy(choiceMap, BG_MAP_RAM_SUB(2), choiceMapLen/2);

	bool isHost = true; //solo
	bool isok = false;
	//consoleSelect(&bottomScreen);
	touchPosition touch;
	while(!isok) {
		swiWaitForVBlank();
		consoleClear();
		scanKeys();
		int keys = keysDown();
		if(keys & KEY_TOUCH) {
			touchRead(&touch);
			if(touch.px>36 && touch.px<210 && touch.py>31 && touch.py<81) {
				isHost = true;
				isok = true;
			} else if(touch.px>50 && touch.px<210 && touch.py>112 && touch.py<169){
				isHost = false;
				isok = true;
			}
		}
		if(keys & KEY_UP) {
			isHost = true;
			isok = true;
		}
		if(keys & KEY_DOWN) {
			isHost = false;
			isok = true;
		}
	}
	return isHost;
}

/**
 * @brief Configuration of the bottom screen
 * 
 */
void bottom::configGraphics() 
{
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG2_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	BGCTRL_SUB[2] = BG_COLOR_256 | BG_MAP_BASE(2) | BG_TILE_BASE(5) | BG_32x32;

	consoleInit(&bottomScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 4, 1, false, true);

	swiCopy(bottomTiles, BG_TILE_RAM_SUB(5), bottomTilesLen/2);
	swiCopy(bottomPal, BG_PALETTE_SUB, bottomPalLen/2);
	swiCopy(bottomMap, BG_MAP_RAM_SUB(2), bottomMapLen/2);

	consoleClear();
	swiWaitForVBlank();
	printf("\x1b[%d;%dH\x1b[37m%d %s", 21, 24, 10, "BB");

	configureSprites();
}

/**
 * @brief Display the hand (2 cards) of the local player
 * 
 * @param cardState 
 */
void bottom::displayHand(CardState* cardState)  //rm ? or add time here?
{
	updateCard(&hand1, cardState[0]);
	updateCard(&hand2, cardState[1]);
	displayCard(hand1, false);
	displayCard(hand2, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}
void bottom::displayCard1(CardState cardState) 
{
	updateCard(&hand1, cardState);
	displayCard(hand1, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}
void bottom::displayCard2(CardState cardState) 
{
	updateCard(&hand2, cardState);
	displayCard(hand2, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

/**
 * @brief Fold the hand of the local player
 * 
 */
void bottom::fold()
{
	displayCard(hand1, true);
	displayCard(hand2, true);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}
/**
 * @brief Remove the cards of the local player (if he's out of money/doesn't paly)
 * 
 */
void bottom::rmCards(){}


/**
 * @brief Get the number of Players before starting a game
 * 
 * @param numPlayers 
 * @return int 
 */
int bottom::getNbOfPlayers(int numPlayers)  //update with custom console instead
{
	// Wait for user input to set the number of players
    int _numPlayers = numPlayers;
	bool isok = false;
	consoleSelect(&bottomScreen);
	while(!isok) {
		swiWaitForVBlank();
		consoleClear();
		scanKeys();
		int keys = keysDown();
		iprintf("\nNumber of players: %u", _numPlayers);
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

/**
 * @internal
 * @brief Internal blind counter (to be changed by the local player)
 * 
 */
static int bb = 10;

/**
 * @internal
 * @brief 
 * 
 * @param bb 
 * @param current_bet 
 * @param bankroll 
 */
static void printParams(const int current_bet, const int bankroll){
	/* cc->cursorX = 2;
	cc->cursorY = 2; */
	//iprintf("\x1bCurrent Bet : %d", 10);
	printf("\x1b[%d;%dH\x1b[20m%s : %d", 1, 1, "Current Bet", current_bet);

	printf("\x1b[%d;%dH\x1b[37m%s : %d", 3, 1, "Bankroll", bankroll);

	printf("\x1b[%d;%dH\x1b[37m%d %s", 21, 24, bb, "BB");

	swiWaitForVBlank();
}

/**
 * @brief Update the bottom screen
 * 
 * @param player 
 */
void bottom::updateGraphics(const Player* player) 
{
	consoleSelect(&bottomScreen);
	consoleClear();
	 	
	printf("\x1b[%d;%dH\x1b[20m%s : %d", 1, 1, "Current Bet", player->currentBet);
	printf("\x1b[%d;%dH\x1b[37m%s : %d", 3, 1, "Bankroll", player->bankroll);
	printf("\x1b[%d;%dH\x1b[37m%d %s", 21, 24, bb, "BB");
	
	swiWaitForVBlank();
}

/**
 * @brief Wait for the local player to make a move
 * 
 * @param player 
 * @param current_bet 
 * @return Move 
 */
Move bottom::waitForLocalPlayerMove(const Player* player, const int current_bet)
{
	consoleSelect(&bottomScreen);
	Move move;
	bool isok = false;
	touchPosition touch;
	while(!isok) {
		swiWaitForVBlank();
		scanKeys();
		int keys = keysDown();
		// int held = keysHeld();
		touchRead(&touch);

		/**
		 * @brief +/- buttons (blind amount)
		 * 
		 *  @param keysDown
		 *  @param touchPosition
		 */
		if((keys & KEY_TOUCH) && touch.px>135 && touch.px<172) {
			if(touch.py>60 && touch.py<110) {
				//++bb;
				bb += 5;
			} else if(touch.py>110 && touch.py<157){
				//--bb;
				bb -= 5;
				if(bb < 0) bb = 0;
			}
		}
		/**
		 * @brief FOLD, CHECK, BB buttons
		 * 
		 *  @param keysDown
		 * 	@param touchPosition
		 */
		if((keys & KEY_TOUCH) && touch.py>164 && touch.py<190) {
			if(touch.px>5 && touch.px<82) {  //FOLD
				move.action = FOLD;
				isok = true;
			} else if(touch.px>92 && touch.px<165) {  //CHECK
				if (player->currentBet == current_bet) {
					move.action = CHECK;
					isok = true;
				} else {
					// handle error
				}
			} else if(touch.px>175 && touch.px<250) {  //BB
				if(bb == current_bet) {
					move.action = CALL;
					//move.amount = bb;
					printf("\x1b[%d;%dH\x1b[20m%s : %d", 1, 1, "Current Bet", current_bet);
					//printf("\x1b[%d;%dH\x1b[37m%s : %d", 3, 1, "Bankroll", bb);
					isok = true;
				} else if(bb >= 2*current_bet) {
					move.action = RAISE;
					move.amount = bb;
					isok = true;
				} else {
					// handle error
				}
			}
		}
		/**
		 * @brief ALL-IN, 40, 30, 20 BB butttons
		 * 
		 * @param keysDown
		 * @param touchPosition
		 */
		if((keys & KEY_TOUCH) && touch.px>176 && touch.px<247) {
			if(touch.py>58 && touch.py<80) {  //ALL IN
				
			} else if(touch.py>85 && touch.py<109){  //40BB
			
			} else if(touch.py>111 && touch.py<134){  //30BB
			
			} else if(touch.py>138 && touch.py<160){  //20BB
			
			} 
		}

		/**
		 * @brief Arrow buttons
		 * 
		 * @param keysDown
		 */
		if(keys & KEY_UP) {
			bb += 5;
		}
		if(keys & KEY_DOWN) {
			bb -= 5;
			if(bb < 0) bb = 0;
		}
		if(keys & KEY_RIGHT) {
			
		}
		if(keys & KEY_LEFT) {
			
		}
		/**
		 * @brief A, B, X, Y butttons 
		 * 
		 * @param keysDown
		 */
		if(keys & KEY_A) {
			if(player->currentBet == current_bet) {
				move.action = CHECK;
				isok = true;
			} else if (player->currentBet < current_bet && bb < 2*current_bet) {
				move.action = CALL;
				move.amount = current_bet;
				isok = true;
			} else if (bb > 2*current_bet) {
				move.action = RAISE;
				move.amount = bb;
				isok = true;
			}
		}
		if(keys & KEY_B) {
			move.action = FOLD;
			isok = true;
		}

		/**
		 * @brief Clear the console then update with current params
		 * 
		 */
		consoleClear();
		printParams(player->currentBet, player->bankroll);
		swiWaitForVBlank();
	}
	return move;
}

void bottom::wifiInfo()
{
	consoleSelect(&bottomScreen);
	consoleClear();	
	printf("\x1b[%d;%dH\x1b[37m%s", 8, 8, "Connecting to wifi...");
	swiWaitForVBlank();
}

void bottom::wifiInfo(const char* str)
{
	consoleSelect(&bottomScreen);	
	printf("\x1b[%d;%dH\x1b[37m%s", 10, 13, str);
	swiWaitForVBlank();
}
void bottom::socketInfo(const char* str)
{
	consoleSelect(&bottomScreen);	
	printf("\x1b[%d;%dH\x1b[37m%s", 12, 13, str);
	swiWaitForVBlank();
}
void bottom::stateInfo(const char* str)
{
	consoleSelect(&bottomScreen);
	printf("\x1b[%d;%dH\x1b[37m%s", 3, 11, str);
	swiWaitForVBlank();
}


void bottom::nextStep() {  //rm after tests --> or modify
	bool isok = false;
	while(!isok) {
		scanKeys();
		int keys = keysDown();
		if(keys & KEY_RIGHT) {
			isok = true;
		}
	}
}