#include "entitySky.h"

#include "../includes.h"
#include <time.h>

#include "../game.h"

EntitySky::EntitySky() : EntityMesh("skybox") {
}

EntitySky::~EntitySky() {
	std::cout << "Delete EntitySky - " << this->name << std::endl;
}

void EntitySky::processEvent(std::string name, void* data) {
	this->model.setTranslation(
		World::getInstance()->camera->eye.x,
		World::getInstance()->camera->eye.y,
		World::getInstance()->camera->eye.z	
	);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	__super::processEvent(name, data);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}