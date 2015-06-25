#ifndef ENTITY_H
#define ENTITY_H

#include "../framework.h"
#include "../includes.h"
#include <map>

class Entity
{
public:
	int id;
	std::string name;
	std::vector<std::string> tags;

	std::map<int, Entity*> children;
	Entity* parent;

	Matrix44 model;

	static int GUID;

	Entity(std::string name);
	virtual ~Entity() = 0;

	virtual void processEvent(std::string name, void* data = NULL);

	void addChild(Entity* child);
	Matrix44 getGlobalMatrix();
	Entity* getParent();

	Entity* findByID(int id);
	Entity* findByName(std::string name);
	std::vector<Entity*> findByTag(std::string tag);

	static int getNextGUID();
};

#endif