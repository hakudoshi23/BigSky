#include "world.h"
#include "../utils.h"
#include "../mesh.h"
#include "../texture.h"
#include "../shader.h"

#include "../entity/entityFactory.h"
#include "../entity/entityMesh.h"
#include "../entity/entitySky.h"

#include "../entity/bulletManager.h"

#include <cmath>

World::World() : Entity("world")
{
	this->controller = new Controller();
	this->clipper = new Clipper();
	this->terrain = NULL;
	this->skybox = NULL;
}

World::~World()
{
	std::cout << "Detroy World!" << std::endl;
}

void World::setCamera(Camera* camera)
{
	this->camera = camera;
}

void World::init()
{
	this->skybox = EntityFactory::newSky();
	this->addChild(this->skybox);

	this->terrain = EntityFactory::newIsland();
	this->addChild(this->terrain);

	int x,z;
	for(x = -3; x < 4; x++){
		for(z = -3; z < 4; z++){
			this->terrain = EntityFactory::newSea();
			this->terrain->model.setTranslation(x * 10000, 15, z * 10000);
			this->addChild(this->terrain);
		}
	}

	int i;
	EntityMesh* e;
	e = EntityFactory::newPlane();
	e->model.setTranslation(200, 1700, 1600);
	this->addChild(e);
	this->controller->setEntity(e->id);
	this->controller->setCamera(camera);
	for(i = 0; i<10;i++){
		e = EntityFactory::newPlane();
		e->model.setTranslation(rand() % 200-100, 1500 + rand() % 200-100, 1400 + rand() % 200-100);
		e->tags.push_back("enemy");
		this->addChild(e);
	}

	this->controller->init();
}

void World::render()
{
	camera->set();
	this->clipper->ExtractFrustum(this->camera);

	this->skybox->model.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);

	this->processEvent("render");
}

void World::update(double delta)
{
	this->controller->update(delta);
	this->processEvent("update", &delta);
	if(this->toDestroy.size() > 0){
		for (std::vector<Entity*>::iterator it = this->toDestroy.begin() ; it != this->toDestroy.end();){
			Entity* parent = (*it)->parent;
			std::cout << "Let's delete Entity [" << (*it)->id << "] parent " << parent->id << std::endl;
			for (std::vector<Entity*>::iterator it2 = parent->children.begin() ; it2 != parent->children.end();){
				if((*it)->id == (*it2)->id){
					delete *it2;
					it2 = parent->children.erase(it2);
				} else ++it2;
			}
			it = this->toDestroy.erase(it);
		}
	}
}

void World::processEvent(std::string name, void* data)
{
	for (std::vector<Entity*>::iterator it = children.begin() ; it != children.end(); ++it)
		(*it)->processEvent(name, data);
}

Entity* World::findByID(int id)
{
	for (std::vector<Entity*>::iterator it = children.begin() ; it != children.end(); ++it)
		if((*it)->id == id) return *it;
	return NULL;
}

Entity* World::findByName(std::string name)
{
	for (std::vector<Entity*>::iterator it = children.begin() ; it != children.end(); ++it)
		if((*it)->name == name) return *it;
	return NULL;
}

std::vector<Entity*> World::findByTag(std::string tag)
{
	std::vector<Entity*> result;
	for (std::vector<Entity*>::iterator it = children.begin() ; it != children.end(); ++it)
		if(std::find((*it)->tags.begin(), (*it)->tags.end(), tag) != (*it)->tags.end())
			result.push_back(*it);
	return result;
}

void World::destroy(Entity* entity)
{
	this->toDestroy.push_back(entity);
}

