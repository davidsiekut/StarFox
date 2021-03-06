#pragma once

#include "Entity.h"
#include <functional>

#define MAXIMUM_PARTICLES 200

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

	virtual void Update(float dt);
	virtual void Draw();

	void SetParticleSize(float particleSize) { this->particleSize = particleSize; }
	void SetRedInterPolation(std::function<float(float, float, float)> f) { this->RedInterpolation = f; }
	void SetGreenInterPolation(std::function<float(float, float, float)> f) { this->GreenInterpolation = f; }
	void SetBlueInterPolation(std::function<float(float, float, float)> f) { this->BlueInterpolation = f; }
	void SetInitialColor(glm::vec3 initialColor) { this->initialColor = initialColor; }
	void SetSpread(float spread) { this->spread = spread; }
	void SetMaxParticles(unsigned int maxParticles) { this->maxParticles = maxParticles; }
	void SetMainDirection(glm::vec3 mainDirection) { this->mainDirection = mainDirection; }

	virtual bool IsOpaque() { return false; }

private:

	static const GLfloat SQUARE_VERTICES[];
	static int textureID;

	int lastUsed = 0;
	float particleLifeTime;
	float systemLifetime;
	float currentLifetime;
	float particleSize;
	glm::vec3 initialColor;
	float spread;
	unsigned int maxParticles;
	glm::vec3 mainDirection;

	std::function<float(float, float, float)> RedInterpolation;
	std::function<float(float, float, float)> GreenInterpolation;
	std::function<float(float, float, float)> BlueInterpolation;

	unsigned int particleBufferID;
	unsigned int particleBufferSize;

	unsigned int squareBufferID;
	unsigned int squareBufferSize;

	Particle Container[MAXIMUM_PARTICLES];

	struct Vertex
	{
		glm::vec4 xyzs;
		glm::vec4 color;
	};

	void SortParticles();
	int FindUnusedParticle();
};