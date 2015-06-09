#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../framework.h"

class GameState
{
public:
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void update(double delta) = 0;
	virtual void destroy() = 0;
};


#endif