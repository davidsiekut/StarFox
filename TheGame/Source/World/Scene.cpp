#include "Arwing.h"
#include "Chunk.h"
#include "Renderer.h"
#include "Scene.h"
#include "../Camera/ThirdPersonCamera.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

void Scene::Initialize()
{
	printf("[Scene] Initializing...\n");
	
	// init any scene variable such as lights here

	// defer loading to here
	a = new Arwing(NULL);
	a->SetPosition(glm::vec3(0.f, 5.f, 0.f));
	AddEntity(a);
<<<<<<< HEAD
	camera = new ThirdPersonCamera(glm::vec3(0.0f, 0.0f, -25.0f), a);
=======

	// load initial level geometry
	for (unsigned int i = 0; i < 20; i++)
	{
		Chunk* c = new Chunk(NULL);
		c->SetPosition(glm::vec3(0.f, 0.f, i * Chunk::CHUNK_DEPTH));
		AddEntity(c);

	}
	lastChunk = 10;
>>>>>>> 7bdae7188d2aaf47581d839b868fd552b7bd98b3
}

void Scene::Update(float dt)
{
	// update actors
	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		(*it)->Update(dt);
	}
<<<<<<< HEAD
=======

	//update terrain
	glm::vec3 pos = a->GetPosition();

	printf("%f\n", pos.z);

	if (pos.z / 10 > lastChunk)
	{
		printf("Creating chunk %i", lastChunk);
		Chunk* c = new Chunk(NULL);
		c->SetPosition(glm::vec3(0.f, 0.f, (lastChunk + 1) * Chunk::CHUNK_DEPTH));
		AddEntity(c);
		lastChunk++;
	}



>>>>>>> 7bdae7188d2aaf47581d839b868fd552b7bd98b3
	// physics checks go here
}

void Scene::Draw()
{
	Renderer::BeginFrame();
	glm::mat4 W(1.0f);

<<<<<<< HEAD
	// Get the View and Project Matrices
	glm::mat4 V = camera->GetViewMatrix();
	glm::mat4 P = camera->GetProjectionMatrix();
=======
	glm::vec3 from = glm::vec3(a->GetPosition().x, a->GetPosition().y + 5.f, a->GetPosition().z - 30.f);
	glm::vec3 to = glm::vec3(a->GetPosition().x, a->GetPosition().y, a->GetPosition().z + 10.f);

	// TODO implement this into its own camera class
	glm::mat4 V = glm::lookAt(from, to, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 P = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
>>>>>>> 7bdae7188d2aaf47581d839b868fd552b7bd98b3

	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		// check it out
		GLuint program = Renderer::GetShaderProgramID((*it)->GetShaderType());
		glUseProgram(program);

		// Get the World Matrix
		W = (*it)->GetWorldMatrix();

		GLuint WorldMatrixID = glGetUniformLocation(program, "WorldTransform");
		GLuint ViewMatrixID = glGetUniformLocation(program, "ViewTransform");
		GLuint ProjMatrixID = glGetUniformLocation(program, "ProjTransform");

		GLuint lPositionID = glGetUniformLocation(program, "lPosition_World");
		GLuint lColorID = glGetUniformLocation(program, "lColor");
		GLuint lAttenuationID = glGetUniformLocation(program, "lAttenuation");
		GLuint materialCoefficientsID = glGetUniformLocation(program, "materialCoefficients");

		// amp it up!
		glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &W[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

		glm::vec4 materialCoefficients = (*it)->GetMaterialCoefficients();

		glUniform4f(lPositionID, 0.0f, -1.0f, 0.0f, 0.0f);
		glUniform3f(lColorID, 1.0f, 1.0f, 1.0f);
		glUniform3f(lAttenuationID, 0.0f, 0.0f, 0.02f);
		glUniform4f(materialCoefficientsID, materialCoefficients.x, materialCoefficients.y, materialCoefficients.z, materialCoefficients.w);

		(*it)->Draw();
	}

	Renderer::EndFrame();
}

void Scene::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}