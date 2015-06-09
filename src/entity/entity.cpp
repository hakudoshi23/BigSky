#include "entity.h"

#include "../includes.h"

int Entity::GUID = 0;

Entity::Entity(std::string name)
{
	this->id = this->getNextGUID();
	this->parent = NULL;
	this->name = name;
}

Entity::~Entity()
{
	std::cout << "Delete Entity!" << std::endl;
}

void Entity::addChild(Entity* child)
{
	this->children.push_back(child);
	child->parent = this;
}

Matrix44 Entity::getGlobalMatrix()
{
	if(this->parent == NULL){
		return this->model;
	}else{
		return this->model * this->getParent()->getGlobalMatrix();
	}
}

Entity* Entity::getParent()
{
	return this->parent;
}

int Entity::getNextGUID(){
	return ++Entity::GUID;
}