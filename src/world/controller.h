#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../framework.h"

#include "world.h"

class Controller
{
public:
	int entity_id;

	Controller();
	~Controller();

	void setEntity(int entity_id);
	
	void init();
	void update(double delta);
};

#endif