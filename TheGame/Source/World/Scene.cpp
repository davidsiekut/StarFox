#include "Enemy.h"
#include "EnemyFactory.h"
#include "BuildingFactory.h"
#include "Chunk.h"
#include "Cube.h"
#include "GameplayCamera.h"
#include "InputManager.h"
#include "PewPew.h"
#include "Physics.h"
#include "Renderer.h"
#include "Scene.h"
#include "SkyBox.h"
#include "Texture.h"
#include "ThirdPersonCamera.h"
#include "UltraBoss.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <time.h>

const unsigned int Scene::TERRAIN_PRELOAD = 5;
const unsigned int Scene::TERRAIN_LOADAHEAD = 5;
#define SCORE_SPAWN_BOSS 1000

#define MAXTEXTURES 7
Texture textures[MAXTEXTURES];

Scene::Scene()
{

}

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
	enemyFactory = new EnemyFactory(a);
	camera = new GameplayCamera(25.f, a);//new ThirdPersonCamera(25.f, a); //new GameplayCamera(25.f, a);

	// load initial level geometry
	for (unsigned int i = 0; i < TERRAIN_PRELOAD; i++)
	{
		AddChunk(glm::vec3(0.f, 0.f, i * Chunk::CHUNK_DEPTH));
	}

	SkyBox* skybox = new SkyBox(NULL, a);
	skybox->SetPosition(glm::vec3(0, 200, a->GetPosition().z + 500));
	AddEntity(skybox);
}

void Scene::LoadTextures()
{
	std::string texturesToLoad[] = { "default.jpg", "dolan.jpg", "building.jpg", "grass.jpg", "sky.jpg", "dolan_ultra.jpg", "pewpew.jpg" };

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

	UpdateEntities(entities, dt);
	UpdateEntities(transparentEntities, dt);

	// update terrain
	if ((a->GetPosition().z / Chunk::CHUNK_DEPTH) + TERRAIN_LOADAHEAD > lastChunk)
	{
		AddChunk(glm::vec3(0.f, 0.f, lastChunk * Chunk::CHUNK_DEPTH));
	}

	if (score < SCORE_SPAWN_BOSS)
	{
		// spawn new enemies?
		enemyFactory->SpawnCheck(dt);
	}
	else if (!bossSpawned)
	{
		bossSpawned = true;
		enemyFactory->SpawnUltraBoss();
	}

	// if enemies can attack, do so and add pewpews to entity list
	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		if ((*it)->GetName().compare("ENEMY") == 0)
		{
			if (((Enemy*)(*it))->attackCooldown <= 0)
			{
				float meetingTime = ((*it)->GetPosition().z - a->GetPosition().z) / (a->speedZ - PewPew::PEWPEW_SPEED_ENEMY);
				glm::vec3 target = a->GetPosition();
				target.z += meetingTime * a->speedZ;

				glm::vec3 direction = glm::normalize(target - (*it)->GetPosition());

				PewPew* pewpew = new PewPew("ENEMY", direction);
				pewpew->SetPosition(glm::vec3((*it)->GetPosition().x, (*it)->GetPosition().y, (*it)->GetPosition().z));
				queued.push_back(pewpew);
				((Enemy*)(*it))->attackCooldown = 2.f;
			}
		}
		else if ((*it)->GetName().compare("BOSS") == 0)
		{
			if (((Enemy*)(*it))->attackCooldown <= 0)
			{
				glm::vec3 target = a->GetPosition();
				target.x += (rand() % 150 * 2) - 150; // spread range
				target.y += (rand() % 150 * 2) - 150;

				glm::vec3 directionL = glm::normalize(target - (*it)->GetPosition());
				glm::vec3 directionR = glm::normalize(target - (*it)->GetPosition());

				PewPew* pewpewL = new PewPew("ENEMY", directionL);
				PewPew* pewpewR = new PewPew("ENEMY", directionR);
				pewpewL->SetPosition(glm::vec3((*it)->GetPosition().x - 17, (*it)->GetPosition().y - 5, (*it)->GetPosition().z));
				pewpewR->SetPosition(glm::vec3((*it)->GetPosition().x + 17, (*it)->GetPosition().y - 5, (*it)->GetPosition().z));
				queued.push_back(pewpewL);
				queued.push_back(pewpewR);
				((Enemy*)(*it))->attackCooldown = UltraBoss::BOSS_ATTACK_CD;
			}
		}
	}

	for (std::vector<Entity*>::iterator it = queued.begin(); it < queued.end(); ++it)
	{
		entities.push_back(*it);
	}
	queued.clear();
}

