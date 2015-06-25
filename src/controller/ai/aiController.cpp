#include "aiController.h"

#include "../../includes.h"
#include "../../utils.h"

#include "../../world/bulletManager.h"
#include "../../sound/soundManager.h"
#include "../../game.h"

AIController::AIController(){
	this->current_patrol_index = 0;
	this->state = AI_STATE_PATROL;
	this->detect_radius = 300;
	this->shootCD = 0.5;
	this->lastShoot = 0;
}

AIController::~AIController(){
}

void AIController::init(){
}

void AIController::update(double delta){
	Entity* plane = World::getInstance()->findByID(this->entity_id);
	if(plane != NULL){
		double speed = delta * 40;

		Vector3 f = plane->model.rotateVector(Vector3(0, 0, 1));
		plane->model.traslate(f.x * speed, f.y * speed, f.z * speed);
		
		Vector3 v = Vector3();
		switch(this->state){
			case AI_STATE_PATROL:
				if(this->patroll_points.size() > 0){
					/* Follow path */
					float dst = this->chase(this->patroll_points[this->current_patrol_index]);
					if(dst < 2){
						this->current_patrol_index++;
						if(this->current_patrol_index >= this->patroll_points.size())
							this->current_patrol_index = 0;
					}
					Entity* player = World::getInstance()->findByName("player");
					if(player != NULL){
						Vector3 playerPos = Vector3(player->model.m[12], player->model.m[13], player->model.m[14]);
						Vector3 pos = Vector3(plane->model.m[12],plane->model.m[13],plane->model.m[14]);
						if(playerPos.distance(pos) < this->detect_radius)
							this->state = AI_STATE_ATTACK;
					}
				}
				break;
			case AI_STATE_ATTACK:
				Entity* player = World::getInstance()->findByName("player");
				if(player != NULL){
					Vector3 playerPos = Vector3(player->model.m[12], player->model.m[13], player->model.m[14]);
					this->chase(playerPos);
					/* Shoot bullets */
					Vector3 pos = Vector3(plane->model.m[12],plane->model.m[13],plane->model.m[14]);
					if(this->lastShoot > 0) this->lastShoot -= delta;
					if(this->lastShoot <= 0){
						BulletManager::getInstance()->createBullet(pos, f * 500, 10, this->entity_id);
						this->lastShoot = this->shootCD;
					}
					if(playerPos.distance(pos) > this->detect_radius * 1.2)
						this->state = AI_STATE_PATROL;
				} else this->state = AI_STATE_PATROL;
				break;
		}
	}
}