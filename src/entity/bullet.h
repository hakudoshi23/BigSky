#ifndef BULLET_H
#define BULLET_H

#include "../includes.h"
#include "../framework.h"

class Bullet
{
public:
	Vector3 position, velocity;
	float damage, ttl;
	int author_id;

	Bullet();
	~Bullet();
};

#endif