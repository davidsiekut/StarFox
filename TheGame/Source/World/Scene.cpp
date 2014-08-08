#include "Arwing.h"
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
	Arwing* a = new Arwing(NULL);
	AddEntity(a);
	camera = new ThirdPersonCamera(glm::vec3(0.0f, 0.0f, -25.0f), a);
}

void Scene::Update(float dt)
{
	// update actors
	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		(*it)->Update(dt);
	}
	// physics checks go here
}

void Scene::Draw()
{
	Renderer::BeginFrame();
	glm::mat4 W(1.0f);

	// Get the View and Project Matrices
	glm::mat4 V = camera->GetViewMatrix();
	glm::mat4 P = camera->GetProjectionMatrix();

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

		GLuint LightPositionID = glGetUniformLocation(program, "lPosition_World");
		GLuint lColorID = glGetUniformLocation(program, "lColor");
		GLuint lAttenuationID = glGetUniformLocation(program, "lAttenuation");
		GLuint MaterialID = glGetUniformLocation(program, "materialCoefficients");

		// amp it up!
		glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &W[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

		glm::vec4 materialCoefficients = (*it)->GetMaterialCoefficients();
		glUniform4f(MaterialID, materialCoefficients.x, materialCoefficients.y, materialCoefficients.z, materialCoefficients.w);
		glUniform4f(LightPositionID, 5.0f, 5.0f, -5.0f, 1.0f);
		glUniform3f(lColorID, 1.0f, 1.0f, 1.0f);
		glUniform3f(lAttenuationID, 0.0f, 0.0f, 0.02f);

		(*it)->Draw();
	}

	Renderer::EndFrame();
}

void Scene::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}