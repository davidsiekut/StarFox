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
	static void SortParticles();
	static int FindUnusedParticle() { return unusedParticle; }
	static void Update(float dt);

private:
	int lastUsed = 0;
	static int unusedParticle;
};