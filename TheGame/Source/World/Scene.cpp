#include "Renderer.h"
#include "Scene.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

void Scene::Initialize()
{
	printf("[Scene] Initializing...\n");

	// init any scene variable such as lights here

	// defer loading to here
	Entity* e = new Entity(NULL);
	e->SetPosition(glm::vec3(0, 0, 0));
	e->SetScaling(glm::vec3(0.01, 0.01, 0.01));
	AddEntity(e);
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

	// TODO implement this into its own camera class
	glm::mat4 V = glm::lookAt(glm::vec3(5.f, 5.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 P = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		// check it out
		GLuint program = Renderer::GetShaderProgramID((*it)->GetShaderType());
		glUseProgram(program);

		// dial it in
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