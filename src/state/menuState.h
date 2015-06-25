#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "../framework.h"

#include "hud/hud.h"
#include "gameState.h"

class MenuState : public GameState
{
public:
	HUD* hud;

	MenuState();
	~MenuState();

	void init();
	void render();
	void update(double delta);
	void destroy();
};


#endif