void Scene::Draw()
{
	Renderer::GetInstance().BeginFrame();

	glm::mat4 V = camera->GetViewMatrix();

	// draw ui
	glm::mat4 P = glm::ortho(0.f, (float)800, (float)600, 0.f, 0.1f, 1000.f);

	/*Note that if you are using an orthographic projection that is windowwidth by windowheight in size, 
	then drawing a quad as small as you are drawing will pretty much just draw a single pixel or two, 
	since the viewport is now windowwidth units wide and windowheight units tall, and you are drawing a 
	quad 1 unit wide and 1/2 unit tall. You need to draw your objects larger in order to see anything useful.*/

	DrawEntities(entities);
	DrawEntities(transparentEntities);

	Renderer::GetInstance().EndFrame();
}

void Scene::AddEntity(Entity* entity)
{
	if (entity->IsOpaque())
	{
		entities.push_back(entity);
	}
	else
	{
		transparentEntities.push_back(entity);
	}
}

void Scene::RemoveEntity(Entity* entity)
{
	std::vector<Entity*>::iterator it = std::find(entities.begin(), entities.end(), entity);
	if (it != entities.end())
	{
		delete *it;
		it = entities.erase(it);
	}
	else
	{
		it = std::find(transparentEntities.begin(), transparentEntities.end(), entity);
		if (it != transparentEntities.end())
		{
			delete *it;
			transparentEntities.erase(it);
		}
	}
}

void Scene::AddChunk(glm::vec3 pos)
{
	//printf("[Scene] Creating chunk %i\n", lastChunk);
	Chunk* c = new Chunk(NULL);
	c->SetPosition(pos);
	AddEntity(c);
	chunks.push_back(c);
	lastChunk++;

	if (lastChunk % 2 == 0 && !bossSpawned && lastChunk > 3)
	{
		BuildingFactory::GetInstance().GenerateBuilding(pos);
	}
}

void Scene::GameOver()
{
	if (!gameOver)
	{
		gameOver = true;
		printf("[Player] Game over\n");
		this->camera = new ThirdPersonCamera(25.f, a);
		glfwSetInputMode(WindowManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		InputManager::SetDisabled(true);
	}
}

void Scene::UpdateEntities(std::vector<Entity*> &entities, float dt)
{
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
			// update entity
			(*it)->Update(dt);

			// physics check (only check things that will probably collide)
			if ((*it)->GetName().compare("PLAYER") == 0 ||
				(*it)->GetName().compare("PEWPEW") == 0 ||
				(*it)->GetName().compare("ENEMY") == 0 ||
				(*it)->GetName().compare("BUILDING") == 0
				)
			{
				for (std::vector<Entity*>::iterator itt = entities.begin(); itt < entities.end(); ++itt)
				{
					if (*it != *itt && Physics::CheckAABBCollision(*it, *itt))
					{
						(*it)->OnCollision(*itt);
						(*itt)->OnCollision(*it);
					}
				}
			}

			it++;
		}
	}
}

