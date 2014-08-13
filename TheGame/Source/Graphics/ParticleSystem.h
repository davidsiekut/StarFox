#pragma once

#include "Entity.h"

#define maxParticles 100

class ParticleSystem : public Entity
{
public:
	struct Particle
	{
		glm::vec3 position, speed;
		float r, g, b, a; // Color
		float size, angle, weight, lifeRemaining, distToCamera;

		bool operator<(const Particle& that) const
		{
			return this->distToCamera > that.distToCamera; // Draws farthest from camera first
		}
	};

	ParticleSystem(Entity* parent, float particleLifetime, float systemLifetime, float zSpeed);
	~ParticleSystem();

	void SortParticles();
	int FindUnusedParticle();
	virtual void Update(float dt);
	virtual void Draw();

private:
	int lastUsed = 0;
	float particleLifeTime;
	float systemLifetime;
	float currentLifetime;
	float zSpeed;

	unsigned int particleBufferID;
	unsigned int particleBufferSize;

	unsigned int squareBufferID;
	unsigned int squareBufferSize;

	Particle Container[maxParticles];

	struct Vertex
	{
		glm::vec4 xyzs;
		glm::vec4 color;
	};

	static const GLfloat SQUARE_VERTICES[];
	static int textureID;
};