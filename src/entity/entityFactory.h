#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include "../framework.h"

#include "entity.h"
#include "entityMesh.h"
#include "entitySky.h"

#include "../mesh.h"
#include "../texture.h"
#include "../shader.h"

class EntityFactory
{
public:
	static EntityMesh* newPlane();
	static EntityMesh* newIsland();
	static EntityMesh* newSea();
	static EntitySky* newSky();
};

#endif
