// - nds/c++ libraries
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
//#include <time.h>
// - headers
#include "game.h"
#include "graphics_bottom.h"
#include "graphics_top.h"

//using namespace graphics;  // we could use it but it makes it clearer when we call graphics without it

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

//START_HAND: // avoid using this goto; just here to structure code
        shuffleDeck();
        dealCards();  
        // maybe just display both at ones for every player 
        graphics::bottom::printI(0);  //rm
        graphics::bottom::displayCard1(CardState(players[0]->hand[0]->rank + players[0]->hand[0]->suit));
        graphics::bottom::printI(0);  //rm
        graphics::bottom::displayCard2(CardState(players[0]->hand[1]->rank + players[0]->hand[1]->suit));
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
        bool won = findWinner(); // handle any error (returned false)

        // Check if any players are out of money
        for(auto& player : players) {
            if(player->bankroll == 0) {  // or <= 0 but should never be < 0
                players.erase(players.begin() + player->id);
                numPlayers -= 1;
            }
        }

//RESET_HAND: // avoid using this goto; just here to structure code
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
        graphics::top::clean();
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
            graphics::top::displayFlop1(CardState(flop[i]->rank + flop[i]->suit));
        } else if (i == 1) {
            graphics::top::displayFlop2(CardState(flop[i]->rank + flop[i]->suit));
        } else if (i == 2) {
            graphics::top::displayFlop3(CardState(flop[i]->rank + flop[i]->suit));
        }
        graphics::bottom::printI((flop[i]->rank + flop[i]->suit));
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
    graphics::top::displayTurn(CardState(turn->rank + turn->suit));
}

/**
 * @brief Deal river card (5th/last card)
 * 
 */
void Game::dealRiver() {
    // Deal river card
    communityCards[4] = nextCard();
    river = communityCards[4];
    graphics::top::displayRiver(CardState(river->rank + river->suit));
}


/**
 * @brief Handle each player's turn
 * 
 * @param game 
 */
