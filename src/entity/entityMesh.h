#ifndef ENTITY_MESH_H
#define ENTITY_MESH_H

#include "../framework.h"

#include "entity.h"
#include "../mesh.h"
#include "../texture.h"
#include "../shader.h"

class EntityMesh : public Entity
{
public:
	std::string mesh;
	std::string mesh_low;
	//TODO: Merge in Material
	std::string texture;
	std::string shader;

	bool frustum_culling;
	bool depth_test;
	bool collision;

	EntityMesh(std::string name);
	~EntityMesh();

	void setMesh(std::string mesh);
	void setTexture(std::string mesh);
	void setShader(std::string shader);

	void processEvent(std::string name, void* data = NULL);
};

#endif