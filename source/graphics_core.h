/**
 * @file graphics_core.h
 * @author Rodrigue de Guerre
 * @brief 
 * @version 0.1
 * @date 2024-01-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef GRAPHICS_CORE_H_
#define GRAPHICS_CORE_H_

#include "card.h"

/**
 * @headerfile : header-only core graphics
 * 
 * @brief namespace to encapsulate graphics implementation
 * + specified by nested namespaces : top & bottom
 * + Helps avoiding global variables without using classes for graphics
 * 
 */
namespace graphics
{
	/**
	 * @brief Card sprite : 
	 * + frame_gfx pointer helps select one out of 52 cards
	 * 
	 */
	struct CardSprite
	{
		int x, y;

		u16* gfx;
		u8*  frame_gfx;

		int state;
		int count;
	};
}

#endif /* GRAPHICS_CORE_H_ */