bool Game::playersMove() {
    int players_left = numPlayers;
    for(auto player : players) {
        player->isPlaying = true;
        graphics::top::updateGraphics(players, total_pot, currentBet);
        graphics::bottom::updateGraphics(players[0]);

        if(players_left < 2)
            return false;

        bool validMove = false;
        while (!validMove) {
            Move res = waitForPlayerMove(player); // wait for each player to make a move
            switch (res.action) {
                case FOLD:
                    player->hasFolded = true;
                    --players_left;
                    validMove = true;
                    break;
                case CHECK:
                    if(player->currentBet >= currentBet) validMove = true; //otw ret to waitForPlayerMove()
                    break;
                case CALL:
                    if(player->bankroll < currentBet) {
                        player->isAllIn = true;
                        player->currentBet = player->bankroll;
                        player->bankroll = 0;

                        currentBet = player->currentBet;
                        total_pot += currentBet;
                    } else {
                        player->bankroll -= (currentBet - player->currentBet);
                        total_pot += (currentBet - player->currentBet);
                        player->currentBet = currentBet;
                    }
                    validMove = true;
                    break;
                case RAISE:
                    if(player->bankroll > res.amount && res.amount >= 2*currentBet) {
                        player->currentBet = res.amount;
                        player->bankroll -= res.amount;

                        currentBet = res.amount;
                        total_pot += currentBet;

                        validMove = true;
                    } else if(player->bankroll <= res.amount && res.amount >= 2*currentBet) {
                        player->isAllIn = true;
                        player->currentBet = player->bankroll;  //or res.amount
                        player->bankroll = 0;

                        //currentBet = player->currentBet;  // Not always ! 
                        total_pot += player->currentBet;

                        validMove = true;
                    }
                    break;
                case ALLIN:
                    player->isAllIn = true;
                    player->currentBet = player->bankroll;
                    player->bankroll = 0;

                    currentBet = player->currentBet;  // Not always !  (if current_bet > player's bankroll)

                    total_pot += player->currentBet;

                    validMove = true;
                    break;
            }     
            //updateGraphics(players, total_pot, currentBet);
        }
        player->isPlaying = false;
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
        return graphics::bottom::waitForLocalPlayerMove(player, currentBet);
    } else if(player->id == 100){ // change condition to network
        // Network call (multiplayer)
    } else {
        graphics::bottom::printI(0);
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


/**
 * @brief Determine the higher count of a certain occurence in a vector of a any type
 * 
 * @tparam T 
 * @param vect 
 * @return int 
 */
template<class T> int higherCount(const std::vector<T>& vect)
{
    int higher_count = 0;
    for(const auto& targ : vect) {
        int count = std::count(vect.begin(), vect.end(), targ);
        if(count > higher_count) higher_count = count;
    }
    return higher_count;
}

//MARK: Deteremine the best Hand 
/**
 * @brief Section 9 functions each determining a particular hand to then know which player has the best hand
 * 
 * @attention These functions are not headered as they are only intended to be used in bool Game::findWinner();
 * 
 */

/**
 * @brief 
 * 
 * @param hand 
 * @return true 
 * @return false 
 */
bool isRoyalFlush(std::vector<Rank> ranks, std::vector<Suit> suits) 
{
    std::sort(ranks.begin(), ranks.end());

    bool isRoyal = std::adjacent_find(ranks.begin(), ranks.end(),
                    [](Rank a, Rank b) { return static_cast<int>(a) + 1 != static_cast<int>(b); }) == ranks.end();

    bool isSameSuit = std::adjacent_find(suits.begin(), suits.end(),
                    [](Suit a, Suit b) { return a != b; }) == suits.end();

    return isRoyal && isSameSuit && ranks[0] == Rank::TEN && ranks[4] == Rank::KING;
}

bool isStraightFlush(std::vector<Rank> ranks, std::vector<Suit> suits) 
{
    std::sort(ranks.begin(), ranks.end());

    bool isSequential = std::adjacent_find(ranks.begin(), ranks.end(),
                        [](Rank a, Rank b) { return static_cast<int>(a) + 1 != static_cast<int>(b); }) == ranks.end();

    bool isSameSuit = std::adjacent_find(suits.begin(), suits.end(),
                        [](Suit a, Suit b) { return a != b; }) == suits.end();

    return isSequential && isSameSuit;
}

bool hasFourOfAKind(const std::vector<Rank>& ranks) 
{
    return higherCount(ranks) == 4;
}

bool hasFullHouse(const std::vector<Rank>& ranks) {
    int higher_count = 0;
    int _count = 0;
    //Rank val;
    Rank _targ;
    for(const auto& targ : ranks) {
        int count = std::count(ranks.begin(), ranks.end(), targ);
        if(count > higher_count) {
            higher_count = count;
            //val = targ;
        }
        // Full House check (AA KKK)
        if(count == 2) {  
            _count = 2;
            _targ = targ;
        }
        if(higher_count == 3 && _count == 2 && targ != _targ) return true;
    }
    return false;
    // ret FULL_HOUSE with Higher Rank  //Sould return both to really determine the best full house but ok
    //return (BestHand){FULL_HOUSE, targ > _targ ? targ : _targ};  
}

bool hasFlush(const std::vector<Suit>& suits) 
{
    return higherCount(suits) == 5;
}

bool hasStraight(std::vector<Rank> ranks) 
{
    std::sort(ranks.begin(), ranks.end());

    return std::adjacent_find(ranks.begin(), ranks.end(),
                    [](Rank a, Rank b) { return static_cast<int>(a) + 1 != static_cast<int>(b); }) == ranks.end();
}

bool hasThreeOfAKind(const std::vector<Rank>& ranks) 
{
    return higherCount(ranks) == 3;
}

bool hasTwoPairs(const std::vector<Rank>& ranks) {
    int higher_count = 0;
    int _count = 0;
    //Rank val;
    Rank _targ;
    for(const auto& targ : ranks) {
        int count = std::count(ranks.begin(), ranks.end(), targ);
        if(count > higher_count) {
            higher_count = count;
           //val = targ;
        }
        if(count == 2) {
            _count = count;
            _targ = targ;
        }
        if(higher_count == 2 && _count == 2 && targ != _targ) return true;
    }
    return false;
}

bool hasPair(const std::vector<Rank>& ranks) 
{
    return higherCount(ranks) == 2;
}

/**
 * @brief 
 * 
 * @param hand 
 * @return Hand 
 */
Hand findBestHand(const std::vector<Card>& hand)
{
    std::vector<Rank> ranks;
    std::vector<Suit> suits;
    for (const auto& card : hand) {
        ranks.push_back(card.rank);
        suits.push_back(card.suit);
    }
    if(isRoyalFlush(ranks, suits)) {
        return ROYAL_FLUSH;
    } 
    else if(isStraightFlush(ranks, suits)) {
        return STRAIGHT_FLUSH;
    } 
    else if(hasFourOfAKind(ranks)) {
        return FOUR_OF_A_KIND;
    } 
    else if(hasFullHouse(ranks)) {
        return FULL_HOUSE;
    }
    else if(hasFlush(suits)){
        return FLUSH;
    }
    else if(hasStraight(ranks)){
        return STRAIGHT;
    }
    else if(hasThreeOfAKind(ranks)) {
        return THREE_OF_A_KIND;
    }
    else if(hasTwoPairs(ranks)) {
        return TWO_PAIRS;
    }
    else if(hasPair(ranks)) {
        return PAIR;
    }
    return HIGH_CARD;
}

/**
 * @brief Find the winner of the current hand
 * 
 * @return true if a winner has been found
 * @return false otw (shouldn't return false)
 */
bool Game::findWinner()
{
    std::vector<Card> _communityCards;
    for (const auto& ptr : communityCards) {
        _communityCards.push_back(*ptr);
    }
    Hand bestHand;
    Player* winner = nullptr;
    for(const auto* player : players) {
        _communityCards.push_back(*player->hand[0]);
        _communityCards.push_back(*player->hand[1]);
        Hand _hand = findBestHand(_communityCards);
        if(_hand > bestHand) {
            bestHand = _hand;
            winner = const_cast<Player*>(player);
        }
        _communityCards.pop_back();
        _communityCards.pop_back();
    }
    if (winner != nullptr) {
        winner->bankroll += total_pot;
        return true;
    }
    return false;
}










/* 
#pragma once
template<typename T> const wchar_t *GetTypeName();

#define DEFINE_TYPE_NAME(type, name) \
    template<>const wchar_t *GetTypeName<type>(){return name;}
 */
template<class T> BestHand compare(const std::vector<T>& vect)
{
    T val;
    int higher_count = 0;
    int _count = 0;
    T _targ;
    for(const auto& targ : vect) {
        int count = std::count(vect.begin(), vect.end(), targ);
        if(count > higher_count) {
            higher_count = count;
            val = targ;
        }
        // Full House check (AA KKK)
        if(count == 2) {  
            _count = 2;
            _targ = targ;
        }
        if(higher_count == 3 && _count == 2 && targ != _targ)
            return (BestHand){FULL_HOUSE, targ > _targ ? targ : _targ};  // ret FULL_HOUSE with Higher Rank  //Sould return both to really determine the best full house but ok
    }

    BestHand best;

    if(std::is_same<T, Rank>::value && higher_count == 1) {
        best.hand = HIGH_CARD;

    } else if(std::is_same<T, Rank>::value && higher_count == 1) ;

    if(std::is_same<T, Suit>::value && higher_count == 5){

    }
}