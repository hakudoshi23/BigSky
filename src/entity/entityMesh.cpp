#include "entityMesh.h"

#include "../includes.h"
#include <time.h>

#include "../game.h"

EntityMesh::EntityMesh(std::string name) : Entity(name) {
	this->frustum_culling = true;
	this->depth_test = true;
	this->collision = true;
}

EntityMesh::~EntityMesh() {
	std::cout << "Delete EntityMesh - " << this->name << std::endl;
}

void EntityMesh::processEvent(std::string name, void* data) {
	if(name.compare("render") == 0){
		if(!this->frustum_culling || World::getInstance()->clipper->PointInFrustum(this->model.m[12],this->model.m[13],this->model.m[14]))
		{
			Mesh* _m = Mesh::load(mesh.c_str());
			Shader* _s = Shader::Load(shader.c_str());
			Texture* _t = Texture::load(texture.c_str());
			Matrix44 mvp = this->getGlobalMatrix() * World::getInstance()->camera->viewprojection_matrix;
			_s->enable();
			_s->setMatrix44("u_model", this->model );
			_s->setMatrix44("u_mvp", mvp );
			_s->setUniform1("u_time", Game::getInstance()->time);
			_s->setTexture("u_texture", _t->texture_id);
			_m->render(GL_TRIANGLES, _s);
			_s->disable();
		}
	}
	__super::processEvent(name, data);
}

void EntityMesh::setMesh(std::string mesh) {
	this->mesh = mesh;
}

void EntityMesh::setTexture(std::string texture) {
	this->texture = texture;
}

void EntityMesh::setShader(std::string shader) {
	this->shader = shader;
}