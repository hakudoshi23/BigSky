#ifndef CONTROLS_STATE_H
#define CONTROLS_STATE_H

#include "../framework.h"

#include "hud/hud.h"
#include "gameState.h"

class ControlsState : public GameState
{
public:
	HUD* hud;

	ControlsState();
	~ControlsState();

	void init();
	void render();
	void update(double delta);
	void destroy();
};


#endif