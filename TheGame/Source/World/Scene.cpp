//#include "Arwing.h"
//#include "Chunk.h"
#include "Cube.h"
#include "Renderer.h"
#include "Scene.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

const unsigned int Scene::TERRAIN_PRELOAD = 5;
const unsigned int Scene::TERRAIN_LOADAHEAD = 5;

void Scene::Initialize()
{
	printf("[Scene] Initializing...\n");

	// init any scene variable such as lights here

	// defer loading to here
	a = new Arwing(NULL);
	a->SetPosition(glm::vec3(0.f, 5.f, 0.f));
	AddEntity(a);

	// load initial level geometry
	for (unsigned int i = 0; i < TERRAIN_PRELOAD; i++)
	{
		AddChunk(glm::vec3(0.f, 0.f, i * Chunk::CHUNK_DEPTH));
	}
}

void Scene::Update(float dt)
{
	// update actors
	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		(*it)->Update(dt);
	}

	//update terrain
	if ((a->GetPosition().z / Chunk::CHUNK_DEPTH) + TERRAIN_LOADAHEAD > lastChunk)
	{
		AddChunk(glm::vec3(0.f, 0.f, lastChunk * Chunk::CHUNK_DEPTH));
	}

	// physics checks go here
}

void Scene::Draw()
{
	Renderer::BeginFrame();

	glm::mat4 W(1.0f);

	glm::vec3 from = glm::vec3(a->GetPosition().x, a->GetPosition().y + 5.f, a->GetPosition().z - 30.f);
	glm::vec3 to = glm::vec3(a->GetPosition().x, a->GetPosition().y, a->GetPosition().z + 10.f);

	// TODO implement this into its own camera class
	glm::mat4 V = glm::lookAt(from, to, glm::vec3(0.f, 1.f, 0.f));
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

void Scene::AddChunk(glm::vec3 pos)
{
	printf("[Scene] Creating chunk %i\n", lastChunk);
	Chunk* c = new Chunk(NULL);
	c->SetPosition(pos);
	AddEntity(c);
	chunks.push_back(c);
	lastChunk++;

	int r = rand() % 5;
	if (r == 1)
	{
		printf("[Scene] Creating cube\n");
		Cube* u = new Cube(c, glm::vec3(5.f, 5.f, 5.f));
		u->SetPosition(glm::vec3(0.f, 5.f, 0.f));
		AddEntity(u);
	}
}