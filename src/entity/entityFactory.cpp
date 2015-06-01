#include "entityFactory.h"

EntityMesh* EntityFactory::newPlane()
{
	EntityMesh* e = new EntityMesh("plane");
	e->setMesh("data/mesh/spitfire/spitfire.ASE");
	e->setTexture("data/mesh/spitfire/spitfire_color_spec.tga");
	e->setShader("data/shaders/default");
	e->tags.push_back("plane");
	return e;
}

EntityMesh* EntityFactory::newIsland()
{
	EntityMesh* e = new EntityMesh("terrain");
	e->setMesh("data/mesh/island/island.ASE");
	e->setTexture("data/mesh/island/island_color.tga");
	e->setShader("data/shaders/default");
	e->frustum_culling = false;
	e->tags.push_back("terrain");
	return e;
}

EntityMesh* EntityFactory::newSea()
{
	EntityMesh* e = new EntityMesh("sea");
	e->setMesh("data/mesh/water/agua.ASE");
	e->setTexture("data/mesh/water/agua.tga");
	e->setShader("data/shaders/sea");
	e->frustum_culling = false;
	e->tags.push_back("sea");
	return e;
}

EntitySky* EntityFactory::newSky()
{
	EntitySky* e = new EntitySky();
	e->setMesh("data/mesh/skydome/cielo.ASE");
	e->setTexture("data/mesh/skydome/cielo.tga");
	e->setShader("data/shaders/default");
	e->frustum_culling = false;
	e->depth_test = false;
	e->tags.push_back("sky");
	return e;
}