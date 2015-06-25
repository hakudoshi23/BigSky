#ifndef CONTROLLER_H
#define CONTROLLER_H
  
#include "../framework.h"
#include "../includes.h"

#include "../entity/entity.h"

class Controller
{
public:
	int entity_id;

	Controller(){this->entity_id = 0;}
	
	virtual void init() = 0;
	virtual void update(double delta) = 0;
};

#endif