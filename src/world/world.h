#ifndef WORLD_H
#define WORLD_H

#include "../framework.h"

#include "../camera.h"
#include "../clipper.h"
#include "../entity/entity.h"
#include "../entity/entityMesh.h"

class World : public Entity
{
public:
	Camera* camera;
	Clipper* clipper;

	EntityMesh* terrain;
	EntityMesh* skybox;

	World();
	~World();

	void processEvent(std::string name, void* data = NULL);

	void setCamera(Camera* camera);
	
	void init();
	void render();
	void update(double delta);

	Entity* findByID(int id);
	Entity* findByName(std::string name);
};

#endif