/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>
//#include "graphics_sub.h"
#include "graphics_top.h"
#include "graphics_bottom.h"
#include "game.h"

//#include <time.h>

int main(int argc, char** argv) {

	//Configure and fill BG0
    loadingTop();
    //initGame();
    Game* game = new Game(3);

	configGraphics_Top(); 
    configGraphics_Bottom();

	//Position
    while(1){
        game->startGame();
        //updateGraphics_Top();
        //updateGraphics_Bottom();

    }
    return 0;
}       


/*         
        displayFlop();
        displayTurn();
        displayRiver(); 
        */


