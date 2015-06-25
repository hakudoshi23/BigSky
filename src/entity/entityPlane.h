#ifndef ENTITY_PLANE_H
#define ENTITY_PLANE_H

#include "../includes.h"
#include "../framework.h"

#include "entityMesh.h"
#include "../controller/controller.h"

class EntityPlane : public EntityMesh
{
public:
	Controller* controller;
	Vector3* max_move;
	
	float missile_cd, last_missile;
	float shoot_cd, last_shoot;
	float bullet_dmg;
	
	float shield, max_shield, shield_regen;
	float health, max_health;

	EntityPlane(std::string name);
	~EntityPlane();

	void processEvent(std::string name, void* data = NULL);

	void yaw(float speed);
	void roll(float speed);
	void pitch(float speed);

	bool shootBullet();
	bool shootMissile();

	void damage(float amount);
};

#endif