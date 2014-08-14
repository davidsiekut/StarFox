#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ParticleSystem.h"
#include "Scene.h"
#include "TextureHelper.h"

const GLfloat ParticleSystem::SQUARE_VERTICES[] = 
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
};

int ParticleSystem::textureID = -1;

ParticleSystem::ParticleSystem(Entity* parent, float particleLifetime, float systemLifetime, float zSpeed) : Entity(parent)
{
	this->shaderType = SHADER_PARTICLES;
	this->particleLifeTime = particleLifetime;
	this->systemLifetime = systemLifetime;
	this->zSpeed = zSpeed;
	this->currentLifetime = systemLifetime;

	// Create buffers on GPU for square vertices
	glGenBuffers(1, &squareBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, squareBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SQUARE_VERTICES), SQUARE_VERTICES, GL_STATIC_DRAW);


	// Pre calculate size of particle buffer
	particleBufferSize = maxParticles * (2 * sizeof(glm::vec4));

	// Create buffers on GPU for particles
	glGenBuffers(1, &particleBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, particleBufferID);
	glBufferData(GL_ARRAY_BUFFER, particleBufferSize, NULL, GL_STREAM_DRAW);

	if (textureID == -1)
	{
		textureID = TextureHelper::LoadDDS("../Assets/Textures/particle.DDS");
	}
}

ParticleSystem::~ParticleSystem()
{
	//printf("[ParticleSystem] Deleting and cleaning up...\n");
	glDeleteBuffers(1, &squareBufferID);
	glDeleteVertexArrays(1, &squareBufferID);

	glDeleteBuffers(1, &particleBufferID);
	glDeleteVertexArrays(1, &particleBufferID);
}

void ParticleSystem::SortParticles()
{
	std::sort(&Container[0], &Container[maxParticles]);
}

int ParticleSystem::FindUnusedParticle()
{
	for (int i = lastUsed; i < maxParticles; i++)
	{
		if (Container[i].lifeRemaining < 0)
		{
			lastUsed = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsed; i++)
	{
		if (Container[i].lifeRemaining < 0)
		{
			lastUsed = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void ParticleSystem::Update(float dt)
{
	if (systemLifetime > 0)
	{
		if (currentLifetime < 0)
		{
			markedForDeletion = true;
		}
		else
		{
			currentLifetime -= dt;
		}
	}

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
		Container[index].lifeRemaining = particleLifeTime; //Lives for 3 seconds. Can be changed to longer
		Container[index].position = GetPositionWorld();

		// The spread between each particle, can be changed if needed
		float particleSpread = 4.0f;
		glm::vec3 mainDirection = glm::vec3(0.0f, 0.0f, zSpeed); // No original speed in particles on creation

		//VERY STRAIGHTFORWARD WAY TO GENERATE A DIRECTION FOR EACH PARTICLE CHANGE IF YOU CAN
		glm::vec3 randomDirection = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
			);

		Container[index].speed = mainDirection + randomDirection*particleSpread;

		// Each particle starts as white and SHOULD turn orange... will need to be changed if lifeRemaining is changed
		Container[index].r = 1;
		Container[index].g = 1 - (1/(3*particleLifeTime) * dt);
		Container[index].b = 1 - (1/particleLifeTime * dt);
		Container[index].a = 1 - (9/(20*particleLifeTime) * dt);

		// Random size for each particle
		Container[index].size = (rand() % 1000) / 2000.0f + 0.1f;
	}
		

	int partCount = 0;

	Vertex particleBuffer[maxParticles];

	for (int i = 0; i < maxParticles; i++){

		Particle& p = Container[i]; // shortcut

		if (p.lifeRemaining > 0.0f){

			// Decrease life of particle
			p.lifeRemaining -= dt;
			if (p.lifeRemaining > 0.0f){

				// Particles fly in all directions with no collision between each other
				p.speed += glm::vec3((rand() % 3 - 1)*3.0f, (rand() % 3 - 1)*3.0f, (rand() % 3 - 1)*3.0f) * dt * 0.5f;
				p.position += p.speed * dt;
				p.distToCamera = glm::length2(p.position - Scene::GetInstance().GetGPCamera()->GetPosition());

				// Turn particles from white to orange
				p.r -= 0;
				p.g -= (1 / (3 * particleLifeTime)) * dt;
				p.b -= (1 / particleLifeTime) * dt;
				p.a -= (9 / (20 * particleLifeTime)) * dt;

				// Fill the GPU buffer
				particleBuffer[partCount].xyzs = glm::vec4(p.position.x, p.position.y, p.position.z, p.size);
				particleBuffer[partCount].color = glm::vec4(p.r, p.g, p.b, p.a);
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

	glBindBuffer(GL_ARRAY_BUFFER, particleBufferID);
	glBufferData(GL_ARRAY_BUFFER, particleBufferSize, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleBufferSize, particleBuffer);
}

void ParticleSystem::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//GLuint program = Renderer::GetInstance().GetShaderProgramID(this->shaderType);
	GLuint program = Renderer::GetInstance().GetShaderProgramID(this->GetShaderType());
	glUseProgram(program);

	glm::mat4 W = GetWorldMatrix();
	GLuint WorldMatrixID = glGetUniformLocation(program, "WorldTransform");
	glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &W[0][0]);

	GLuint CameraRight_worldspace_ID = glGetUniformLocation(program, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(program, "CameraUp_worldspace");

	glm::mat4 ViewMatrix = Scene::GetInstance().GetGPCamera()->GetViewMatrix();
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	GLuint TextureSamplerID = glGetUniformLocation(program, "TextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Set sampler to user Texture Unit 0
	glUniform1i(TextureSamplerID, 0);

	glBindVertexArray(particleBufferID);

	// Square Vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, squareBufferID);
	glVertexAttribPointer(0,    // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                      // size
		GL_FLOAT,               // type
		GL_FALSE,               // normalized?
		0,						// stride
		(void*)0                // array buffer offset
		);

	// Position of the center of the particule and size of the square
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particleBufferID);
	glVertexAttribPointer(1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)0 // offset
		);

	// Position of the center of the particule and size of the square
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particleBufferID);
	glVertexAttribPointer(2,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec4)) // offset
		);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, maxParticles);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// Reset openGL states to regular values after drawing particles.
	glDisable(GL_BLEND);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
}