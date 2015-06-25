#ifndef PARTICLE_H
#define PARTICLE_H

#include "../includes.h"
#include "../framework.h"

class Particle
{
public:
	Vector3 position, velocity;
	std::string texture;
	float size, ttl;
	Vector4 color;

	Particle();
	~Particle();
};

#endif