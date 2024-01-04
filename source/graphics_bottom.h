
#ifndef GRAPHICS_BOTTOM_H_
#define GRAPHICS_BOTTOM_H_

#include <nds.h>
#include <stdio.h>
#include "graphics_core.h"
#include "card.h"

/**
 * @brief Specification of core graphics namespace for the bottom screen
 * 
 */
namespace graphics::bottom
{
	/**
	 * @brief Configuration of the bottom screen
	 * 
	 */
	void configGraphics();

	/**
	 * @brief Display the hand (2 cards) of the local player
	 * 
	 * @param cardState 
	 */
	void displayHand(CardState* cardState);
	void displayCard1(CardState cardState);
	void displayCard2(CardState cardState);

	/**
	 * @brief Fold the hand of the local player
	 * 
	 */
	void fold();
	/**
	 * @brief Remove the cards of the local player (if he's out of money/doesn't paly)
	 * 
	 */
	void rmCards();

	/**
	 * @brief Get the number of Players before starting a game
	 * 
	 * @param numPlayers 
	 * @return int 
	 */
	int getNbOfPlayers(int numPlayers = 3);

	/**
	 * @brief Update the bottom screen
	 * 
	 * @param player 
	 */
	void updateGraphics(const Player* player);

	/**
	 * @brief Wait for the local player to make a move
	 * 
	 * @param player 
	 * @param current_bet 
	 * @return Move 
	 */
	Move waitForLocalPlayerMove(const Player* player, const int current_bet);

	void printI(int i = 0); //rm
}

#endif /* GRAPHICS_BOTTOM_H_ */
