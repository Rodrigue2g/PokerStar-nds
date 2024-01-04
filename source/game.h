
#ifndef GAME_H_
#define GAME_H_
#include <iostream>
#include <vector>

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

    /**
     * @brief Find the winner of the current hand
     * 
     * @return true if a winner has been found
     * @return false otw (shouldn't return false)
     */
    bool findWinner();
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

#endif /* GAME_H_ */