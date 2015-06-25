#ifndef WIN_STATE_H
#define WIN_STATE_H

#include "../framework.h"

#include "hud/hud.h"
#include "gameState.h"

class WinState : public GameState
{
public:
	HUD* hud;

	WinState();
	~WinState();

	void init();
	void render();
	void update(double delta);
	void destroy();
};


#endif