#include "entitySky.h"

#include "../includes.h"
#include <time.h>

#include "../game.h"

EntitySky::EntitySky() : EntityMesh("skybox")
{
}

EntitySky::~EntitySky()
{
	std::cout << "Delete EntitySky - " << this->name << std::endl;
}

void EntitySky::processEvent(std::string name, void* data)
{
	glDisable(GL_DEPTH_TEST);
	__super::processEvent(name, data);
	glEnable(GL_DEPTH_TEST);
}