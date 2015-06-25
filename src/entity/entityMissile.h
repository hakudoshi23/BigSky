#ifndef ENTITY_MISSILE_H
#define ENTITY_MISSILE_H

#include "../includes.h"
#include "../framework.h"

#include "entityMesh.h"
#include "../controller/controller.h"

class EntityMissile : public EntityMesh
{
public:
	Controller* controller;
	Vector3* max_move;

	float dmg;

	EntityMissile();
	~EntityMissile();

	void processEvent(std::string name, void* data = NULL);

	void yaw(float speed);
	void roll(float speed);
	void pitch(float speed);
};

#endif