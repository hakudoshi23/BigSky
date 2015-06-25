#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "../framework.h"
#include "../mesh.h"
#include "particle.h"

class ParticleManager
{
public:
	static ParticleManager *getInstance();

	std::vector<Particle*> particles;
	Mesh* mesh;
	
	Vector3 rand;

	~ParticleManager();

	void render();
	void update(double delta);
	
	void addParticle( Particle* p );
	void clear();
private:
	ParticleManager();
};

#endif