void Scene::DrawEntities(std::vector<Entity*> &entities)
{
	glm::mat4 V = camera->GetViewMatrix();
	glm::mat4 P = camera->GetProjectionMatrix();

	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		//GLuint program = Renderer::GetInstance().GetShaderProgramID(this->shaderType);
		GLuint program;
		if (Renderer::GetInstance().GetCurrentShader() > -1)
		{
			program = Renderer::GetInstance().GetShaderProgramID(Renderer::GetInstance().GetCurrentShader());
			textures[0].Bind();
		}
		else
		{
			program = Renderer::GetInstance().GetShaderProgramID((*it)->GetShaderType());
			textures[(*it)->GetTextureID()].Bind(); // for texture2d
		}
		glUseProgram(program);

		GLuint ViewMatrixID = glGetUniformLocation(program, "ViewTransform");
		GLuint ProjMatrixID = glGetUniformLocation(program, "ProjTransform");

		GLuint lAttenuationID = glGetUniformLocation(program, "lAttenuation");
		GLuint lColorID = glGetUniformLocation(program, "lColor");
		GLuint lPositionID = glGetUniformLocation(program, "lPosition_World");
		GLuint samplerID = glGetUniformLocation(program, "sampler"); // for texture2d

		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

		if (lightingMode == 0)
		{
			glUniform3f(lAttenuationID, 0.0f, 0.0f, 0.02f);
			glUniform3f(lColorID, 1.0f, 1.0f, 1.0f);
			glUniform4f(lPositionID, a->GetPosition().x, a->GetPosition().y - 0.3f, a->GetPosition().z - 3.f, 1.f);
		}
		else if (lightingMode == 1)
		{
			glUniform3f(lAttenuationID, 0.0f, 0.0f, 0.02f);
			glUniform3f(lColorID, 1.0f, 1.0f, 1.0f);
			glUniform4f(lPositionID, 0.f, -1.f, 0.f, 0.f);
		}
		else
		{
			// failsafe fallback
			glUniform3f(lAttenuationID, 0.0f, 0.0f, 0.02f);
			glUniform3f(lColorID, 1.0f, 1.0f, 1.0f);
			glUniform4f(lPositionID, 0.f, -1.f, 0.f, 0.f);
		}

		glUniform1f(samplerID, 0); // for texture2d

		Arwing *arwing = dynamic_cast<Arwing*>(*it);


		if ((arwing != NULL && !(*arwing).isFlashing) || arwing == NULL /*|| 
			checkVisible(P * V * (*it)->GetWorldMatrix(), 
								 (*it)->GetPosition().x, 
								 (*it)->GetPosition().y, 
								 (*it)->GetPosition().z, 
								 (*it)->GetRadius())*/
							)
		{
			(*it)->Draw();
		}
	}
}

