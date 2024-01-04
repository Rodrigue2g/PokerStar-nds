#include "player.h"
#include "cards.h"
#include "game.h"
#include "graphics_bottom.h"
#include "graphics_top.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Default Construct for a new Game object :: Defaults to 3 players
 * 
 */
Game::Game()
    : numPlayers(3), dealerIndex(0), topCardIndex(0), 
      total_pot(0.0), currentBet(0), ante(0), blind(10), smallBlind(5), bigBlind(10) {

    // Initialize deck
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            deck.push_back(new Card);
            deck[suit+rank]->suit = static_cast<Suit>(suit);
            deck[suit+rank]->rank = static_cast<Rank>(rank);
        }
    }

    // Initialize players
    for (int i = 0; i < numPlayers; ++i) {
        players.push_back(new Player);
        players[i]->id = i;
        players[i]->bankroll = 1000; // = 10€
        players[i]->currentBet = 0;
        players[i]->hasFolded = false;
        players[i]->isAllIn = false;
        players[i]->isDealer = false;
        players[i]->Time = 0;
    }

    players[dealerIndex]->isDealer = true;

}
/**
 * @brief Construct a new Game object with a custom number of players
 * 
 * @param numPlayers ! 2 < numPlayers < 7 ! 
 * + Min 3 players
 * + Max 6 players
 */
Game::Game(int numPlayers) // add condit° on numPlayers 
    : numPlayers(numPlayers), dealerIndex(0), topCardIndex(0), 
      total_pot(0.0), currentBet(0), ante(0), blind(10), smallBlind(5), bigBlind(10) {

    // Initialize deck
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            deck.push_back(new Card);
            deck[suit+rank]->suit = static_cast<Suit>(suit);
            deck[suit+rank]->rank = static_cast<Rank>(rank);
        }
    }

    // Initialize players
    //players.resize(numPlayers);
    for (int i = 0; i < numPlayers; ++i) {
        players.push_back(new Player);
        players[i]->id = i;
        players[i]->bankroll = 1000; // = 10€
        players[i]->currentBet = 0;
        players[i]->hasFolded = false;
        players[i]->isAllIn = false;
        players[i]->isDealer = false;
        players[i]->Time = 0;
    }

    players[dealerIndex]->isDealer = true;

}

Game::~Game() {
    for(auto elem : players) {
        delete elem;
        elem = nullptr;
    }

    for(auto elem : deck){
        delete elem;
        elem = nullptr;
    }
}

/**
 * @brief Start the game (loop on every hand)
 * + init players
 * + Deal players' cards 
 * + Wait for first player to make a move (bet, check or fold)
 * - same for next players
 * 
 * + Deal the Flop
 * + wait for first player to make a move (bet, check or fold)
 * - same for next players
 * 
 * + Deal next card (turn)
 * + wait for first player to make a move (bet, check or fold)
 * - same for next players
 * 
 * + Deal last card (river)
 * + wait for first player to make a move (bet, check or fold)
 * - same for next players
 * 
 * + Determine winner and add the total_pot to his bankroll
 * + Start new hand (loop back until there's only 1 player left)
 * 
 * @attention INTERUPTS:
 * + If the start key is toggled, trigger an interrupt and stop the game.
 * + If during a hand only one player reamins (everyone else folds)  --> GOTO END_OF_HAND ! 
 * - Not very good to use `goto` but needed a sort of interupt to jump at the end of the loop (and not out of it)
 * 
 */
