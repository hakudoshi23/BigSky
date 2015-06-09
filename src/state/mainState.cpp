#include "mainState.h"

#include "../includes.h"
#include <iostream>

#include "../game.h"
#include "../world/world.h"
#include "../entity/bullet.h"
#include "../entity/bulletManager.h"

MainState::MainState(){
	this->update_count = 0;
}

void MainState::init()
{
	glClearColor(0.623529, 0.752941, 0.796078, 1.0);

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	Camera* camera = new Camera();
	camera->lookAt(Vector3(0,1500,1500),Vector3(0,750,0), Vector3(0,1,0));
	camera->setPerspective(70, Game::getInstance()->window_width/ (float) Game::getInstance()->window_height, 1, 10000);

	Game::getInstance()->world = new World();
	Game::getInstance()->world->setCamera(camera);
	Game::getInstance()->world->init();
}
   
void MainState::render()
{
	Game::getInstance()->world->render();
}

void MainState::update(double delta)
{
	this->update_count++;
	/* Avoid checking all collisions at the same frame! */
	Game::getInstance()->world->update(delta);
	
	std::vector<Entity*> planes = Game::getInstance()->world->findByTag("plane");

	/* Test collisions - Plane vs Sea */
	if(this->update_count % 4 == 2){
		for (std::vector<Entity*>::iterator it = planes.begin() ; it != planes.end(); ++it){
			EntityMesh* em1 = (EntityMesh*) (*it);
			if(em1->model.m[13] <= 0){
				Game::getInstance()->world->destroy(em1);
			}
		}
	}

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
					Game::getInstance()->world->destroy(em);
				}
			}
		}
	}

	/* Test collisions - Plane vs Plane */
	if(this->update_count % 4 == 1){
		for (std::vector<Entity*>::iterator it = planes.begin() ; it != planes.end(); ++it){
			EntityMesh* em1 = (EntityMesh*) (*it);
			Mesh* mesh1 = Mesh::load(em1->mesh.c_str());
			mesh1->collision_model->setTransform(em1->model.m);
			for (std::vector<Entity*>::iterator it2 = it + 1 ; it2 != planes.end(); ++it2){
				//std::cout << "Check: " << (*it)->id << " - " << (*it2)->id << std::endl;
				EntityMesh* em2 = (EntityMesh*) (*it2);
				Mesh* mesh2 = Mesh::load(em2->mesh.c_str());
				if(mesh1->collision_model->collision(mesh2->collision_model, -1, 0.1, em2->model.m)){
					std::cout << "P vs P Collision! " << (*it)->id << " - " << (*it2)->id << std::endl;
					Game::getInstance()->world->destroy(em1);
					Game::getInstance()->world->destroy(em2);
				}
			}
		}
	}
}

void MainState::destroy()
{
}