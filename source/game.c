#include "player.h"
#include "cards.h"
#include "game.h"
#include "graphics_bottom.h"
#include "graphics_top.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Game *game;

void initGame(){
    // Wait for user input to set the number of players
    game = malloc(sizeof(Game));
    game->numPlayers = 3; //getNumberOfPlayers(3);
    game->players = malloc(sizeof(Player) * game->numPlayers);

    game->topCardIndex = 0;
    for (int i = 0; i < 52; i++) {
        game->deck[i] = malloc(sizeof(Card));
        if (game->deck[i] == NULL) {
            // Handle memory allocation error
        }
    }

    for (int i = 0; i < game->numPlayers; i++) {
        game->players[i].id = i;
        game->players[i].bankroll = 1000; // = 10€
        game->players[i].currentBet = 0;
        game->players[i].hasFolded = false;
        game->players[i].isAllIn = false;
        game->players[i].isDealer = false;
        game->players[i].Time = 0;
    }
    game->ante = 0;
    game->dealerIndex = 0;  // or set by user ?
    game->players[game->dealerIndex].isDealer = true;

    game->ante = 0;
    game->blind = 10;
    game->smallBlind = 5;
    game->bigBlind = 10;
}

// init players
// deal cards 
// wait for first player to make a move (bet, check or fold)
// same for next players
// deal next card
// wait for first player to make a move (bet, check or fold)
// same for next players
// deal last card (flop)
// wait for first player to make a move (bet, check or fold)
// same for next players
// determine winner and add the prize pot to his pot

// Start new hand (same as above)
// If at some point a player has no more money, he is out of the game
// Once there is only one player remaining the game finishes 
// Or if the start key is toggled, then it triggers an interrupt and the game stops
void startGame(){
    while(game->numPlayers > 1) { // and add 'start' key interrupt
        shuffleDeck();
        // Deal cards to players
        dealCards();
        printI(0);
        displayCard1(game->players[0].hand[0]->rank + game->players[0].hand[0]->suit);
        printI(0);
        displayCard2(game->players[0].hand[1]->rank + game->players[0].hand[1]->suit);
        // Each player makes a move
        game->players[game->dealerIndex+1].currentBet = game->smallBlind;
        game->players[game->dealerIndex+1].bankroll -= game->smallBlind;
        game->players[game->dealerIndex+2].currentBet = game->bigBlind;
        game->players[game->dealerIndex+1].bankroll -= game->bigBlind;
        //playersMove(&game);
        printI(0);
        dealFlop();
        //playersMove(game);
        //printI(0);

        dealTurn();
        //playersMove(game);
        printI(0);

        dealRiver();
        //playersMove(game);
        printI(0);
        // Determine winner and handle pot
        // Player* winner = handWinner(game);  // or just void handWinner(game);
       // handlePot(winner); 

        // Check if any players are out of money
        for(int i = 0; i < game->numPlayers; i++) {
            if(game->players[i].bankroll == 0) {  // or <= 0 but should never be < 0
                //removePlayer(game, i);
                //removePlayer(game, &game->players[i]);
                game->numPlayers -= 1;
            }
        }
        if (game->dealerIndex < game->numPlayers - 1) {
            game->players[game->dealerIndex].isDealer = false;
            game->dealerIndex++;
        } else {
            game->players[game->dealerIndex].isDealer = false;
            game->dealerIndex = 0;
        }
        game->players[game->dealerIndex].isDealer = true;
        game->currentBet = 0;
        game->pot = 0;
        //game->topCardIndex = 0;
        cleanTop();
        //printI(0);
    } 
}


/**
 * @brief init + shuffle the deck (memory must already be allocated)
 * 
 */
void shuffleDeck() {
    for(int i = 0; i < 13; i++) {
        game->deck[i]->suit = SPADE;
        game->deck[i]->rank = i;
    }
    for(int i = 0; i < 13; i++) {
        game->deck[i+13]->suit = DIAMOND;
        game->deck[i+13]->rank = i;
    }
    for(int i = 0; i < 13; i++) {
        game->deck[i+26]->suit = HEART;
        game->deck[i+26]->rank = i;
    }
    for(int i = 0; i < 13; i++) {
        game->deck[i+39]->suit = CLUB;
        game->deck[i+39]->rank = i;
    }
    for (int i = 0; i < 52; i++) {
        int j = rand() % 51;
        Card *temp = game->deck[i];
        game->deck[i] = game->deck[j];
        game->deck[j] = temp;
    }
}

/**
 * @brief Deal the next card from the deck
 * 
 * @return Card* 
 */
Card *nextCard() {
    if (game->topCardIndex < 0 || game->topCardIndex > 51) game->topCardIndex = 0;
    return game->deck[game->topCardIndex++];
}

/**
 * @brief Deal 2 cards to each player
 * 
 */