void Game::startGame(){
    while(numPlayers > 1) { // and add 'start' key interrupt

START_HAND: // avoid using this goto; just here to structure code
        shuffleDeck();
        dealCards();  
        // maybe just display both at ones for every player 
        printI(0);  //rm
        displayCard1(CardState(players[0]->hand[0]->rank + players[0]->hand[0]->suit));
        printI(0);  //rm
        displayCard2(CardState(players[0]->hand[1]->rank + players[0]->hand[1]->suit));
        // Each player makes a move
        players[dealerIndex+1]->currentBet = smallBlind;
        players[dealerIndex+1]->bankroll -= smallBlind;
        players[dealerIndex+2]->currentBet = bigBlind;
        players[dealerIndex+2]->bankroll -= bigBlind;
        currentBet = bigBlind;
        total_pot = bigBlind + smallBlind;
        if(!playersMove()) goto END_OF_HAND;
        currentBet = 0;
        for(auto& player : players) player->currentBet = 0;

        dealFlop();
        if(!playersMove()) goto END_OF_HAND;
        currentBet = 0;
        for(auto& player : players) player->currentBet = 0;

        dealTurn();
        if(!playersMove()) goto END_OF_HAND;
        currentBet = 0;
        for(auto& player : players) player->currentBet = 0;

        dealRiver();
        if(!playersMove()) goto END_OF_HAND;

END_OF_HAND:
        // Determine winner and handle pot
        // Player* winner = handWinner(game);  // or just void handWinner(game);
        // handlePot(winner); 
        
        // Check if any players are out of money
        for(auto& player : players) {
            if(player->bankroll == 0) {  // or <= 0 but should never be < 0
                players.erase(players.begin() + player->id);
                numPlayers -= 1;
            }
        }

RESET_HAND: // avoid using this goto; just here to structure code
        // Next player is the dealer
        if (dealerIndex < numPlayers - 1) {
            players[dealerIndex]->isDealer = false;
            dealerIndex++;
        } else {
            players[dealerIndex]->isDealer = false;
            dealerIndex = 0;
        }
        players[dealerIndex]->isDealer = true;

        // Reset pot and current bet
        for(auto& player : players) player->currentBet = 0;
        currentBet = 0;
        total_pot = 0;
        //topCardIndex = 0;
        cleanTop();
        //printI(0);
    } 
}


/**
 * @brief init + shuffle the deck (memory must already be allocated)
 * 
 */
