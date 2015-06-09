#include "bulletManager.h"

#include <time.h>

#include "../game.h"

BulletManager* BulletManager::instance = NULL;

BulletManager::BulletManager() : Entity("BulletManager")
{
	this->gravity = new Vector3(0,-9.81,0);
	this->mesh = new Mesh();
}

BulletManager::~BulletManager()
{
	std::cout << "Delete BulletManager" << std::endl;
}

std::vector<Bullet*> BulletManager::getBullets()
{
	return this->bullets;
}

void BulletManager::processEvent(std::string name, void* data)
{
	if(name.compare("render") == 0){
		if(bullets.size() > 0){
			/* Upload Bullet vertices */
			this->mesh->clear();
			for (std::vector<Bullet*>::iterator it = bullets.begin() ; it != bullets.end(); ++it){
				this->mesh->vertices.push_back((*it)->position + (*it)->velocity * 0.1);
				this->mesh->vertices.push_back((*it)->position);
				this->mesh->colors.push_back(Vector4(1,0,0,0.5));
				this->mesh->colors.push_back(Vector4(1,0,0,0));
			}
			this->mesh->uploadToVRAM();
			/* Render VBO */
			Shader* _s = Shader::Load("data/shaders/bullet");
			Matrix44 mvp = this->getGlobalMatrix() * Game::getInstance()->world->camera->viewprojection_matrix;
			_s->enable();
			_s->setMatrix44("u_model", this->model );
			_s->setMatrix44("u_mvp", mvp );
			glEnable( GL_BLEND );
			glLineWidth(2);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			this->mesh->render(GL_LINES, _s);
			glLineWidth(1);
			glDisable( GL_BLEND );
			_s->disable();
		}
	}else if(name.compare("update") == 0){
		std::vector<int> toDelete;
		double delta = *(double*)data;
		for (std::vector<Bullet*>::iterator it = bullets.begin() ; it != bullets.end(); ++it){
			/* Check alive time */
			(*it)->ttl -= delta;
			if((*it)->ttl <= 0) toDelete.push_back(it - bullets.begin());
			/* Update position */
			Vector3 v = (*it)->velocity + *this->gravity * delta * 10;
			(*it)->position = (*it)->position + v * delta;
		}

		for (unsigned i=0; i < toDelete.size(); ++i){
			bullets.erase(bullets.begin() + toDelete.at(i));
		}
	}
}

void BulletManager::createBullet( Vector3 pos, Vector3 vel, float dmg, int author_id)
{
	rand.random(3);
	Bullet* b = new Bullet();
	b->author_id = author_id;
	b->position = pos;
	b->velocity = vel + rand;
	b->damage = dmg;
	b->ttl = 3;
	this->bullets.push_back(b);
}

BulletManager* BulletManager::getInstance()
{
	if (!BulletManager::instance){
		BulletManager::instance = new BulletManager();
		Game::getInstance()->world->addChild(BulletManager::instance);
	}
	return BulletManager::instance;
}

void BulletManager::destroy()
{
	if (BulletManager::instance){
		delete BulletManager::instance;
		BulletManager::instance = NULL;
	}
}