/**
 * @file game.h
 * @author Rodrigue de Guerre
 * @brief 
 * @version 0.1
 * @date 2024-01-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef GAME_H_
#define GAME_H_
#include <iostream>
#include <vector>

#include "wifi.h"
#include "card.h"


enum Hand {
    ROYAL_FLUSH = 10,
    STRAIGHT_FLUSH = 9,
    FOUR_OF_A_KIND = 8,
    FULL_HOUSE = 7,
    FLUSH = 6,
    STRAIGHT = 5,
    THREE_OF_A_KIND = 4,
    TWO_PAIRS = 3,
    PAIR = 2,
    HIGH_CARD = 1
};

struct BestHand {
    Hand hand;
    Rank highCard;
};

struct GameState {
    std::vector<int> playersIn;
    int currentBet;
    int playerBankroll;
};

class Game {
public:
    Game();
    Game(int numPlayers, bool isLocalGame, bool isHost);
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

    void joinGame();
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
    Card* nextCard();

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

    //MARK: Network methods
    //template <class T> void sendDataOnline(T data);
    /**
     * @brief Send a card to online player (from host)
     * 
     * @param card 
     */
    void sendCard(CardState card);
    /**
     * @brief Send the Game State to online player (from host)
     * 
     * @param state 
     */
    void sendGameState(GameState state);
    /**
     * @brief Wait for online player move (on host nds)
     * 
     * @param move 
     * @return true if move has been recieved  
     * @return false if move has not been recieved yet
     */
    bool waitForOnlineMove(Move& move);


    /**
     * @brief Wait to recieve a card from host nds (for online player)
     * 
     * @param card 
     * @return true if card has been recieved 
     * @return false if card has not been recieved yet
     */
    bool waitForCard(CardState& card);
    /**
     * @brief Wait to recieve the game state from host nds (for online player)
     * 
     * @param gameState 
     * @return true if state has been recieved 
     * @return false if state has not been recieved yet
     */
    bool waitForGameState(GameState& gameState);
    /**
     * @brief Send online player's move to host nds
     * 
     * @param move 
     */
    void sendMove(Move move);


    /**
     * @brief Find the winner of the current hand
     * 
     * @return true if a winner has been found
     * @return false otw (shouldn't return false)
     */
    bool findWinner();
private:
    bool isOnlineGame;
    bool isHost;

    int numPlayers;
    std::vector<Player*> players;
    int dealerIndex;

    int topCardIndex;
    Deck deck;  // std::vector<Card*>

    int total_pot;
    int currentBet;

    int smallBlind;
    int bigBlind;

    std::vector<Card*> communityCards;
    std::vector<Card*> flop;
    Card* turn;
    Card* river;
};


//MARK: Overload helpers
/**
 * @overload
 * 
 * @brief 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator>(const Hand lhs, const Hand rhs) 
{
    return static_cast<int>(lhs) > static_cast<int>(rhs);
}
/**
 * @overload
 * 
 * @brief 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator<(const Hand lhs, const Hand rhs) 
{
    return static_cast<int>(lhs) < static_cast<int>(rhs);
}
/**
 * @overload
 * 
 * @brief 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator==(const Hand lhs, const Hand rhs) 
{
    return static_cast<int>(lhs) == static_cast<int>(rhs);
}
/**
 * @overload
 * 
 * @brief 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator!=(const Hand lhs, const Hand rhs) 
{
    return static_cast<int>(lhs) != static_cast<int>(rhs);
}
/**
 * @overload
 * 
 * @brief 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator==(const Rank lhs, const Rank rhs)
{
    return static_cast<int>(lhs) == static_cast<int>(rhs);
}
/**
 * @overload
 * 
 * @brief 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator!=(const Rank lhs, const Rank rhs)
{
    return static_cast<int>(lhs) != static_cast<int>(rhs);
}
/**
 * @overload
 * 
 * @brief 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator<(const Rank lhs, const Rank rhs)
{
    return static_cast<int>(lhs) < static_cast<int>(rhs);
}
/**
 * @overload
 * 
 * @brief 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator>(const Rank lhs, const Rank rhs)
{
    return static_cast<int>(lhs) > static_cast<int>(rhs);
}

#endif /* GAME_H_ */