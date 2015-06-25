#ifndef LOSE_STATE_H
#define LOSE_STATE_H

#include "../framework.h"

#include "hud/hud.h"
#include "gameState.h"

class LoseState : public GameState
{
public:
	HUD* hud;

	LoseState();
	~LoseState();

	void init();
	void render();
	void update(double delta);
	void destroy();
};


#endif