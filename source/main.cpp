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
// - nds/c++ libraries
#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>
// - headers
#include "graphics_top.h"
#include "graphics_bottom.h"
#include "game.h"
// - soundbank headers
#include "soundbank.h"
#include "soundbank_bin.h"

using namespace graphics;


int main(int argc, char** argv) {

    enterCriticalSection();
    leaveCriticalSection(1);
    
    mmInitDefaultMem((mm_addr)soundbank_bin);
    mmSetModuleTempo(512);
	mmLoad(MOD_007);
    mmLoad(MOD_BILIJEAN);

	mmStart(MOD_007, MM_PLAY_LOOP);

    top::loading();
    bool online = bottom::isOnlineGame();
    if(online) {
        bottom::wifiInfo();
        if(initWiFi())
            bottom::wifiInfo("Wifi Ok");
        else 
            bottom::wifiInfo("Wifi error");
        if(openSocket())
            bottom::wifiInfo("Socket Ok");
        else 
            bottom::wifiInfo("Socket not Ok");
        bottom::nextStep();
        bool isHost = bottom::isHost();
        mmStop();
        mmStart(MOD_BILIJEAN, MM_PLAY_LOOP);
        if(isHost) {
            // int players = bottom::getNbOfPlayers();
            Game* game = new Game(3, true, true);
            top::configGraphics(); 
            bottom::configGraphics();
            while(1){
                game->startGame(); // Init a new game
            }
            delete game;
            game = nullptr;
        } else {
            Game* game = new Game();
            top::configGraphics(); 
            bottom::configGraphics();
            while(1){
                game->joinGame();
            }
            delete game;
            game = nullptr;
        }
        mmStop();
    } else {
        mmStop();
        mmStart(MOD_BILIJEAN, MM_PLAY_LOOP);

        Game* game = new Game();

        top::configGraphics(); 
        bottom::configGraphics();

        while(1){
            game->startGame();
            // Init a new game
        }
        delete game;
        game = nullptr;
        mmStop();
    }
    return 0;
}
 