#include "entity.h"

#include "../includes.h"
#include <string>

int Entity::GUID = 0;

Entity::Entity(std::string name) {
	this->id = this->getNextGUID();
	this->parent = NULL;
	this->name = name;
}

Entity::~Entity() {
	std::cout << "Delete Entity: " << this->id << std::endl;
}

void Entity::addChild(Entity* child) {
	this->children[child->id] = child;
	child->parent = this;
}

Matrix44 Entity::getGlobalMatrix() {
	if(this->parent == NULL){
		return this->model;
	}else{
		return this->model * this->getParent()->getGlobalMatrix();
	}
}

Entity* Entity::getParent() {
	return this->parent;
}

void Entity::processEvent(std::string name, void* data) {
	for(std::map<int, Entity*>::iterator it = children.begin(); it != children.end(); it++){
		it->second->processEvent(name, data);
	}
}

Entity* Entity::findByID(int id) {
	std::map<int, Entity*>::iterator it = children.find(id);
	if(it != children.end()){
		return it->second;
	}
	return NULL;
}

Entity* Entity::findByName(std::string name) {
	for(std::map<int, Entity*>::iterator it = children.begin(); it != children.end(); it++)
		if(it->second->name == name) return it->second;
	return NULL;
}

std::vector<Entity*> Entity::findByTag(std::string tag) {
	std::vector<Entity*> result;
	for (std::map<int, Entity*>::iterator it = children.begin() ; it != children.end(); ++it)
		if(std::find(it->second->tags.begin(), it->second->tags.end(), tag) != it->second->tags.end())
			result.push_back(it->second);
	return result;
}

int Entity::getNextGUID(){
	return ++Entity::GUID;
}