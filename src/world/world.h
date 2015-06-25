#ifndef WORLD_H
#define WORLD_H

#include "../framework.h"

#include "../camera.h"
#include "../clipper.h"
#include "../entity/entity.h"
#include "../entity/entityMesh.h"
#include "../controller/playerController.h"

class World : public Entity
{
public:
	static World* getInstance();

	std::vector<Entity*> toDestroy;

	Clipper* clipper;
	Camera* camera;

	World();
	~World();

	void processEvent(std::string name, void* data = NULL);
	
	void init();
	void render();
	void update(double delta);

	void destroy(Entity* entity);
	void destroy();
};

#endif