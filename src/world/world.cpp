#include "world.h"
#include "../utils.h"
#include "../mesh.h"
#include "../texture.h"
#include "../shader.h"

#include "../entity/entityFactory.h"
#include "../entity/entityMesh.h"
#include "../entity/entitySky.h"
#include "../entity/entityParticle.h"

#include "../world/bulletManager.h"
#include "../controller/playerController.h"

#include <cmath>

World::World() : Entity("root") {
	this->clipper = new Clipper();
	this->camera = new Camera();
}

World::~World() {
	std::cout << "Detroy World!" << std::endl;
}

void World::init() {
}

void World::render() {
	this->camera->set();
	this->clipper->ExtractFrustum(camera);
	this->processEvent("render");
}

void World::update(double delta) {
	this->processEvent("update", &delta);
	/* Free unused entities */
	if(this->toDestroy.size() > 0){
		for (std::vector<Entity*>::iterator it = this->toDestroy.begin() ; it != this->toDestroy.end();){
			delete (*it);
			it = this->toDestroy.erase(it);
		}
	}
}

void World::processEvent(std::string name, void* data) {
	for(std::map<int, Entity*>::iterator it = children.begin(); it != children.end(); it++)
		it->second->processEvent(name, data);
}

void World::destroy(Entity* entity){
	std::map<int, Entity*>::iterator it;
	while((it = entity->children.begin()) != entity->children.end())
		this->destroy(it->second);
	if(entity->parent != NULL){
		it = entity->parent->children.find(entity->id);
		if(it != entity->parent->children.end()) entity->parent->children.erase(it);
	}
	for (std::vector<Entity*>::iterator it = this->toDestroy.begin() ; it != this->toDestroy.end(); ++it)
		if((*it)->id == entity->id) return;
	this->toDestroy.push_back(entity);
}

void World::destroy(){
	std::map<int, Entity*>::iterator it;
	while((it = this->children.begin()) != this->children.end())
		this->destroy(it->second);
}

World* World::getInstance(){
	static World* instance = new World();
	return instance;
}