#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "../framework.h"
#include "../mesh.h"
#include "bullet.h"
#include "../entity/entity.h"

class BulletManager
{
public:
	static BulletManager *getInstance();

	std::vector<Bullet*> bullets;
	Mesh* mesh;
	
	Vector3 rand;

	~BulletManager();

	std::vector<Bullet*> getBullets();

	void render();
	void update(double delta);
	
	void createBullet( Vector3 pos, Vector3 vel, float dmg, int author_id );
	void clear();
private:
	BulletManager();
};

#endif