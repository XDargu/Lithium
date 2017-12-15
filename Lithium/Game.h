#ifndef _GAME_H
#define _GAME_H

// Includes
#include "Core\Graphics\RenderManager.h"

// Compilation defines

// Forward declarations

// Classes
class Game
{
public:

	static Game& GetInstance();

	Game() {}

	sBool
	Construct();

	void
	Update();
	
	void
	Render();

	sBool
	ShouldFinishGame();

	void
	Release();

	cEgRenderManager mRenderManager;
};

#endif