void dealCards() 
{
    for (int i = 0; i < game->numPlayers; i++) {
        game->players[i].hand[0] = nextCard();
    }
    for (int i = 0; i < game->numPlayers; i++) {
        game->players[i].hand[1] = nextCard();
    }
}

/**
 * @brief Deal flop cards (one by one or all at once?)
 * 
 */
void dealFlop() {
    // Deal flop cards
    for(int i = 0; i < 3; i++) {
        game->communityCards[i] = nextCard();
        game->flop[i] = game->communityCards[i];
        if (i == 0) {
            displayFlop1(game->flop[i]->rank + game->flop[i]->suit);
        } else if (i == 1) {
            displayFlop2(game->flop[i]->rank + game->flop[i]->suit);
        } else if (i == 2) {
            displayFlop3(game->flop[i]->rank + game->flop[i]->suit);
        }
        printI((game->flop[i]->rank + game->flop[i]->suit));
    }
    /* displayFlop(&(CardState[]){(game->flop[0]->rank + game->flop[0]->suit), 
                               (game->flop[1]->rank + game->flop[1]->suit),
                               (game->flop[2]->rank + game->flop[2]->suit)}); */
    //printI((game->flop[0]->rank + game->flop[0]->suit));
}

/**
 * @brief Deal turn card (4th card)
 * 
 */
void dealTurn() {
    // Deal turn card
    game->communityCards[3] = nextCard(); 
    game->turn = game->communityCards[3];
    displayTurn(game->turn->rank + game->turn->suit);
}

/**
 * @brief Deal river card (5th/last card)
 * 
 */
void dealRiver() {
    // Deal river card
    game->communityCards[4] = nextCard();
    game->river = game->communityCards[4];
    displayRiver(game->river->rank + game->river->suit);
}


/**
 * @brief Handle each player's turn
 * 
 * @param game 
 */
void playersMove(Game *game) {
    for(int i = 0; i < game->numPlayers; i++) {
        // wait for each player to make a move  
        Move res = waitForPlayerMove(*game, &game->players[i]);
        if(res.action == FOLD) {
            game->players[i].hasFolded = true;
            break;
        } else if(res.action == CHECK) {
            if(game->players[i].currentBet < game->currentBet) {
                // unableToCheck();
                continue; // return to waitForPlayerMove() for this player!
            } else {
                break;
            }
        } else if(res.action == CALL) {
            if(game->players[i].bankroll < game->currentBet) {
                game->players[i].isAllIn = true;
                game->players[i].currentBet += game->players[i].bankroll;
                game->players[i].bankroll = 0;
            } else {
                game->players[i].bankroll -= game->currentBet;
                game->players[i].currentBet = game->currentBet;
            }
            //updatePot(game, game->currentBet);
        } else if(res.action == RAISE && res.amount >= game->currentBet*2) {
            if(game->players[i].bankroll > res.amount) {
                game->players[i].currentBet += res.amount;
                game->players[i].bankroll -= res.amount;
                game->currentBet = res.amount;
            } else if(game->players[i].bankroll <= res.amount) {
                game->players[i].currentBet += game->players[i].bankroll;
                game->players[i].isAllIn = true;
                game->players[i].bankroll = 0;
                game->currentBet = game->players[i].currentBet;
            }
            //updatePot(game, game->currentBet);
        } else if(res.action == ALLIN) {
            game->players[i].currentBet += game->players[i].bankroll;
            game->players[i].isAllIn = true;
            game->players[i].bankroll = 0;
            game->currentBet = game->players[i].currentBet;
           // updatePot(game, game->players[i].bankroll);
        }
        //update graphics
    }
}

/**
 * @brief 
 * 
 * @param game 
 * @param player 
 * @return Move 
 */
Move waitForPlayerMove(const Game game, Player *player)
{
    if(player->id == 0) {
        // ask graphics
        return waitForLocalPlayerMove();
    } else {
        // ai 
        if(game.currentBet == player->currentBet) {
            return (Move){CHECK, 0};
        }
        if(game.currentBet > 3*player->currentBet) {
            return (Move){FOLD, 0};
        }
        if(game.currentBet > player->currentBet && player->bankroll > game.currentBet) {
            return (Move){CALL, game.currentBet - player->currentBet};
        } else if(game.currentBet > player->currentBet && player->bankroll <= game.currentBet) {
            return (Move){ALLIN, player->bankroll};
        }
    }
}




Player *handWinner(Game *game) {
    // Determine the winner of the hand
    // Return a pointer to the winning player
}

void handlePot(Player *winner) {
    // Handle the pot and award it to the winner
}

void removePlayer(Game *game, int playerIndex) {
    // Remove a player from the game
}




// Update the game or player time
void updateTime(int *time) {
    // Implement time update logic
}

// Determine the best hand for a player
void findBestHand(Player *player) {
    // Implement hand evaluation logic
}

// Update each player's hand based on community cards
void updatePlayersHand(Game *game) {
    for (int i = 0; i < game->numPlayers; i++) {
        findBestHand(&game->players[i]);
    }
}
