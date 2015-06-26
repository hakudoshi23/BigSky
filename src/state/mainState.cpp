#include "mainState.h"

#include "../includes.h"
#include <iostream>

#include "hud/gameHud.h"
#include "../game.h"
#include "../camera.h"
#include "../world/world.h"
#include "../world/bullet.h"
#include "../entity/entity.h"
#include "../sound/soundManager.h"
#include "../entity/entityMissile.h"
#include "../world/bulletManager.h"
#include "../world/particleManager.h"
#include "../entity/entityFactory.h"
#include "../controller/ai/aiController.h"

#include "../state/winState.h"
#include "../state/loseState.h"

MainState::MainState() {
	this->score = 0;
	this->hud = new GameHUD(&this->score);
	this->hud->init();
	this->update_count = 0;
	this->player_id = 0;
}

MainState::~MainState() {
	delete this->hud;
}

void MainState::init() {
	glClearColor(0.623529, 0.752941, 0.796078, 1.0);

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	World::getInstance()->camera->setPerspective(70, Game::getInstance()->window_width / (float) Game::getInstance()->window_height, 1, 10000);

	SoundManager::getInstance()->playSound("data/sound/player_engine.wav", 0, true, 1);
	SoundManager::getInstance()->playSound("data/sound/fight_music.wav", 0, true, 1);

	/* Fill world with entities */
	World::getInstance()->addChild(EntityFactory::newSky());
	/* Add player */
	EntityPlane* ep = EntityFactory::newPlayer();
	World::getInstance()->addChild(ep);
	/* Setup camera */
	Vector3 playerPos = Vector3(ep->model.m[12], ep->model.m[13], ep->model.m[14]);
	Vector3 center = playerPos + ep->model.rotateVector(Vector3(0, 1, 5));
	Vector3 up = ep->model.rotateVector(Vector3(0, 1, 0));
	Vector3 eye = ep->model * Vector3(0, 8, -100);
	World::getInstance()->camera->lookAt(eye, center, up);
	this->player_id = ep->id;
	/* Add frigate */
	Entity* e = EntityFactory::newFrigate();
	e->model.setTranslation(750, 300, 2000);
	e->model.rotateLocal(3.5, Vector3(0, 1, 0));
	World::getInstance()->addChild(e);
	/* Add enemies */
	ep = EntityFactory::newEnemy();
	ep->model.setTranslation(250, 0, 2000);
	AIController* aic = (AIController*) ep->controller;
	aic->patroll_points.push_back(Vector3(750, 0, 2000));
	aic->patroll_points.push_back(Vector3(750, 0, 1500));
	aic->patroll_points.push_back(Vector3(250, 0, 1500));
	aic->patroll_points.push_back(Vector3(250, 0, 2000));
	World::getInstance()->addChild(ep);
	ep = EntityFactory::newEnemy();
	ep->model.setTranslation(250, 50, 2000);
	aic = (AIController*) ep->controller;
	aic->patroll_points.push_back(Vector3(750, 50, 2000));
	aic->patroll_points.push_back(Vector3(750, 50, 1500));
	aic->patroll_points.push_back(Vector3(250, 50, 1500));
	aic->patroll_points.push_back(Vector3(250, 50, 2000));
	World::getInstance()->addChild(ep);
	/* Add asteroids */
	EntityMesh* em = EntityFactory::newAsteroid(0);
	World::getInstance()->addChild(em);
	em = EntityFactory::newAsteroid(0);
	em->model.rotateLocal(3.5, Vector3(0, 1, 0));
	World::getInstance()->addChild(em);
}
   
void MainState::render() {
	World::getInstance()->render();
	BulletManager::getInstance()->render();
	ParticleManager::getInstance()->render();
	this->hud->render();
}

