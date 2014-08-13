#pragma once

class ParticleSystem
{
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

public:
	void SortParticles();
	int FindUnusedParticle() { return unusedParticle; }
	void Update(float dt);

private:
	int lastUsed = 0;
	int unusedParticle;
	Particle Container[maxParticles];
};