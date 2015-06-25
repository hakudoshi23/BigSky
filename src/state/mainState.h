#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "../framework.h"

#include "hud/hud.h"
#include "gameState.h"

class MainState : public GameState
{
public:
	HUD* hud;
	int player_id, update_count;

	MainState();
	~MainState();

	void init();
	void render();
	void update(double delta);
	void destroy();
};


#endif