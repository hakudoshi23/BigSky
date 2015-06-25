#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include "../framework.h"

#include "entity.h"
#include "entityMesh.h"
#include "entitySky.h"
#include "entityPlane.h"

#include "../mesh.h"
#include "../texture.h"
#include "../shader.h"

class EntityFactory
{
public:
	static EntityPlane* newEnemy();
	static EntityPlane* newPlayer();
	static EntityMesh* newAsteroid(int type);
	static EntityMesh* newFrigate();
	static EntityMesh* newPlanet();
	static EntitySky* newSky();
};

#endif
