#include "entitySkybox.h"

#include "../includes.h"
#include <time.h>

#include "../game.h"

EntitySkybox::EntitySkybox() : EntityMesh("skybox")
{
}

EntitySkybox::~EntitySkybox()
{
	std::cout << "Delete EntityMesh - " << this->name << std::endl;
}

void EntitySkybox::processEvent(std::string name, void* data)
{
	if(name.compare("render") == 0){
		glDisable(GL_DEPTH_TEST);
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
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);
	}else{
		EntityMesh::processEvent(name, data);
	}
}