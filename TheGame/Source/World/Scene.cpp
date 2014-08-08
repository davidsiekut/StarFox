#include "Arwing.h"
#include "Enemy.h"
#include "Chunk.h"
#include "Cube.h"
#include "Renderer.h"
#include "Scene.h"
#include "ThirdPersonCamera.h"
#include "GameplayCamera.h"
#include "EnemyFactory.h"

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
	a->SetPosition(glm::vec3(0.f, 10.f, 0.f));
	AddEntity(a);

	enemyFactory = new EnemyFactory(a, this);

	camera = new GameplayCamera(40.f, a);

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
	camera->Update(dt);

	// update terrain
	if ((a->GetPosition().z / Chunk::CHUNK_DEPTH) + TERRAIN_LOADAHEAD > lastChunk)
	{
		AddChunk(glm::vec3(0.f, 0.f, lastChunk * Chunk::CHUNK_DEPTH));
	}

// Spawn one enemy every 8 seconds.
	enemyTimer -= dt;
	if (enemyTimer < 0)
	{
		enemyTimer = 8.f;
		if (left)
		{
			enemyFactory->SpawnEnemies(3, EnemyFactory::Direction::LEFT, 12.5f);
			left = !left;
		}
		else
		{
			enemyFactory->SpawnEnemies(3, EnemyFactory::Direction::RIGHT, 12.5f);
			left = !left;
		}
	}	// physics checks go here
	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		std::string s1 = "ARWING";
		std::string s2 = "CHUNK";
		if ((*it)->GetName().compare(s1) && (*it)->GetName().compare(s2))
		{
			// entity is not ARWING or CHUNK
			//printf("Checking entity %s\n", (*it)->GetName().c_str());

			// check intersection of arwing collider and entity
			if (CheckAABBCollision(a, (*it)))
			{
				printf("[Physics] Arwing hit -> %s\n", (*it)->GetName().c_str());
			}
		}
	}
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

	printf("[Scene] Creating cube\n");
	Cube* u = new Cube(c, glm::vec3(10.f, 1.f, 1.f));
	u->SetPosition(glm::vec3(0.f, 5.f, 0.f));
	AddEntity(u);
}

bool Scene::CheckAABBCollision(Entity* b1, Entity* b2)
{
	glm::vec3 min1 = glm::vec3(
		b1->GetPositionWorld().x - b1->COLLIDE_X,
		b1->GetPositionWorld().y - b1->COLLIDE_Y,
		b1->GetPositionWorld().z - b1->COLLIDE_Z);

	glm::vec3 max1 = glm::vec3(
		b1->GetPositionWorld().x + b1->COLLIDE_X,
		b1->GetPositionWorld().y + b1->COLLIDE_Y,
		b1->GetPositionWorld().z + b1->COLLIDE_Z);

	glm::vec3 min2 = glm::vec3(
		b2->GetPositionWorld().x - b2->COLLIDE_X,
		b2->GetPositionWorld().y - b2->COLLIDE_Y,
		b2->GetPositionWorld().z - b2->COLLIDE_Z);

	glm::vec3 max2 = glm::vec3(
		b2->GetPositionWorld().x + b2->COLLIDE_X,
		b2->GetPositionWorld().y + b2->COLLIDE_Y,
		b2->GetPositionWorld().z + b2->COLLIDE_Z);

	return(max1.x > min2.x &&
		min1.x < max2.x &&
		max1.y > min2.y &&
		min1.y < max2.y &&
		max1.z > min2.z &&
		min1.z < max2.z);
}