bool Scene::checkVisible(const glm::mat4 MVPMatrix, const GLfloat &xPos, const GLfloat &yPos, const GLfloat &zPos, const GLfloat &radius)
{
	// The equation for a plane is: Ax + By + Cz + D = 0, where A, B and C define the plane's normal vector, D is the distance from the origin to the plane,
	// and x, y and z are any points on the plane.. You can plug any point into the equation and if the result is 0 then the point lies on the plane. If the
	// result is greater than 0 then the point is in front of the plane, and if it's negative the point is behind the plane.
	enum term { A = 0, B, C, D };

	glm::vec4 leftPlane;
	GLfloat distance, length;

	/*
	leftPlane[A] = MVPMatrix[3] + MVPMatrix[0];
	leftPlane[B] = MVPMatrix[7] + MVPMatrix[4];
	leftPlane[C] = MVPMatrix[11] + MVPMatrix[8];
	leftPlane[D] = MVPMatrix[15] + MVPMatrix[12];
	*/

	leftPlane = MVPMatrix[3] + MVPMatrix[0];

	// Normalise the plane
	length = sqrtf(leftPlane[A] * leftPlane[A] + leftPlane[B] * leftPlane[B] + leftPlane[C] * leftPlane[C]);
	leftPlane[A] /= length;
	leftPlane[B] /= length;
	leftPlane[C] /= length;
	leftPlane[D] /= length;

	// Check the point's location with respect to the left plane of our viewing frustrum
	distance = leftPlane[A] * xPos + leftPlane[B] * yPos + leftPlane[C] * zPos + leftPlane[D];
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the left plane
	}

	// Check the point's location with respect to the right plane of our viewing frustum
	glm::vec4 rightPlane;
	rightPlane = MVPMatrix[3] - MVPMatrix[0];
	
	/*
	rightPlane[B] = MVPMatrix[7] - MVPMatrix[4];
	rightPlane[C] = MVPMatrix[11] - MVPMatrix[8];
	rightPlane[D] = MVPMatrix[15] - MVPMatrix[12];*/

	// Normalise the plane
	length = sqrtf(rightPlane[A] * rightPlane[A] + rightPlane[B] * rightPlane[B] + rightPlane[C] * rightPlane[C]);
	rightPlane[A] /= length;
	rightPlane[B] /= length;
	rightPlane[C] /= length;
	rightPlane[D] /= length;

	distance = rightPlane[A] * xPos + rightPlane[B] * yPos + rightPlane[C] * zPos + rightPlane[D];
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the right plane
	}

	// Check the point's location with respect to the bottom plane of our viewing frustum
	glm::vec4 bottomPlane;
	bottomPlane = MVPMatrix[3] + MVPMatrix[1];
	/*bottomPlane[B] = MVPMatrix[7] + MVPMatrix[5];
	bottomPlane[C] = MVPMatrix[11] + MVPMatrix[9];
	bottomPlane[D] = MVPMatrix[15] + MVPMatrix[13];*/

	// Normalise the plane
	length = sqrtf(bottomPlane[A] * bottomPlane[A] + bottomPlane[B] * bottomPlane[B] + bottomPlane[C] * bottomPlane[C]);
	bottomPlane[A] /= length;
	bottomPlane[B] /= length;
	bottomPlane[C] /= length;
	bottomPlane[D] /= length;

	distance = bottomPlane[A] * xPos + bottomPlane[B] * yPos + bottomPlane[C] * zPos + bottomPlane[D];
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the bottom plane
	}

	// Check the point's location with respect to the top plane of our viewing frustrum
	glm::vec4 topPlane;
	topPlane = MVPMatrix[3] - MVPMatrix[1];
	
	/*topPlane[B] = MVPMatrix[7] - MVPMatrix[5];
	topPlane[C] = MVPMatrix[11] - MVPMatrix[9];
	topPlane[D] = MVPMatrix[15] - MVPMatrix[13];*/

	// Normalise the plane
	length = sqrtf(topPlane[A] * topPlane[A] + topPlane[B] * topPlane[B] + topPlane[C] * topPlane[C]);
	topPlane[A] /= length;
	topPlane[B] /= length;
	topPlane[C] /= length;
	topPlane[D] /= length;

	distance = topPlane[A] * xPos + topPlane[B] * yPos + topPlane[C] * zPos + topPlane[D];
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the top plane
	}

	// Check the point's location with respect to the near plane of our viewing frustum
	glm::vec4 nearPlane;
	nearPlane = MVPMatrix[3] + MVPMatrix[2];
	/*nearPlane[B] = MVPMatrix[7] + MVPMatrix[6];
	nearPlane[C] = MVPMatrix[11] + MVPMatrix[10];
	nearPlane[D] = MVPMatrix[15] + MVPMatrix[14];*/

	// Normalise the plane
	length = sqrtf(nearPlane[A] * nearPlane[A] + nearPlane[B] * nearPlane[B] + nearPlane[C] * nearPlane[C]);
	nearPlane[A] /= length;
	nearPlane[B] /= length;
	nearPlane[C] /= length;
	nearPlane[D] /= length;

	distance = nearPlane[A] * xPos + nearPlane[B] * yPos + nearPlane[C] * zPos + nearPlane[D];
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the near plane
	}

	// Check the point's location with respect to the far plane of our viewing frustum
	glm::vec4 farPlane;
	farPlane = MVPMatrix[3] - MVPMatrix[2];
	/*farPlane[B] = MVPMatrix[7] - MVPMatrix[6];
	farPlane[C] = MVPMatrix[11] - MVPMatrix[10];
	farPlane[D] = MVPMatrix[15] - MVPMatrix[14];*/

	// Normalise the plane
	length = sqrtf(farPlane[A] * farPlane[A] + farPlane[B] * farPlane[B] + farPlane[C] * farPlane[C]);
	farPlane[A] /= length;
	farPlane[B] /= length;
	farPlane[C] /= length;
	farPlane[D] /= length;

	distance = farPlane[A] * xPos + farPlane[B] * yPos + farPlane[C] * zPos + farPlane[D];
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the far plane
	}

	// If we got here, then the bounding sphere is within at least all six sides of the view frustum, so it's visible and we should draw it!
	return true;
}