#include "Arwing.h"
#include "Enemy.h"
#include "Chunk.h"
#include "Cube.h"
#include "Renderer.h"
#include "Scene.h"
#include "Texture.h"
#include "ThirdPersonCamera.h"
#include "GameplayCamera.h"
#include "EnemyFactory.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <time.h>

const unsigned int Scene::TERRAIN_PRELOAD = 5;
const unsigned int Scene::TERRAIN_LOADAHEAD = 5;

#define MAXTEXTURES 2
Texture textures[MAXTEXTURES];

void Scene::Initialize()
{
	printf("[Scene] Initializing...\n");

	srand((int)time(NULL));

	LoadTextures();

	// player setup
	a = new Arwing(NULL);
	a->SetPosition(glm::vec3(0.f, 10.f, 0.f));
	AddEntity(a);

	// setup some other stuff
	enemyFactory = new EnemyFactory(a, this);
	camera = new GameplayCamera(40.f, a);

	// load initial level geometry
	for (unsigned int i = 0; i < TERRAIN_PRELOAD; i++)
	{
		AddChunk(glm::vec3(0.f, 0.f, i * Chunk::CHUNK_DEPTH));
	}
}

void Scene::LoadTextures()
{
	std::string texturesToLoad[] = { "default.jpg", "dolan.jpg" };

	for (unsigned int i = 0; i < MAXTEXTURES; i++)
	{
		printf("[Scene] Loading texture %s\n", texturesToLoad[i].c_str());
		textures[i].LoadTexture("../Assets/Textures/" + texturesToLoad[i], true);
		textures[i].SetFilterType(FILTER_MIN_LINEAR_MIPMAP_LINEAR, FILTER_MAG_NEAREST);
	}
}

void Scene::Update(float dt)
{
	// update camera
	camera->Update(dt);

	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end();)
	{
		// deletion check
		if ((*it)->markedForDeletion)
		{
			delete *it;
			it = entities.erase(it);
		}
		else if ((*it)->GetPositionWorld().z < a->GetPositionWorld().z - Chunk::CHUNK_DEPTH)
		{
			delete *it;
			it = entities.erase(it);
		}
		else
		{
			// update actors
			(*it)->Update(dt);
			
			// physics check
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
			
			it++;
		}
	}

	// update terrain
	if ((a->GetPosition().z / Chunk::CHUNK_DEPTH) + TERRAIN_LOADAHEAD > lastChunk)
	{
		AddChunk(glm::vec3(0.f, 0.f, lastChunk * Chunk::CHUNK_DEPTH));
	}

	// spawn new enemies?
	enemyFactory->SpawnCheck(dt);
}

void Scene::Draw()
{
	Renderer::BeginFrame();

	glm::mat4 W(1.0f);
	glm::mat4 V = camera->GetViewMatrix();

	// draw ui
	glm::mat4 P = glm::ortho(0.f, (float)800, (float)600, 0.f, 0.1f, 1000.f);

	/*Note that if you are using an orthographic projection that is windowwidth by windowheight in size, 
	then drawing a quad as small as you are drawing will pretty much just draw a single pixel or two, 
	since the viewport is now windowwidth units wide and windowheight units tall, and you are drawing a 
	quad 1 unit wide and 1/2 unit tall. You need to draw your objects larger in order to see anything useful.*/

	P = camera->GetProjectionMatrix(); 
	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		GLuint program = Renderer::GetShaderProgramID((*it)->GetShaderType());
		glUseProgram(program);

		W = (*it)->GetWorldMatrix();

		GLuint WorldMatrixID = glGetUniformLocation(program, "WorldTransform");
		GLuint ViewMatrixID = glGetUniformLocation(program, "ViewTransform");
		GLuint ProjMatrixID = glGetUniformLocation(program, "ProjTransform");

		GLuint lPositionID = glGetUniformLocation(program, "lPosition_World");
		GLuint lColorID = glGetUniformLocation(program, "lColor");
		GLuint lAttenuationID = glGetUniformLocation(program, "lAttenuation");
		GLuint materialCoefficientsID = glGetUniformLocation(program, "materialCoefficients");
		GLuint samplerID = glGetUniformLocation(program, "sampler"); // for texture2d

		glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &W[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

		glm::vec4 materialCoefficients = (*it)->GetMaterialCoefficients();
		textures[(*it)->GetTextureID()].Bind(); // for texture2d

		glUniform4f(lPositionID, 0.0f, -1.0f, 0.0f, 0.0f);
		glUniform3f(lColorID, 1.0f, 1.0f, 1.0f);
		glUniform3f(lAttenuationID, 0.0f, 0.0f, 0.02f);
		glUniform4f(materialCoefficientsID, materialCoefficients.x, materialCoefficients.y, materialCoefficients.z, materialCoefficients.w);
		glUniform1i(samplerID, 0); // for texture2d

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
	//printf("[Scene] Creating chunk %i\n", lastChunk);
	Chunk* c = new Chunk(NULL);
	c->SetPosition(pos);
	AddEntity(c);
	chunks.push_back(c);
	lastChunk++;

	int s = (rand() % 40) - 20;
	// add level geometry to this chunk
	Cube* u = new Cube(c, glm::vec3(5.f, 20.f, 5.f));
	u->SetPosition(glm::vec3(s, 10.f, 0.f));
	AddEntity(u);
	//printf("[Scene] Creating cube at (%f, %f, %f)\n", u->GetPositionWorld().x, u->GetPositionWorld().y, u->GetPositionWorld().z);
}

bool Scene::CheckAABBCollision(Entity* b1, Entity* b2)
{
	glm::vec3 min1 = glm::vec3(
		b1->GetPositionWorld().x - b1->COLLIDE_X / 2,
		b1->GetPositionWorld().y - b1->COLLIDE_Y / 2,
		b1->GetPositionWorld().z - b1->COLLIDE_Z / 2);

	glm::vec3 max1 = glm::vec3(
		b1->GetPositionWorld().x + b1->COLLIDE_X / 2,
		b1->GetPositionWorld().y + b1->COLLIDE_Y / 2,
		b1->GetPositionWorld().z + b1->COLLIDE_Z / 2);

	glm::vec3 min2 = glm::vec3(
		b2->GetPositionWorld().x - b2->COLLIDE_X / 2,
		b2->GetPositionWorld().y - b2->COLLIDE_Y / 2,
		b2->GetPositionWorld().z - b2->COLLIDE_Z / 2);

	glm::vec3 max2 = glm::vec3(
		b2->GetPositionWorld().x + b2->COLLIDE_X / 2,
		b2->GetPositionWorld().y + b2->COLLIDE_Y / 2,
		b2->GetPositionWorld().z + b2->COLLIDE_Z / 2);

	return(max1.x > min2.x &&
		min1.x < max2.x &&
		max1.y > min2.y &&
		min1.y < max2.y &&
		max1.z > min2.z &&
		min1.z < max2.z);
}