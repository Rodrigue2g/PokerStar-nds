
#ifndef GAME_H_
#define GAME_H_
#include "game.h"
#include "player.h"
#include "cards.h"

typedef struct Player {
    Card *hole[2];  //starting cards
    Card *hand[5];  // best hand with commuinty cards included
    
    bool isDealer;
    bool hasFolded;
    int *Time; //must be a time var ==> change later

    double bankroll;
    double currentBet;
    bool isAllIn;

} Player;

typedef struct {
    int numPlayers;
    Player *players;
    int dealerIndex;

    Card *deck[52];
    Deck *localDeck;  //Or initialize here if without a pointer

    double pot;

    int ante;
    int blind;
    int smallBlind;
    int bigBlind;

    Card *communityCards[5];
    Card *flop[3];
    Card *turn;
    Card *river;

    int *clock;
    int *time; // If a player wants more time ==> Maybe in Player only?

} Game;


void initGame();
void startGame();

// Shuffle the deck
void shuffleDeck(Game *game);
void dealCards(Game *game);

void dealFlop(Game *game);
void dealTurn(Game *game);
void dealRiver(Game *game);

void playersMove(Game *game);

Player *handWinner(Game *game);
void handlePot(Player *winner);

void removePlayer(Game *game, int playerIndex);


// Burn the top card from the deck
void burn(Card *deck, int *topCardIndex);

// Deal the next card from the deck
Card nextCard(Card *deck, int *topCardIndex);

// Player takes their turn
void playerPlay(Player *player);

// Move to the next player
void nextPlayer(Game *game, int *currentPlayerIndex);

// Update the game pot
void updatePot(Game *game, double amount);

// Update the game or player time
void updateTime(int *time);

// Player folds
void foldPlayer(Player *player);

// Player places a bet
void bet(Player *player, double amount);
// Determine the best hand for a player
void findBestHand(Player *player);

// Update each player's hand based on community cards
void updatePlayersHand(Game *game);


#endif /* GAME_H_ */