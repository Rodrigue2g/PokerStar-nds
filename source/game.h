
#ifndef GAME_H_
#define GAME_H_
#include <iostream>
#include <vector>

#include "card.h"

/* 
struct Player {
    unsigned int id;
    char *name;

    std::vector<Card*> hole;  //Card *hole[2];  //starting cards
    std::vector<Card*> hand;  //Card *hand[5];  // best hand with commuinty cards included
    
    bool isDealer;
    bool hasFolded;  //or  bool isIn; ?           
    int *Time; //must be a time var ==> change later

    int bankroll;
    int currentBet;
    bool isAllIn;

};
*/
class Game {
public:
    Game();
    Game(int numPlayers);
    virtual ~Game();

    /**
     * @brief initialize the game
     * 
     *  number of players + other settings
     * 
     */
    // void initGame();

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
protected:
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
     * @returns bool : Do we keep playing this hand? (if only 1 player remains, he wins this hand and we skip the others steps) 
     */
    bool playersMove();
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
    Move waitForPlayerMove(const Player *player);

private:
    int numPlayers;
    std::vector<Player*> players;
    int dealerIndex;

    int topCardIndex;
    Deck deck;  // std::vector<Card*>

    double total_pot;
    int currentBet;

    int ante;    //rm
    int blind;  //rm
    int smallBlind;
    int bigBlind;

    std::vector<Card*> communityCards;
    std::vector<Card*> flop;
    Card* turn;
    Card* river;

    int *clock;
    int *time; // If a player wants more time ==> Maybe in Player only?
};

// MARK: - @TODO to be implemented

Player *handWinner(Game *game);
void handlePot(Player *winner);

void removePlayer(Game *game, int playerIndex);

// Update the game or player time
void updateTime(int *time);

// Determine the best hand for a player
void findBestHand(Player *player);

// Update each player's hand based on community cards
// void updatePlayersHand(Game *game);


#endif /* GAME_H_ */