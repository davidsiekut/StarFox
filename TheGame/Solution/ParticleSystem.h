#pragma once

#include "Entity.h"

#define maxParticles 10000

class ParticleSystem : public Entity
{
public:
	struct Particle
	{
		glm::vec3 position, speed;
		unsigned char r, g, b, a; // Color
		float size, angle, weight, lifeRemaining, distToCamera;

		bool operator<(const Particle& that) const
		{
			return this->distToCamera > that.distToCamera; // Draws farthest from camera first
		}
	};

	ParticleSystem();
	~ParticleSystem();

	void SortParticles();
	int FindUnusedParticle();
	virtual void Update(float dt);
	virtual void Draw();

private:
	int lastUsed = 0;
	unsigned int particleBufferID;
	unsigned int particleBufferSize;
	Particle Container[maxParticles];

	struct Vertex
	{
		glm::vec3 squareVertices;
		glm::vec4 xyzs;
		glm::vec4 color;
	};
};