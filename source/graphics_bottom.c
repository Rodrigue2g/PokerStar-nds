
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


PrintConsole bottomScreen;

void configGraphics_Bottom() {
	//REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	//REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG0_ACTIVE;
	configureBG0_Bottom();
	configureSprites_Bottom();
	//updateGraphics_Bottom();
}

void configureBG0_Bottom() {

	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG2_ACTIVE;
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	BGCTRL_SUB[2] = BG_COLOR_256 | BG_MAP_BASE(2) | BG_TILE_BASE(5) | BG_32x32;

	consoleInit(&bottomScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 4, 1, false, true);

	swiCopy(bottomTiles, BG_TILE_RAM_SUB(5), bottomTilesLen/2);
	swiCopy(bottomPal, BG_PALETTE_SUB, bottomPalLen/2);
	swiCopy(bottomMap, BG_MAP_RAM_SUB(2), bottomMapLen/2);

	consoleClear();
	swiWaitForVBlank();
	printf("\x1b[%d;%dH\x1b[37m%s %d", 3, 12, "Player", 1);
	printf("\x1b[%d;%dH\x1b[37m%d %s", 21, 24, 10, "BB");
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

	initCard(&hand1, (u8*)cardsTiles, 30, 80);
	initCard(&hand2, (u8*)cardsTiles, 65, 80);

	dmaCopy(cardsPal, SPRITE_PALETTE_SUB, cardsPalLen);
}	

void displayHand(CardState* cardState)  //rm ? or add time here?
{
	updateCard(&hand1, cardState[0]);
	updateCard(&hand2, cardState[1]);
	displayCard(hand1, false);
	displayCard(hand2, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void displayCard1(CardState cardState) 
{
	updateCard(&hand1, cardState);
	displayCard(hand1, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void displayCard2(CardState cardState) 
{
	updateCard(&hand2, cardState);
	displayCard(hand2, false);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void fold()
{
	displayCard(hand1, true);
	displayCard(hand2, true);
	swiWaitForVBlank();
	oamUpdate(&oamSub);
}

void rmCards(){}

void displayCard(CardSpriteBottom card, bool fold) 
{
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

int getNbOfPlayers(int numPlayers)  //update with custom console instead
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

void printI(int i) {  //rm after tests --> or modify
	bool isok = false;
	//consoleDemoInit();
	while(!isok) {
		//swiWaitForVBlank();
		//consoleClear();
		scanKeys();
		int keys = keysDown();

		//iprintf("\nNumber of players: %u", i);
		if(keys & KEY_RIGHT) {
			isok = true;
		}
		if(keys & KEY_DOWN) {

		}
		if(keys & KEY_A) {
			//isok = true;
		}
		if(keys & KEY_B) {
			//isok = true;
		}
	}
}

static inline void printXY(const int x, const int y, const char *text) //rm ?
{
    printf("\x1b[%d;%dH%s", y, x, text);
}
static void printColoredTextAtPos(const char *text, int colorCode, int row, int col) { //rm
    //printf("\x1b[%d;%dH\x1b[38;5;%dm%s\x1b[0m", row, col, colorCode, text);
	printf("\x1b[%d;%dH\x1b[15m%s", row, col, text);

}


//void updateGraphics_Bottom(const Player* player, const int current_bet) 
void updateGraphics_Bottom()
{
	/* 
	printf("\x1b[%d;%dH\x1b[20m%s : %d", 1, 1, "Current Bet", current_bet);
	printf("\x1b[%d;%dH\x1b[37m%s : %d", 3, 1, "Bankroll", bankroll);
	printf("\x1b[%d;%dH\x1b[37m%d %s", 21, 24, bb, "BB");
 	*/
	swiWaitForVBlank();
}

static void printParams(const int bb, const int current_bet, const int bankroll){
	/* cc->cursorX = 2;
	cc->cursorY = 2; */
	//iprintf("\x1bCurrent Bet : %d", 10);
	printf("\x1b[%d;%dH\x1b[20m%s : %d", 1, 1, "Current Bet", current_bet);

	printf("\x1b[%d;%dH\x1b[37m%s : %d", 3, 1, "Bankroll", bankroll);

	printf("\x1b[%d;%dH\x1b[37m%d %s", 21, 24, bb, "BB");

	swiWaitForVBlank();
}

//Move waitForLocalPlayerMove(const Player* player, const int current_bet)
Move waitForLocalPlayerMove(const int current_bet, const int player_bet, const int player_bankroll)
{
	bool isok = false;
	//consoleInit(0,0, BgType_Text4bpp, BgSize_T_256x256, 0, 1, false, true);
	//consoleInit(0,0, BgType_Text4bpp, BgSize_T_256x256, 4, 1, true, true);
	Move move;
	int bb = 10;
	touchPosition touch;
	consoleSelect(&bottomScreen);
	while(!isok) {
		scanKeys();
		int keys = keysDown();

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
				if (player_bet == current_bet) {
					move.action = CHECK;
					isok = true;
				} else {
					// handle error
				}
			} else if(touch.px>175 && touch.px<250) {  //BB
				if(bb == current_bet) {
					move.action = CALL;
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
			if(player_bet == current_bet) {
				move.action = CHECK;
				isok = true;
			} else if (player_bet < current_bet && bb < 2*current_bet) {
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
		printParams(bb, player_bet, player_bankroll);
		swiWaitForVBlank();
	}
	return move;
}