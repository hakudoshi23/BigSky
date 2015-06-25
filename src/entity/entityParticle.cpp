#include "entityParticle.h"

#include "../includes.h"
#include <time.h>

#include "../game.h"
#include "../world/particleManager.h"

EntityParticle::EntityParticle() : Entity("particle") {
	color = new Vector4(1,1,1,1);
	current_freq = 0;
	entity_ttl = -1;
	random = 2;
	freq = 0.1;
	size = 3;
	ttl = 3;
}

EntityParticle::~EntityParticle() {
	std::cout << "Delete EntityParticle - " << this->name << std::endl;
	delete this->color;
}

void EntityParticle::processEvent(std::string name, void* data) {
	if(name.compare("update") == 0){
		double delta = *(double*)data;
		if(this->entity_ttl != -1){
			this->entity_ttl -= delta;
			if(this->entity_ttl <= 0){
				World::getInstance()->destroy(this);
				return;
			}
		}
		current_freq += delta;
		while(current_freq > freq){
			Particle* p = new Particle();
			p->texture = this->texture;
			p->color = *this->color;
			Matrix44 mv = this->getGlobalMatrix();
			p->position = Vector3(mv.m[12], mv.m[13], mv.m[14]);
			p->velocity = Vector3();
			p->velocity.random(random);
			p->size = this->size;
			p->ttl = this->ttl;
			ParticleManager::getInstance()->addParticle(p);
			current_freq -= freq;
		}
	}
}