void MainState::update(double delta) {
	this->update_count++;
	/* Avoid checking all collisions at the same frame! */
	World::getInstance()->update(delta);
	BulletManager::getInstance()->update(delta);
	ParticleManager::getInstance()->update(delta);
	this->hud->update(delta);

	/* Have you lost? */
	Entity* player = World::getInstance()->findByID(this->player_id);
	if(player == NULL) {
		std::cout << "YOU LOSE!" << std::endl;
		Game::swapState(new LoseState());
	}else if(World::getInstance()->findByName("enemy") == NULL){
		std::cout << "YOU WIN!" << std::endl;
		Game::swapState(new WinState(this->score));
	}

	/* Update Camera position */
	if(player != NULL) {
		Matrix44 invCam = World::getInstance()->camera->view_matrix;
		invCam.inverse();
		Vector3 perfectEye = player->model * Vector3(0, 8, -12);
		Vector3 playerPos = Vector3(player->model.m[12],player->model.m[13],player->model.m[14]);
		Vector3 center = playerPos + player->model.rotateVector(Vector3(0, 1, 5));
		Vector3 up = player->model.rotateVector(Vector3(0, 1, 0));
		Vector3 eye = Vector3(invCam.m[12],invCam.m[13],invCam.m[14]);
		eye = eye + ((perfectEye - eye) * 0.1f);
		World::getInstance()->camera->lookAt(eye, center, up);
	}
	
	std::vector<Entity*> terrains = World::getInstance()->findByTag("terrain");
	std::vector<Bullet*> bullets = BulletManager::getInstance()->getBullets();
	std::vector<Entity*> planes = World::getInstance()->findByTag("plane");
	std::vector<Entity*> missiles = World::getInstance()->findByTag("missile");
	EntityMesh* frigate = (EntityMesh*) World::getInstance()->findByName("frigate");
	float collision_dmg = 500;

	for (std::vector<Entity*>::iterator it = planes.begin() ; it != planes.end(); ++it){
		EntityMesh* em = (EntityMesh*) (*it);
		Mesh* _m = Mesh::load(em->mesh.c_str());
		Vector3 pos = Vector3(em->model.m[12],em->model.m[13],em->model.m[14]);
		_m->collision_model->setTransform(em->model.m);
		/* Test collisions - Plane vs Bullet */
		for (std::vector<Bullet*>::iterator it2 = bullets.begin() ; it2 != bullets.end(); ++it2){
			if((*it2)->author_id != em->id && _m->collision_model->sphereCollision((*it2)->position.v, 1)){
				(*it2)->ttl = 0;
				em->processEvent("damage", &(*it2)->damage);
				if((*it2)->author_id == this->player_id)
					this->score += 1;
			}
		}
		/* Test collisions - Plane vs Plane */
		for (std::vector<Entity*>::iterator it2 = it + 1 ; it2 != planes.end(); ++it2){
			Vector3 origin = Vector3((*it2)->model.m[12],(*it2)->model.m[13],(*it2)->model.m[14]);
			if(_m->collision_model->sphereCollision(origin.v, 3)){
				em->processEvent("damage", &collision_dmg);
				(*it2)->processEvent("damage", &collision_dmg);
			}
		}
		/* Test collisions - Plane vs Terrain */
		for (std::vector<Entity*>::iterator it2 = terrains.begin() ; it2 != terrains.end(); ++it2){
			EntityMesh* em2 = (EntityMesh*) (*it2);
			Mesh* _m2 = Mesh::load(em2->mesh.c_str());
			_m2->collision_model->setTransform(em2->model.m);
			if(_m2->collision_model->sphereCollision(pos.v, 3)){
				em->processEvent("damage", &collision_dmg);
			}
		}
		/* Test collisions - Plane vs Frigate */
		if(frigate != NULL){
			Mesh* fMesh = Mesh::load(frigate->mesh.c_str());
			fMesh->collision_model->setTransform(frigate->model.m);
			if(fMesh->collision_model->sphereCollision(pos.v, 3)){
				em->processEvent("damage", &collision_dmg);
			}
		}
		/* Test collisions - Plane vs Missile */
		EntityMissile* miss;
		for (std::vector<Entity*>::iterator it = missiles.begin() ; it != missiles.end(); ++it){
			miss = (EntityMissile*) (*it);
			Vector3 origin = Vector3(miss->model.m[12], miss->model.m[13], miss->model.m[14]);
			if(_m->collision_model->sphereCollision(origin.v, 2)){
				em->processEvent("damage", &miss->dmg);
				miss->processEvent("destroy");
				if(em->id != this->player_id)
					this->score += 20;
			}else if(miss->ttl <= 0)
				miss->processEvent("destroy");
		}
	}
}

void MainState::destroy() {
	SoundManager::getInstance()->stop();
	World::getInstance()->destroy();
	World::getInstance()->update(1);
	BulletManager::getInstance()->clear();
	ParticleManager::getInstance()->clear();
}