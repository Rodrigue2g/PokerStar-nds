#ifndef GRAPHICS_MAIN_H_
#define GRAPHICS_MAIN_H_

// configuration of top screen
void configGraphics_Top();
void configureBG0_Top();
void configureBG1_Top();
void configureSprites_Top();

void updateGraphics_Top();


typedef struct {
	u16* gfx;
	int color;
    //int number;
	int x, y;
} Sprite;

typedef struct {
	u16* gfx;
    const unsigned short* pal;
    int palLen;
    const unsigned int* tiles;
    int tilesLen;
	int x, y;
} _Sprite;

void displayCard(Sprite card, bool reveal, int count);
void createPlayer(Sprite sprite, OamState* screen, int count);


#endif /* GRAPHICS_MAIN_H_ */