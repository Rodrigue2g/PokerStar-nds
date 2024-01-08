/**
 * @file game.cpp
 * @author Rodrigue de Guerre
 * @brief 
 * @version 0.1
 * @date 2024-01-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
// - nds/c++ libraries
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
//#include <time.h>
// - headers
#include "game.h"
#include "graphics_bottom.h"
#include "graphics_top.h"
#include "wifi.h"

//using namespace graphics;  // we could use it but it makes it clearer when we call graphics without it

/**
 * @brief For network syncing : wait for confirmation that the data has been recieved
 * 
 * @return true 
 * @return false 
 */
static bool waitForAck()
{
    char msg[1];
    NetworkResponse res;
	if(receiveData(msg,1)>0) {
        res = static_cast<NetworkResponse>(msg[0]);
        return res == ACK;
    }
    return false;
}
/**
 * @brief For network syncing : send confirmation that the data has been well recieved
 * 
 */
static void sendAck()
{
	char msg[1];
    msg[0] = static_cast<NetworkResponse>(ACK);
    sendData(msg, 1);
}

/**
 * @brief Default Construct for a new Game object :: Defaults to 3 players
 * 
 */
Game::Game()
    : isOnlineGame(false), isHost(true), numPlayers(3), dealerIndex(0), topCardIndex(0),
      total_pot(0), currentBet(0), smallBlind(5), bigBlind(10) {

    // Initialize deck
    for (Suit suit = SPADE; suit <= CLUB; ++suit) {
        for (Rank rank = AS; rank <= KING; ++rank) {
            deck.push_back(new Card);
            deck[suit+rank]->suit = suit;
            deck[suit+rank]->rank = rank;
        }
    }

    // Initialize players
    for (int i = 0; i < numPlayers; ++i) {
        players.push_back(new Player);
        players[i]->id = i;
        players[i]->bankroll = 100; // = 10€
        players[i]->currentBet = 0;
        players[i]->hasFolded = false;
        players[i]->isAllIn = false;
        players[i]->isDealer = false;
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
Game::Game(int nbplayers, bool online, bool host)
    : isOnlineGame(online), isHost(host), numPlayers(nbplayers), dealerIndex(0), topCardIndex(0),
      total_pot(0), currentBet(0), smallBlind(5), bigBlind(10) {

    // Initialize deck
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            deck.push_back(new Card);
            deck[suit+rank]->suit = static_cast<Suit>(suit);
            deck[suit+rank]->rank = static_cast<Rank>(rank);
        }
    }
    // Initialize players
    int bots;
    numPlayers > 3 ? bots = 0 : bots = 3 - numPlayers; // Must be at least 3 to start the game

    for (int i = 0; i < numPlayers + bots; ++i) {  // Player 0 is Local & 1 is Online ; Then the rest are bots
        players.push_back(new Player);
        players[i]->id = i;
        players[i]->bankroll = 100; // = 10€
        players[i]->currentBet = 0;
        players[i]->hasFolded = false;
        players[i]->isAllIn = false;
        players[i]->isDealer = false;
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
    if(isOnlineGame && isHost) {

    }
    while(numPlayers > 1) { // and add 'start' key interrupt

//START_HAND: // avoid using this goto; just here to structure code
        shuffleDeck();
        dealCards();  
        graphics::bottom::nextStep();
        graphics::bottom::displayCard1(CardState(players[0]->hand[0]->rank + players[0]->hand[0]->suit));
        graphics::bottom::nextStep();
        graphics::bottom::displayCard2(CardState(players[0]->hand[1]->rank + players[0]->hand[1]->suit));

        // Send data if host
        if(isOnlineGame && isHost) {
            sendCard(CardState(players[1]->hand[0]->rank + players[1]->hand[0]->suit));
            bool ack = false;
            while(!ack) {
                ack = waitForAck();
                graphics::bottom::stateInfo("waiting too");
            }
            graphics::bottom::stateInfo("got it too");
            sendCard(CardState(players[1]->hand[0]->rank + players[1]->hand[0]->suit));
            while(!ack) {
                ack = waitForAck();
            }
        }

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
        // Display everyone's (left in game) hand
        graphics::top::displayPlayersHands(players);
        graphics::bottom::nextStep();

        // Determine winner and handle pot
        int winnerId = findWinner();
        graphics::top::displayResult(players[0]->id == static_cast<unsigned int>(winnerId), winnerId); // Did the local player win?

        // Check if any players are out of money
        for(size_t i=0; i<players.size(); ++i) {
            if(players[i]->bankroll == 0) {  // or <= 0 but should never be < 0
                players.erase(players.begin() + i);
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
        graphics::bottom::updateGraphics(players[0]);
        graphics::top::clean();
        //nextStep();
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
    for(auto& player : players) {
        player->hand.push_back(nextCard());
    }
    for(auto& player : players) {
        player->hand.push_back(nextCard());
    }
}

/**
 * @brief Deal flop cards (one by one or all at once?)
 * 
 */
void Game::dealFlop() {
    for(int i = 0; i < 3; i++) {
        communityCards.push_back(nextCard());
        flop.push_back(communityCards[i]);
        if (i == 0) {
            graphics::top::displayFlop1(CardState(flop[i]->rank + flop[i]->suit));
            // send data if host
                if(isOnlineGame && isHost) {
                    sendCard(CardState(flop[i]->rank + flop[i]->suit));
                    bool ack = false;
                    while(!ack) {
                        ack = waitForAck();
                    }
                }
        } else if (i == 1) {
            graphics::top::displayFlop2(CardState(flop[i]->rank + flop[i]->suit));
            // send data if host
            if(isOnlineGame && isHost) {
                sendCard(CardState(flop[i]->rank + flop[i]->suit));
                bool ack = false;
                while(!ack) {
                    ack = waitForAck();
                }
            }
        } else if (i == 2) {
            graphics::top::displayFlop3(CardState(flop[i]->rank + flop[i]->suit));
            // send data if host
            if(isOnlineGame && isHost) {
                sendCard(CardState(flop[i]->rank + flop[i]->suit));
                bool ack = false;
                while(!ack) {
                    ack = waitForAck();
                }
            }
        }
        graphics::bottom::nextStep();
    }
}

/**
 * @brief Deal turn card (4th card)
 * 
 */
void Game::dealTurn() {
    communityCards.push_back(nextCard());
    turn = communityCards[3];
    graphics::top::displayTurn(CardState(turn->rank + turn->suit));
     // send data if host
    if(isOnlineGame && isHost) {
        sendCard(CardState(turn->rank + turn->suit));
        bool ack = false;
        while(!ack) {
            ack = waitForAck();
        }
    }
}

/**
 * @brief Deal river card (5th/last card)
 * 
 */
void Game::dealRiver() {
    communityCards.push_back(nextCard());
    river = communityCards[4];
    graphics::top::displayRiver(CardState(river->rank + river->suit));
     // send data if host
    if(isOnlineGame && isHost) {
        sendCard(CardState(river->rank + river->suit));
        bool ack = false;
        while(!ack) {
            ack = waitForAck();
        }
    }
}


/**
 * @brief Handle each player's turn
 * 
 * @param game 
 */
bool Game::playersMove() {
    int players_left = numPlayers;
    for(auto& player : players) {
        if(player->hasFolded) continue;

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
    } else if(player->id == 1 && isOnlineGame){ // Network available with only one NDS. LoaclID = 0 & OnlineID = 1
        GameState gamestate;
        for(auto& elem : players) {
            if(!elem->hasFolded) gamestate.playersIn.push_back(elem->id);
        }
        gamestate.currentBet = currentBet;
        gamestate.playerBankroll = player->bankroll;
        sendGameState(gamestate);
        bool ack = false;
        while(!ack) {
            ack = waitForAck();  //dwni?
        }
        Move move;
        bool recieved = false;
        while(!recieved) {
            recieved = waitForOnlineMove(move);
        }
        return move;
    } else { // ai (bots)
        graphics::bottom::nextStep();

        if(currentBet == player->currentBet) {
            return (Move){CHECK, 0};
        } else if(currentBet > 5*player->currentBet && player->currentBet !=0 ) {
            return (Move){FOLD, 0};
        } else if(currentBet > player->currentBet && player->bankroll > currentBet) {
            return (Move){CALL, currentBet - player->currentBet};
        } else if(currentBet > player->currentBet && player->bankroll <= currentBet) {
            return (Move){ALLIN, player->bankroll};
        } else if(player->hand[0]->rank == static_cast<Rank>(6) && player->hand[1]->rank == static_cast<Rank>(9)) {
            return (Move){ALLIN, player->bankroll};
        } else if(player->hand[1]->rank == static_cast<Rank>(6) && player->hand[0]->rank == static_cast<Rank>(9)) {
            return (Move){ALLIN, player->bankroll};
        } else {
            return (Move){FOLD,0};
        }
    }
}


void Game::joinGame()  // host id = 0 , local id = 1, bot id = 2+
{
    while(numPlayers > 1) {
        // Display Hand
        CardState card;
        GameState gameState;
        total_pot = smallBlind + bigBlind; 
        bool recieved = false;
        while(!recieved){
            recieved = waitForCard(card);
            graphics::bottom::stateInfo("waiting");
        }
        graphics::bottom::stateInfo("got it");
        sendAck();
        graphics::bottom::displayCard1(card);
        recieved = false;
        while(!recieved){
            recieved = waitForCard(card);
        }
        sendAck();
        graphics::bottom::displayCard2(card);


        // Wait for his turn then wait for first move
        Move move;
        recieved = false;
        while(!recieved){
            recieved = waitForGameState(gameState);
        }
        sendAck();
        int playersIn = 0;
        for(auto& player : players) {
            bool isIn = std::find(gameState.playersIn.begin(), gameState.playersIn.end(), player->id) != gameState.playersIn.end();
            isIn ? player->hasFolded = false : player->hasFolded = true;
            isIn ? playersIn += 1 :  playersIn += 0;
        }
        currentBet = gameState.currentBet;
        // currentBet == bigBlind ? total_pot = playersIn*bigBlind : total_pot += playersIn*currentBet;
        total_pot = playersIn*currentBet;
        players[1]->bankroll = gameState.playerBankroll;

        move = graphics::bottom::waitForLocalPlayerMove(players[1], currentBet);
        sendMove(move);
        
        graphics::top::updateGraphics(players, total_pot, currentBet);
        graphics::bottom::updateGraphics(players[1]);


        // Display Flop
        recieved = false;
        while(!recieved){
            recieved = waitForCard(card);
        }
        sendAck();
        graphics::top::displayFlop1(card);
        recieved = false;
        while(!recieved){
            recieved = waitForCard(card);
        }
        sendAck();
        graphics::top::displayFlop2(card);
        recieved = false;
        while(!recieved){
            recieved = waitForCard(card);
        }
        sendAck();
        graphics::top::displayFlop3(card);

        
        // Wait for his turn then wait for second move
        if(move.action != FOLD) {
            recieved = false;
            while(!recieved){
                recieved =  waitForGameState(gameState);
            }
            playersIn = 0;
            for(auto& player : players) {
                bool isIn = std::find(gameState.playersIn.begin(), gameState.playersIn.end(), player->id) != gameState.playersIn.end();
                isIn ? player->hasFolded = false : player->hasFolded = true;
                isIn ? playersIn += 1 :  playersIn += 0;
            }
            currentBet = gameState.currentBet;
            // currentBet == bigBlind ? total_pot = playersIn*bigBlind : total_pot += playersIn*currentBet;
            total_pot = playersIn*currentBet;
            players[1]->bankroll = gameState.playerBankroll;

            move = graphics::bottom::waitForLocalPlayerMove(players[1], currentBet);
            graphics::top::updateGraphics(players, total_pot, currentBet);
            graphics::bottom::updateGraphics(players[1]);
            sendMove(move);
        }


        // Display Turn
        recieved = false;
        while(!recieved){
            recieved = waitForCard(card);
        }
        graphics::top::displayTurn(card);


        // Wait for his turn then wait for third move
        if(move.action != FOLD) {
            recieved = false;
            while(!recieved){
                recieved = waitForGameState(gameState);
            }
           playersIn = 0;
            for(auto& player : players) {
                bool isIn = std::find(gameState.playersIn.begin(), gameState.playersIn.end(), player->id) != gameState.playersIn.end();
                isIn ? player->hasFolded = false : player->hasFolded = true;
                isIn ? playersIn += 1 :  playersIn += 0;
            }
            currentBet = gameState.currentBet;
            // currentBet == bigBlind ? total_pot = playersIn*bigBlind : total_pot += playersIn*currentBet;
            total_pot = playersIn*currentBet;
            players[1]->bankroll = gameState.playerBankroll;

            move = graphics::bottom::waitForLocalPlayerMove(players[1], currentBet);
            graphics::top::updateGraphics(players, total_pot, currentBet);
            graphics::bottom::updateGraphics(players[1]);

            sendMove(move);
        }


        // Display River
        recieved = false;
        while(!recieved){
            recieved = waitForCard(card);
        }
        graphics::top::displayRiver(card);


        // Wait for his turn then wait for last move
        if(move.action != FOLD) {
            recieved = false;
            while(!recieved){
                recieved =  waitForGameState(gameState);
            }
            playersIn = 0;
            for(auto& player : players) {
                bool isIn = std::find(gameState.playersIn.begin(), gameState.playersIn.end(), player->id) != gameState.playersIn.end();
                isIn ? player->hasFolded = false : player->hasFolded = true;
                isIn ? playersIn += 1 :  playersIn += 0;
            }
            currentBet = gameState.currentBet;
            // currentBet == bigBlind ? total_pot = playersIn*bigBlind : total_pot += playersIn*currentBet;
            total_pot = playersIn*currentBet;
            players[1]->bankroll = gameState.playerBankroll;

            move = graphics::bottom::waitForLocalPlayerMove(players[1], currentBet);
            graphics::top::updateGraphics(players, total_pot, currentBet);
            graphics::bottom::updateGraphics(players[1]);

            sendMove(move);
        }

        recieved = false;
        while(!recieved){
            recieved = waitForGameState(gameState);
        }
        // display winner + other's cards 
    }
}

/**
 * @brief Send a card to online player (from host)
 * 
 * @param card 
 */
void Game::sendCard(CardState card)
{
	char msg[1];
    msg[0] = static_cast<char>(card);
    sendData(msg, 1);
}
/**
 * @brief Send the Game State to online player (from host)
 * 
 * @param state 
 */
void Game::sendGameState(GameState state)
{
	char msg[state.playersIn.size() + 3];
    for (size_t i(0); i<state.playersIn.size(); ++i) {
        msg[i] = static_cast<char>(state.playersIn[i]);
    }
    msg[state.playersIn.size()] = '|';
    msg[state.playersIn.size()+1] = state.currentBet;
    msg[state.playersIn.size()+2] = state.playerBankroll;
    sendData(msg, sizeof(msg));
}
/**
 * @brief Wait for online player move (on host nds)
 * 
 * @param move 
 * @return true if move has been recieved  
 * @return false if move has not been recieved yet
 */
bool Game::waitForOnlineMove(Move& move)
{
    char msg[2];
	if(receiveData(msg, 2)>0	)
	{
        move.action = static_cast<Action>(msg[0]);
        move.amount = static_cast<int>(msg[1]);
        return true;
	}
    return false;
}

/**
 * @brief Wait to recieve a card from host nds (for online player)
 * 
 * @param card 
 * @return true if card has been recieved 
 * @return false if card has not been recieved yet
 */
bool Game::waitForCard(CardState& card)
{
	char msg[1];
	if(receiveData(msg, 1)>0)
	{
        card = static_cast<CardState>(msg[0]);
        return true;
	}
    return false;
}
/**
 * @brief Wait to recieve the game state from host nds (for online player)
 * 
 * @param gameState 
 * @return true if state has been recieved 
 * @return false if state has not been recieved yet
 */
bool Game::waitForGameState(GameState& gameState)
{
	char msg[6]; // max size for 3 players : 6bytes ; +1 bytes per players added > 3
	if(receiveData(msg, 6)>0) {
        std::string str(msg);
        for(char c : str) {
            if(c == '|') {
                str.erase(0, 1);
                break;
            }
            gameState.playersIn.push_back(static_cast<int>(c));
            str.erase(0, 1);
        }
        gameState.currentBet = static_cast<int>(str[0]);
        gameState.playerBankroll = static_cast<int>(str[1]);
        return true;
	}
    return false;
}
/**
 * @brief Send online player's move to host nds
 * 
 * @param move 
 */
void Game::sendMove(Move move)
{
    char msg[2];
    msg[0] = static_cast<char>(move.action);
    msg[1] = static_cast<char>(move.amount);
    sendData(msg, 2);
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
    Rank _targ;
    for(const auto& targ : ranks) {
        int count = std::count(ranks.begin(), ranks.end(), targ);
        if(count > higher_count) {
            higher_count = count;
        }
        // Full House check (AA KKK)
        if(count == 2) {  
            _count = 2;
            _targ = targ;
        }
        if(higher_count == 3 && _count == 2 && targ != _targ) return true;
    }
    return false;
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
    Rank _targ;
    for(const auto& targ : ranks) {
        int count = std::count(ranks.begin(), ranks.end(), targ);
        if(count > higher_count) {
            higher_count = count;
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
 * @return id of winner
 * @return -1 if err
 */
int Game::findWinner()
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
        return winner->id;
    }
    return -1;
}