#ifndef ENTITY_PARTICLE_H
#define ENTITY_PARTICLE_H

#include "../framework.h"

#include "entity.h"
#include "../mesh.h"
#include "../world/particle.h"

class EntityParticle : public Entity
{
public:
	std::vector<Particle*> particles;
	Mesh* mesh;

	float ttl, entity_ttl, freq, current_freq, size;
	std::string texture;
	Vector4* color;
	int random;

	EntityParticle();
	~EntityParticle();

	void processEvent(std::string name, void* data = NULL);
};

#endif