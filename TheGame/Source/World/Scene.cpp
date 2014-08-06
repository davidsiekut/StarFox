#include "Renderer.h"
#include "Scene.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

void Scene::Initialize()
{
	printf("Initializing scene...\n");

	// init any scene variable such as lights here
}

void Scene::Update(float dt)
{
	// update actors

	// physics checks go here
}

void Scene::Draw()
{
	Renderer::BeginFrame();

	glm::mat4 W(1.0f);
	// TODO implement this into its own camera class
	glm::mat4 V = glm::lookAt(glm::vec3(0.f, -5.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
	glm::mat4 P = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		// check it out
		GLuint program = Renderer::GetShaderProgramID((*it)->GetShaderType());

		// dial it in
		W = (*it)->GetWorldMatrix();

		WorldMatrixID = glGetUniformLocation(program, "WorldTransform");
		ViewMatrixID = glGetUniformLocation(program, "ViewTransform");
		ProjMatrixID = glGetUniformLocation(program, "ProjTransform");
		LightPositionID = glGetUniformLocation(program, "lPosition_World");
		LightColorID = glGetUniformLocation(program, "lColor");
		LightAttenuationID = glGetUniformLocation(program, "lAttenuation");
		MaterialID = glGetUniformLocation(program, "materialCoefficients");

		// amp it up!
		glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &W[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

		glUniform4f(MaterialID, (*it)->GetMaterialCoefficients().x, (*it)->GetMaterialCoefficients().y, (*it)->GetMaterialCoefficients().z, (*it)->GetMaterialCoefficients().w);
		//glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
		//glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
		//glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);

		(*it)->Draw();
	}

	Renderer::EndFrame();
}