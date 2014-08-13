#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "ParticleSystem.h"
#include "GameTime.h"
#include "Camera.h"

using namespace glm;

struct Particle{
	glm::vec3 position, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float lifeRemaining;
	float distToCamera;

	bool operator<(const Particle& that) const {
		return this->distToCamera > that.distToCamera; // Far are drawn first
	}
};

int lastUsed;
const int maxParticles = 1000;

Particle Container[maxParticles];

void SortParticles()
{
	std::sort(&Container[0], &Container[maxParticles]);
}

int FindUnusedParticle(){

	for (int i = lastUsed; i < maxParticles; i++){
		if (Container[i].lifeRemaining < 0){
			lastUsed = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsed; i++){
		if (Container[i].lifeRemaining < 0){
			lastUsed = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void ParticleSystem::Update(float dt)
{
	// Generate 10 new particle each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(dt*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
	{
		newparticles = (int)(0.016f*10000.0);
	}

	for (int i = 0; i < newparticles; i++) {
		int index = FindUnusedParticle();
		Container[index].lifeRemaining = 3.0f; //Lives for 3 seconds. Can be changed to longer
		Container[index].position = glm::vec3(0, 0, -20.0f);

		// The spread between each particle, can be changed if needed
		float particleSpread = 2.0f;
		glm::vec3 mainDirection = glm::vec3(0.0f, 0.0f, 0.0f); // No original speed in particles on creation

		//VERY STRAIGHTFORWARD WAY TO GENERATE A DIRECTION FOR EACH PARTICLE CHANGE IF YOU CAN
		glm::vec3 randomDirection = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
			);

		Container[index].speed = mainDirection + randomDirection*particleSpread;

		// Each particle starts as white and SHOULD turn orange... will need to be changed if lifeRemaining is changed
		Container[index].r = 256;
		Container[index].g = 256 - 30 * dt;
		Container[index].b = 256 - 85 * dt;
		Container[index].a = 1 - 0.15f * dt;

		// Random size for each particle
		Container[index].size = (rand() % 1000) / 2000.0f + 0.1f;
	}
		

	int partCount = 0;

	for (int i = 0; i < maxParticles; i++){

		Particle& p = Container[i]; // shortcut

		if (p.lifeRemaining > 0.0f){

			// Decrease life of particle
			p.lifeRemaining -= dt;
			if (p.lifeRemaining > 0.0f){

				// Particles fly in all directions with no collision between each other
				p.speed += glm::vec3((rand() % 3 - 1)*3.0f, (rand() % 3 - 1)*3.0f, (rand() % 3 - 1)*3.0f) * dt * 0.5f;
				p.position += p.speed * dt;
				p.distToCamera = glm::length2(p.position - CameraPosition);

				// Fill the GPU buffer
				g_particule_position_size_data[4 * partCount + 0] = p.position.x;
				g_particule_position_size_data[4 * partCount + 1] = p.position.y;
				g_particule_position_size_data[4 * partCount + 2] = p.position.z;

				g_particule_position_size_data[4 * partCount + 3] = p.size;

				g_particule_color_data[4 * partCount + 0] = p.r;
				g_particule_color_data[4 * partCount + 1] = p.g;
				g_particule_color_data[4 * partCount + 2] = p.b;
				g_particule_color_data[4 * partCount + 3] = p.a;

			}
			else
			{
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.distToCamera = -1.0f;
			}

			partCount++;
		}
	}

	SortParticles();
}