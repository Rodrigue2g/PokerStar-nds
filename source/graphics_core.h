
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

	template<typename T>
	class Graphics: public T {
	public:
		Graphics();
		~Graphics();

	protected:
		PrintConsole topScreen, bottomScreen;
	private:

	};
}

#endif /* GRAPHICS_CORE_H_ */


/*
namespace A 
{
	struct SomeStruct{};
}

namespace A::B {
	void useSomeStruct(A::SomeStruct s); 

	using namespace A;
}
*/