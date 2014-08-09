#pragma once

class ParticleSystem
{
	struct Particle
	{
		glm::vec3 position, speed;
		unsigned char r, g, b, a; // Color
		float size, angle, weight, lifeRemaining, distanceFromCamera;

		bool operator<(const Particle& that) const
		{
			return this->distanceFromCamera > that.distanceFromCamera; // Draws farthest from camera first
		}
	};

public:
	int lastUsed = 0;

private:
	void SortParticles();
};