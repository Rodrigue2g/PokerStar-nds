
#ifndef GAME_H_
#define GAME_H_
#include "game.h"
#include "player.h"
#include "card.h"

typedef struct {
    unsigned int id;
    char *name;

    Card *hole[2];  //starting cards
    Card *hand[5];  // best hand with commuinty cards included
    
    bool isDealer;
    bool hasFolded;  //or  bool isIn; ?           
    int *Time; //must be a time var ==> change later

    double bankroll;
    double currentBet;
    bool isAllIn;

} Player;

typedef struct {
    int numPlayers;
    Player *players;
    int dealerIndex;

    int topCardIndex;
    Card *deck[52];
    Deck *localDeck;  //Or initialize here if without a pointer

    double pot;
    int currentBet;

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

/**
 * @brief initialize the game
 * 
 *  number of players + other settings
 * 
 */
void initGame();

/**
 * @brief Start the game loop
 * 
 *  + display the game owner's cards
 *  + display the community cards
 *  + display the pot
 *  + display the players' bankroll
 * 
 *  - Loop for each hand until only one player remains
 *  - Calls other functions to handle each step of the game
 * 
 */
void startGame();

/**
 * @brief Shuffle the deck
 * 
 */
void shuffleDeck();

/**
 * @brief Deal the next card from the deck
 * 
 * @return Card* 
 */
Card *nextCard();

/**
 * @brief Deal each player 2 cards
 * + display Game owner's cards
 * 
 */
void dealCards();

/**
 * @brief Deal the community cards (flop, turn, river)
 * 
 *  + Burn the top card from the deck ?
 * 
 */
void dealFlop();
void dealTurn();
void dealRiver();

/**
 * @brief Each player makes their move
 * 
 */
void playersMove(Game *game);
/**
 * @brief Wait for the player to make a move (graphoics link)
 * 
 *  Graphics for local player
 *  Automated for AI/remote players
 * 
 *  Link to other players? send/recieve data over wifi
 * 
 * @param game 
 * @param player 
 * @return Move 
 */
Move waitForPlayerMove(const Game game, Player *player);


// MARK: - @TODO to be implemented

Player *handWinner(Game *game);
void handlePot(Player *winner);

void removePlayer(Game *game, int playerIndex);

// Update the game or player time
void updateTime(int *time);

// Determine the best hand for a player
void findBestHand(Player *player);

// Update each player's hand based on community cards
void updatePlayersHand(Game *game);


#endif /* GAME_H_ */