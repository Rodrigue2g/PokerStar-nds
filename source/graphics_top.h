/**
 * @file graphics_top.h
 * @author Rodrigue de Guerre
 * @brief 
 * @version 0.1
 * @date 2024-01-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef GRAPHICS_MAIN_H_
#define GRAPHICS_MAIN_H_
#include "card.h"
#include "graphics_core.h"

/**
 * @brief Specification of core graphics namespace for the top screen
 * 
 */
namespace graphics::top
{
	/**
	 * @brief Player hand sprite (same as CardSprite but size changes so helps make a clear difference)
	 * 
	 */
	struct PlayerSprite {
		int x, y;

		u16* gfx;
		u8*  frame_gfx;

		int state;
		int count;
	};

	/**
	 * @brief Top Loading Screen
	 * 
	 */
	void loading();

	/**
	 * @brief Configuration of the top screen
	 * 
	 */
	void configGraphics();

	/**
	 * @brief Display the flop (3 first community cards)
	 * 
	 * @param cardState 
	 */
	void displayFlop(CardState* cardState);  // rm or put time init
	void displayFlop1(CardState cardState);
	void displayFlop2(CardState cardState);
	void displayFlop3(CardState cardState);

	/**
	 * @brief Display the turn (4th card)
	 * 
	 * @param cardState 
	 */
	void displayTurn(CardState cardState);

	/**
	 * @brief Display the river (5th/last card)
	 * 
	 * @param cardState 
	 */
	void displayRiver(CardState cardState);

	/**
	 * @brief Clean the Top Screen (remove the cards)
	 * 
	 */
	void clean();

	/**
	 * @brief Update Game informations on the top screen 
	 * + Total Pot 
	 * + Current bet informations
	 * + Each player's details (id, bankroll, current/ongoing bet)
	 * 
	 * @param players 
	 * @param total_pot 
	 * @param current_bet 
	 */
	void updateGraphics(const std::vector<Player*> players, const int total_pot, const int current_bet);


	void updateOnlineGraphics(const std::vector<Player*> players, const int total_pot, const int current_bet);
	void displayPlayersHands(const std::vector<Player*> players);
}

#endif /* GRAPHICS_MAIN_H_ */