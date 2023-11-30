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
    game->numPlayers = 3; // getNumberOfPlayers();
    game->players = malloc(sizeof(Player) * game->numPlayers);

    for (int i = 0; i < game->numPlayers; i++) {
        game->players[i].bankroll = 1000; // = 10€
        game->players[i].currentBet = 0;
        game->players[i].hasFolded = false;
        game->players[i].isAllIn = false;
        game->players[i].isDealer = false;
        game->players[i].Time = 0;
    }
    game->ante = 0;
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
        shuffleDeck(game);
        // Deal cards to players
        dealCards(game);
        // Each player makes a move
        game->players[game->dealerIndex+1].currentBet = game->smallBlind;
        game->players[game->dealerIndex+1].bankroll -= game->smallBlind;
        game->players[game->dealerIndex+2].currentBet = game->bigBlind;
        game->players[game->dealerIndex+1].bankroll -= game->bigBlind;
        playersMove(game);

        dealFlop(game);
        playersMove(game);
        
        dealTurn(game);
        playersMove(game);

        dealRiver(game);
        playersMove(game);

        // Determine winner and handle pot
        Player* winner = handWinner(game);  // or just void handWinner(game);
        handlePot(winner);

        // Check if any players are out of money
        for(int i = 0; i < game->numPlayers; i++) {
            if(game->players[i].bankroll == 0) {  // or <= 0 but should never be < 0
                //removePlayer(game, i);
                //removePlayer(game, &game->players[i]);
                game->numPlayers -= 1;
            }
        }
        if (game->dealerIndex < game->numPlayers) {
            game->dealerIndex++;
        } else {
            game->dealerIndex = 0;
        }
    } 
}


// Shuffle the deck
void shuffleDeck(Game *game) {
    initDeck(*game->deck);
    for (int i = 0; i < 52; i++) {
        int j = rand() % 52;
        Card *temp = game->deck[i];
        game->deck[i] = game->deck[j];
        game->deck[j] = temp;
    }
}

void dealCards(Game *game) {
    // Deal cards to each player
}

void playersMove(Game *game) {
    // Handle each player's turn
}

void dealFlop(Game *game) {
    // Deal flop cards
}

void dealTurn(Game *game) {
    // Deal turn card
}

void dealRiver(Game *game) {
    // Deal river card
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









// Burn the top card from the deck
void burn(Card *deck, int *topCardIndex) {
    // Increment top card index to simulate burning
    (*topCardIndex)++;
}

// Deal the next card from the deck
Card nextCard(Card *deck, int *topCardIndex) {
    return deck[(*topCardIndex)++];
}

// Player takes their turn
void playerPlay(Player *player) {
    // Implement player's turn logic
}

// Move to the next player
void nextPlayer(Game *game, int *currentPlayerIndex) {
    (*currentPlayerIndex)++;
    if (*currentPlayerIndex >= game->numPlayers) {
        *currentPlayerIndex = 0;
    }
}

// Update the game pot
void updatePot(Game *game, double amount) {
    game->pot += amount;

}

// Update the game or player time
void updateTime(int *time) {
    // Implement time update logic
}

// Player folds
void foldPlayer(Player *player) {
    player->hasFolded = true;
}

// Player places a bet
void bet(Player *player, double amount) {
    player->currentBet += amount;
    player->bankroll -= amount;
    // You might also want to update the game pot here
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
