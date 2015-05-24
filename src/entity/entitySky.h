#ifndef ENTITY_SKYBOX_H
#define ENTITY_SKYBOX_H

#include "../framework.h"

#include "entityMesh.h"

class EntitySky : public EntityMesh
{
public:
	EntitySky();
	~EntitySky();

	void processEvent(std::string name, void* data = NULL);
};

#endif