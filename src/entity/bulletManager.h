#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "../framework.h"
#include "../mesh.h"
#include "bullet.h"
#include "entity.h"

class BulletManager : public Entity
{
public:
	static BulletManager *getInstance();

	std::vector<Bullet*> bullets;
	Vector3* gravity;
	Mesh* mesh;
	
	Vector3 rand;

	BulletManager();
	~BulletManager();

	std::vector<Bullet*> getBullets();

	void processEvent(std::string name, void* data = NULL);
	
	void createBullet( Vector3 pos, Vector3 vel, float dmg, int author_id );
	
	static void destroy();
private:
	static BulletManager *instance;
};

#endif