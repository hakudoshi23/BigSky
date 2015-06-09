#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "../framework.h"
#include "gameState.h"

class MainState : public GameState
{
public:
	int update_count;

	MainState();

	void init();
	void render();
	void update(double delta);
	void destroy();
};


#endif