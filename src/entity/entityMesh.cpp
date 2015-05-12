#include "entityMesh.h"

#include "../includes.h"
#include <time.h>

#include "../game.h"

EntityMesh::EntityMesh(std::string name) : Entity(name)
{
	this->frustum_culling = true;
}

EntityMesh::~EntityMesh()
{
	std::cout << "Delete EntityMesh - " << this->name << std::endl;
}

void EntityMesh::processEvent(std::string name, void* data)
{
	if(name.compare("render") == 0){
		if(!this->frustum_culling || Game::world->clipper->PointInFrustum(this->model.m[12],this->model.m[13],this->model.m[14]))
		{
			Mesh* _m = Mesh::load(mesh.c_str());
			Texture* _t = Texture::load(texture.c_str());
			Matrix44 mvp = this->getGlobalMatrix() * Game::world->camera->viewprojection_matrix;
			glBindTexture(GL_TEXTURE_2D, _t->texture_id);
			shader->enable();
			shader->setMatrix44("u_model", this->model );
			shader->setMatrix44("u_mvp", mvp );
			shader->setTexture("u_texture", _t->texture_id);
			_m->renderVBO(GL_TRIANGLES, shader);
			shader->disable();
			glBindTexture(GL_TEXTURE_2D, _t->texture_id);
		}
	}/*else if(name.compare("update") == 0){
		double delta = *((double*)data);
		this->model.rotateLocal(5 * DEG2RAD * delta, Vector3(0,1,0));
	}*/
	//Entity::processEvent(name, data);
}

void EntityMesh::setMesh(std::string mesh)
{
	this->mesh = mesh;
}

void EntityMesh::setTexture(std::string texture)
{
	this->texture = texture;
}

void EntityMesh::setShader(Shader* shader)
{
	this->shader = shader;
}