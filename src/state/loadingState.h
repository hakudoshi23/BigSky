#ifndef LOADING_STATE_H
#define LOADING_STATE_H

#include "../framework.h"

#include "hud/hud.h"
#include "gameState.h"

class LoadingState : public GameState
{
public:
	HUD* hud;
	bool done;

	LoadingState();
	~LoadingState();

	void init();
	void render();
	void update(double delta);
	void destroy();
};


#endif