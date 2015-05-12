#ifndef ENTITY_H
#define ENTITY_H

#include "../framework.h"

class Entity
{
public:
	int id;
	std::string name;

	std::vector<Entity*> children;
	Entity* parent;

	Matrix44 model;

	static int GUID;

	Entity(std::string name);
	virtual ~Entity() = 0;

	virtual void processEvent(std::string name, void* data = NULL) = 0;

	void addChild(Entity* child);
	Matrix44 getGlobalMatrix();
	Entity* getParent();

	static int getNextGUID();
};

#endif