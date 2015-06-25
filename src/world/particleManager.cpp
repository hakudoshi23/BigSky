#include "particleManager.h"

#include <time.h>

#include "../game.h"

ParticleManager::ParticleManager() {
	this->mesh = new Mesh();
}

ParticleManager::~ParticleManager() {
	std::cout << "Delete ParticleManager" << std::endl;
	delete this->mesh;
}

void ParticleManager::render(){
	std::string last_texture = "";
	/* Upload Particle vertices */
	this->mesh->clear();
	Texture* _t;
	Shader* _s = Shader::Load("data/shaders/particle");
	_s->enable();
	_s->setMatrix44("u_mvp", World::getInstance()->camera->viewprojection_matrix );
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (std::vector<Particle*>::iterator it = particles.begin() ; it != particles.end(); ++it){
		/* If the textures is different from the provious particle, render! */
		if(last_texture != (*it)->texture){
			if(this->mesh->vertices.size() > 0 && last_texture != ""){
				_t = Texture::load(last_texture.c_str());
				_s->setTexture("u_texture", _t->texture_id);
				this->mesh->uploadToVRAM();
				this->mesh->render(GL_QUADS, _s);
				this->mesh->clear();
			}
			last_texture = (*it)->texture;
		}

		Matrix44 camView = World::getInstance()->camera->view_matrix;
		camView.inverse();

		this->mesh->vertices.push_back( (*it)->position + (camView.topVector() + camView.rightVector()) * (*it)->size );
		this->mesh->vertices.push_back( (*it)->position + (camView.topVector() - camView.rightVector()) * (*it)->size );
		this->mesh->vertices.push_back( (*it)->position + (camView.rightVector() * (-1) - camView.topVector()) * (*it)->size );
		this->mesh->vertices.push_back( (*it)->position + (camView.rightVector() - camView.topVector()) * (*it)->size );

		this->mesh->uvs.push_back( Vector2(1,1) );
		this->mesh->uvs.push_back( Vector2(1,0) );
		this->mesh->uvs.push_back( Vector2(0,0) );
		this->mesh->uvs.push_back( Vector2(0,1) );

		Vector4 c = (*it)->color;
		if((*it)->ttl <= 1) c.w = (*it)->ttl / 1;
		this->mesh->colors.push_back( c );
		this->mesh->colors.push_back( c );
		this->mesh->colors.push_back( c );
		this->mesh->colors.push_back( c );
		this->mesh->colors.push_back( c );
		this->mesh->colors.push_back( c );
	}
	if(this->mesh->vertices.size() > 0 && last_texture != ""){
		_t = Texture::load(last_texture.c_str());
		_s->setTexture("u_texture", _t->texture_id);
		this->mesh->uploadToVRAM();
		this->mesh->render(GL_QUADS, _s);
		this->mesh->clear();
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	_s->disable();
}

void ParticleManager::update(double delta){
	for (std::vector<Particle*>::iterator it = particles.begin() ; it != particles.end();){
		(*it)->ttl -= delta;
		(*it)->position = (*it)->position + (*it)->velocity * delta;
		if((*it)->ttl <= 0){
			delete (*it);
			it = this->particles.erase(it);
		}else ++it;
	}
}

void ParticleManager::addParticle( Particle* p) {
	this->particles.push_back(p);
}

void ParticleManager::clear(){
	this->particles.clear();
}

ParticleManager* ParticleManager::getInstance(){
	static ParticleManager* instance = new ParticleManager();
	return instance;
}