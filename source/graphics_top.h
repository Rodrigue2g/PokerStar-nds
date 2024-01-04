
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
	 * @brief Player icon sprite
	 * 
	 */
	struct PlayerSprite {
		int x, y;
		u16* gfx;
		int color;
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

}

#endif /* GRAPHICS_MAIN_H_ */