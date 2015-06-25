#include "missileController.h"

#include "../../includes.h"
#include "../../utils.h"

#include "../../world/bulletManager.h"
#include "../../sound/soundManager.h"
#include "../../game.h"

MissileController::MissileController(){
	this->detect_radius = 100;
	this->target_id = -1;
}

MissileController::~MissileController(){
}

void MissileController::init(){
}

void MissileController::update(double delta){
	Entity* plane = World::getInstance()->findByID(this->entity_id);
	if(plane != NULL){
		double speed = delta * 200;

		Vector3 f = plane->model.rotateVector(Vector3(0, 0, 1));
		plane->model.traslate(f.x * speed, f.y * speed, f.z * speed);
		
		if(this->target_id == -1){
			Vector3 tPos, pos = Vector3(plane->model.m[12], plane->model.m[13], plane->model.m[14]);
			std::vector<Entity*> enemies = World::getInstance()->findByTag("enemy");
			for (std::vector<Entity*>::iterator it = enemies.begin() ; it != enemies.end(); ++it){
				tPos = Vector3((*it)->model.m[12], (*it)->model.m[13], (*it)->model.m[14]);
				if(pos.distance(tPos) < this->detect_radius) this->target_id = (*it)->id;
			}
		}else{
			Entity* e = World::getInstance()->findByID(this->target_id);
			this->chase(Vector3(e->model.m[12], e->model.m[13], e->model.m[14]));
		}
	}
}