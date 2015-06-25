#include "entityMissile.h"

#include "../includes.h"
#include <time.h>

#include "../game.h"
#include "entityParticle.h"
#include "../sound/soundManager.h"
#include "../controller/ai/missileController.h"

EntityMissile::EntityMissile() : EntityMesh("missile") {
	this->max_move = new Vector3(0.1, 0.1, 0.1);
	this->tags.push_back("missile");

	MissileController* c = new MissileController();
	c->entity_id = this->id;
	c->init();
	this->controller = c;

	this->texture = "data/mesh/missile/missile.tga";
	this->mesh = "data/mesh/missile/missile.ASE";
	this->shader = "data/shaders/phong";

	this->dmg = 200;
}

EntityMissile::~EntityMissile() {
	if(this->controller != NULL)
		delete this->controller;
	delete this->max_move;
}

void EntityMissile::processEvent(std::string name, void* data) {
	if(name.compare("destroy") == 0){
		SoundManager::getInstance()->playSound("data/sound/player_explossion.wav", 3000);
		EntityParticle* ep = new EntityParticle();
		ep->texture = "data/textures/smoke.tga";
		ep->model = this->model;
		ep->freq = 0.1f;
		ep->random = 5;
		ep->size = 10;
		this->getParent()->addChild(ep);
		World::getInstance()->destroy(this);
	} else if(name.compare("update") == 0){
		double delta = *(double*)data;
		/* Update controller (move) */
		if(this->controller != NULL)
			this->controller->update(delta);
	}
	__super::processEvent(name, data);
}

void EntityMissile::yaw(float speed){
	float _s = this->max_move->x * speed;
	this->model.rotateLocal(_s, Vector3(0,1,0));
}

void EntityMissile::roll(float speed){
	float _s = this->max_move->y * speed;
	this->model.rotateLocal(_s, Vector3(0,0,1));
}

void EntityMissile::pitch(float speed){
	float _s = this->max_move->z * speed;
	this->model.rotateLocal(_s, Vector3(1,0,0));
}