void Game::shuffleDeck() {
    for(int i = 0; i < 13; i++) {
        deck[i]->suit = SPADE;
        deck[i]->rank = Rank(i);
    }
    for(int i = 0; i < 13; i++) {
        deck[i+13]->suit = DIAMOND;
        deck[i+13]->rank = Rank(i);
    }
    for(int i = 0; i < 13; i++) {
        deck[i+26]->suit = HEART;
        deck[i+26]->rank = Rank(i);
    }
    for(int i = 0; i < 13; i++) {
        deck[i+39]->suit = CLUB;
        deck[i+39]->rank = Rank(i);
    }
    for (int i = 0; i < 52; i++) {
        int j = rand() % 51;
        Card *temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

/**
 * @brief Deal the next card from the deck
 * 
 * @return Card* 
 */
Card* Game::nextCard() {
    if (topCardIndex < 0 || topCardIndex > 51) topCardIndex = 0;
    return deck[topCardIndex++];
}

/**
 * @brief Deal 2 cards to each player
 * 
 */
void Game::dealCards() 
{
    for (int i = 0; i < numPlayers; i++) {
        players[i]->hand[0] = nextCard();
    }
    for (int i = 0; i < numPlayers; i++) {
        players[i]->hand[1] = nextCard();
    }
}

/**
 * @brief Deal flop cards (one by one or all at once?)
 * 
 */
void Game::dealFlop() {
    // Deal flop cards
    for(int i = 0; i < 3; i++) {
        communityCards[i] = nextCard();
        flop[i] = communityCards[i];
        if (i == 0) {
            displayFlop1(CardState(flop[i]->rank + flop[i]->suit));
        } else if (i == 1) {
            displayFlop2(CardState(flop[i]->rank + flop[i]->suit));
        } else if (i == 2) {
            displayFlop3(CardState(flop[i]->rank + flop[i]->suit));
        }
        printI((flop[i]->rank + flop[i]->suit));
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
void Game::dealTurn() {
    // Deal turn card
    communityCards[3] = nextCard(); 
    turn = communityCards[3];
    displayTurn(CardState(turn->rank + turn->suit));
}

/**
 * @brief Deal river card (5th/last card)
 * 
 */
void Game::dealRiver() {
    // Deal river card
    communityCards[4] = nextCard();
    river = communityCards[4];
    displayRiver(CardState(river->rank + river->suit));
}


/**
 * @brief Handle each player's turn
 * 
 * @param game 
 */
bool Game::playersMove() {
    int players_left = numPlayers;
    for(int i = 0; i < numPlayers; i++) {
        // wait for each player to make a move
        players[i]->isPlaying = true;
        updateGraphics_Top(players, total_pot, currentBet);

        if(players_left < 2)
            return false;

        bool validMove = false;
        while (!validMove) {
            Move res = waitForPlayerMove(players[i]);
            if(res.action == FOLD) {
                players[i]->hasFolded = true;
                --players_left;
                validMove = true;
            } else if(res.action == CHECK) {
                if(players[i]->currentBet < currentBet) {
                    // unableToCheck();
                    continue; // return to waitForPlayerMove() for this player!
                } else {
                    validMove = true;
                }
            } else if(res.action == CALL) {
                if(players[i]->bankroll < currentBet) {
                    players[i]->currentBet = players[i]->bankroll;
                    players[i]->bankroll = 0;
                    players[i]->isAllIn = true;

                    total_pot += currentBet;

                    validMove = true;
                } else {
                    players[i]->bankroll -= (currentBet - players[i]->currentBet);

                    total_pot += (currentBet - players[i]->currentBet);
                    
                    players[i]->currentBet = currentBet;

                    validMove = true;
                }
            } else if(res.action == RAISE && res.amount >= 2*currentBet) {
                if(players[i]->bankroll > res.amount) {
                    players[i]->currentBet = res.amount;
                    players[i]->bankroll -= res.amount;

                    currentBet = res.amount;
                    total_pot += currentBet;

                    validMove = true;
                } else if(players[i]->bankroll <= res.amount) {
                    players[i]->currentBet += players[i]->bankroll;
                    players[i]->isAllIn = true;
                    players[i]->bankroll = 0;
                    currentBet = players[i]->currentBet;
                    validMove = true;
                }
                //updatePot(this, currentBet);
            } else if(res.action == ALLIN) {
                players[i]->currentBet += players[i]->bankroll;
                players[i]->isAllIn = true;
                players[i]->bankroll = 0;
                currentBet = players[i]->currentBet;
                validMove = true;
            // updatePot(this, players[i]->bankroll);
            }
            //updateGraphics_Top(players, total_pot, currentBet);
        }
        players[i]->isPlaying = false;
        //update graphics
    }
    if(players_left < 2)
        return false;
    return true;
}

/**
 * @brief 
 * 
 * @param game 
 * @param player 
 * @return Move 
 */
Move Game::waitForPlayerMove(const Player *player)
{
    if(player->id == 0) {
        // ask graphics 
        return waitForLocalPlayerMove(currentBet, player->currentBet, player->bankroll);
    } else if(player->id == 100){ // change condition to network
        // Network call (multiplayer)
    } else {
        printI(0);
        // ai 
        if(currentBet == player->currentBet) {
            return (Move){CHECK, 0};
        }
        if(currentBet > 3*player->currentBet) {
            return (Move){FOLD, 0};
        }
        if(currentBet > player->currentBet && player->bankroll > currentBet) {
            return (Move){CALL, currentBet - player->currentBet};
        } else if(currentBet > player->currentBet && player->bankroll <= currentBet) {
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
/*
// Update each player's hand based on community cards
void updatePlayersHand(Game *game) {
    for (int i = 0; i < game->numPlayers; i++) {
        findBestHand(&game->players[i]);
    }
}
*/