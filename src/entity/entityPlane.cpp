#include "entityPlane.h"

#include "../includes.h"
#include <time.h>

#include "../game.h"
#include "entityMissile.h"
#include "entityParticle.h"
#include "../sound/soundManager.h"
#include "../world/bulletManager.h"
#include "../controller/ai/aiController.h"

EntityPlane::EntityPlane(std::string name) : EntityMesh(name) {
	this->max_move = new Vector3(0.005, 0.03, 0.01);
	this->controller = NULL;
	this->missile_cd = 2;
	this->shoot_cd = 0.2f;
	this->bullet_dmg = 15;
	this->max_health = 12;
	this->health = this->max_health;
	this->max_shield = 30;
	this->shield = this->max_shield;
	this->shield_regen = 2;
}

EntityPlane::~EntityPlane() {
	if(this->controller != NULL)
		delete this->controller;
	delete this->max_move;
}

void EntityPlane::processEvent(std::string name, void* data) {
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
		/* Slowly refill shield */
		if(this->shield < this->max_shield)
			this->shield += this->shield_regen * delta;
	} else if(name.compare("damage") == 0){
		float amount = *(float*)data;
		this->damage(amount);
	}
	__super::processEvent(name, data);
}

void EntityPlane::yaw(float speed){
	float _s = this->max_move->x * speed;
	this->model.rotateLocal(_s, Vector3(0,1,0));
}

void EntityPlane::roll(float speed){
	float _s = this->max_move->y * speed;
	this->model.rotateLocal(_s, Vector3(0,0,1));
}

void EntityPlane::pitch(float speed){
	float _s = this->max_move->z * speed;
	this->model.rotateLocal(_s, Vector3(1,0,0));
}

bool EntityPlane::shootBullet(){
	Vector3 f = this->model.rotateVector(Vector3(0, 0, 1));
	Vector3 pos = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	if(Game::getInstance()->time - this->last_shoot > this->shoot_cd){
		BulletManager::getInstance()->createBullet(pos, f * 500, this->bullet_dmg, this->id);
		this->last_shoot = Game::getInstance()->time;
		return true;
	}
	return false;
}

bool EntityPlane::shootMissile(){
	if(Game::getInstance()->time - this->last_missile > this->missile_cd){
		EntityMissile* m = new EntityMissile();
		m->model = this->model;
		Vector3 f = m->model.rotateVector(Vector3(0, 0, 10));
		m->model.traslate(f.x, f.y, f.z);
		World::getInstance()->addChild(m);
		this->last_missile = Game::getInstance()->time;
		return true;
	}
	return false;
}

void EntityPlane::damage(float amount){
	float _dmg = amount;
	/* Consume shield first */
	if(this->shield > 5) {
		this->shield -= _dmg;
		_dmg = 0;
	}
	if(this->shield < 0) _dmg = -this->shield;
	/* When shield is empty, bad things happen */
	this->health -= _dmg;
	/*std::cout << "Health Updated [" << this->id << "] -> " << this->health 
		<< " (" << this->shield << ") - " << _dmg << std::endl;*/
	if(this->health <= 0) this->processEvent("destroy");
}