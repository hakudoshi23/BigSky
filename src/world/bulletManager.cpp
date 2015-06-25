#include "bulletManager.h"

#include <time.h>

#include "../game.h"

BulletManager::BulletManager() {
	this->mesh = new Mesh();
}

BulletManager::~BulletManager() {
	std::cout << "Delete BulletManager" << std::endl;
	delete this->mesh;
}

std::vector<Bullet*> BulletManager::getBullets() {
	return this->bullets;
}

void BulletManager::render(){
	if(bullets.size() > 0){
		/* Upload Bullet vertices */
		this->mesh->clear();
		for (std::vector<Bullet*>::iterator it = bullets.begin() ; it != bullets.end(); ++it){
			this->mesh->vertices.push_back((*it)->position + (*it)->velocity * 0.1);
			this->mesh->vertices.push_back((*it)->position);
		}
		this->mesh->uploadToVRAM();
		/* Render VBO */
		Shader* _s = Shader::Load("data/shaders/bullet");
		_s->enable();
		_s->setMatrix44("u_mvp", World::getInstance()->camera->viewprojection_matrix );
		_s->setMatrix44("u_model", Matrix44());
		glLineWidth(3);
		this->mesh->render(GL_LINES, _s);
		glLineWidth(1);
		_s->disable();
	}
}

void BulletManager::update(double delta){
	for (std::vector<Bullet*>::iterator it = bullets.begin() ; it != bullets.end();){
		/* Update position */
		(*it)->position = (*it)->position + (*it)->velocity * delta;
		/* Check alive time */
		(*it)->ttl -= delta;
		if((*it)->ttl <= 0) {
			delete (*it);
			it = bullets.erase(it);
		} else ++it;
	}
}

void BulletManager::createBullet( Vector3 pos, Vector3 vel, float dmg, int author_id) {
	rand.random(3);
	Bullet* b = new Bullet();
	b->author_id = author_id;
	b->position = pos;
	b->velocity = vel + rand;
	b->damage = dmg;
	b->ttl = 3;
	this->bullets.push_back(b);
}

void BulletManager::clear(){
	this->bullets.clear();
}

BulletManager* BulletManager::getInstance(){
	static BulletManager* instance = new BulletManager();
	return instance;
}