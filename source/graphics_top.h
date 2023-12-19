
#ifndef GRAPHICS_MAIN_H_
#define GRAPHICS_MAIN_H_
#include "card.h"

typedef struct {
    int x, y;
	u16* gfx;
	int color;
    int count;
} PlayerSprite;

typedef struct 
{
	int x, y;

	u16* gfx;
	u8*  frame_gfx;

	int state;
    int count;
} CardSpriteTop;


void loadingTop();

// configuration of top screen
void configGraphics_Top();
void configureBG0_Top();
void configureBG1_Top();

static void configureSprites_Top();

void updateGraphics_Top();
void displayFlop(CardState* cardState);
void displayTurn(CardState cardState);
void displayRiver(CardState cardState);

void cleanTop();

static void displayCardTop(CardSpriteTop card, bool reveal);
static void displayPlayer(PlayerSprite player);

#endif /* GRAPHICS_MAIN_H_ */