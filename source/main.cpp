/**
 * @file main.cpp
 * @author Rodrigue de Guerre
 * @brief Entry point of Poker-Star nds game 
 * @version 0.1
 * @date 2024-01-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <nds.h>
#include <stdio.h>
#include "graphics_top.h"
#include "graphics_bottom.h"
#include "game.h"

using namespace graphics;


int main(int argc, char** argv) {

    enterCriticalSection();
    leaveCriticalSection(1);

    top::loading();
    //bottom::loading();
    bottom::printI(0); //rm
    
    // int players = bottom::getNbOfPlayers();
    Game* game = new Game();

	top::configGraphics(); 
    bottom::configGraphics();

    while(1){
        game->startGame();
        // Init a new game
    }

    return 0;
}
 