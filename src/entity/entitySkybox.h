#ifndef ENTITY_SKYBOX_H
#define ENTITY_SKYBOX_H

#include "../framework.h"

#include "entityMesh.h"

class EntitySkybox : public EntityMesh
{
public:
	EntitySkybox();
	~EntitySkybox();

	void processEvent(std::string name, void* data = NULL);
};

#endif