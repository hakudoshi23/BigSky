#include "mainState.h"

#include "../includes.h"
#include <iostream>

#include "hud/gameHud.h"
#include "../game.h"
#include "../camera.h"
#include "../world/world.h"
#include "../entity/entity.h"
#include "../world/bullet.h"
#include "../world/bulletManager.h"
#include "../world/particleManager.h"
#include "../entity/entityFactory.h"
#include "../controller/ai/aiController.h"

#include "../state/winState.h"
#include "../state/loseState.h"

MainState::MainState() {
	this->hud = new GameHUD();
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

	/* Fill world with entities */
	World::getInstance()->addChild(EntityFactory::newSky());

	EntityPlane* ep = EntityFactory::newPlayer();
	ep->model.setTranslation(200, 1000, 1000);
	World::getInstance()->addChild(ep);
	Vector3 playerPos = Vector3(ep->model.m[12], ep->model.m[13], ep->model.m[14]);
	Vector3 center = playerPos + ep->model.rotateVector(Vector3(0, 1, 5));
	Vector3 up = ep->model.rotateVector(Vector3(0, 1, 0));
	Vector3 eye = ep->model * Vector3(0, 8, -100);
	World::getInstance()->camera->lookAt(eye, center, up);
	this->player_id = ep->id;

	ep = EntityFactory::newEnemy();
	ep->model.setTranslation(100, 1000, 1400);
	AIController* aic = (AIController*) ep->controller;
	aic->patroll_points.push_back(Vector3(150, 1200, 2000));
	aic->patroll_points.push_back(Vector3(100, 500, 2000));
	aic->patroll_points.push_back(Vector3(400, 500, 1500));
	World::getInstance()->addChild(ep);

	World::getInstance()->addChild(EntityFactory::newAsteroid(0));

	Entity* e = EntityFactory::newFrigate();
	e->model.setTranslation(150, 600, 2000);
	World::getInstance()->addChild(e);
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
		Game::swapState(new WinState());
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
	
	std::vector<Entity*> planes = World::getInstance()->findByTag("plane");

	/* Test collisions - Plane vs Bullet */
	if(this->update_count % 4 == 0){
		std::vector<Bullet*> bullets = BulletManager::getInstance()->getBullets();
		for (std::vector<Entity*>::iterator it = planes.begin() ; it != planes.end(); ++it){
			EntityMesh* em = (EntityMesh*) (*it);
			//std::cout << "Entity ID: " << (*it)->id << std::endl;
			for (std::vector<Bullet*>::iterator it2 = bullets.begin() ; it2 != bullets.end(); ++it2){
				Mesh* _m = Mesh::load(em->mesh.c_str());
				_m->collision_model->setTransform(em->model.m);
				if((*it2)->author_id != em->id && _m->collision_model->sphereCollision((*it2)->position.v, 1)){
					(*it2)->ttl = 0;
					em->processEvent("damage", &(*it2)->damage);
				}
			}
		}
	}

	/* Test collisions - Plane vs Plane */
	float dmg = 500;
	if(this->update_count % 4 == 1){
		for (std::vector<Entity*>::iterator it = planes.begin() ; it != planes.end(); ++it){
			EntityMesh* em1 = (EntityMesh*) (*it);
			Mesh* mesh1 = Mesh::load(em1->mesh.c_str());
			mesh1->collision_model->setTransform(em1->model.m);
			for (std::vector<Entity*>::iterator it2 = it + 1 ; it2 != planes.end(); ++it2){
				Vector3 origin = Vector3((*it2)->model.m[12],(*it2)->model.m[13],(*it2)->model.m[14]);
				if(mesh1->collision_model->sphereCollision(origin.v, 5)){
					em1->processEvent("damage", &dmg);
					(*it2)->processEvent("damage", &dmg);
				}
			}
		}
	}
}

void MainState::destroy() {
	World::getInstance()->destroy();
	World::getInstance()->update(1);
	BulletManager::getInstance()->clear();
	ParticleManager::getInstance()->clear();
}