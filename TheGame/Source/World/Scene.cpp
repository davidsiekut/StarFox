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
#include <UIElement.h>
#include <HealthBar.h>

const unsigned int Scene::TERRAIN_PRELOAD = 5;
const unsigned int Scene::TERRAIN_LOADAHEAD = 5;
#define SCORE_SPAWN_BOSS 2000

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
	camera = new GameplayCamera(25.f, a);

	// load initial level geometry
	for (unsigned int i = 0; i < TERRAIN_PRELOAD; i++)
	{
		AddChunk(glm::vec3(0.f, 0.f, i * Chunk::CHUNK_DEPTH));
	}

	SkyBox* skybox = new SkyBox(NULL, a);
	skybox->SetPosition(glm::vec3(0, 200, a->GetPosition().z + 500));
	AddEntity(skybox);

	HealthBar* ui_healthBar = new HealthBar(glm::vec3(50, 50, 0), glm::vec3(100, 50, 1));
	uiElements.push_back(ui_healthBar);

}

void Scene::LoadTextures()
{
	std::string texturesToLoad[] = { "default.jpg", "dolan.jpg", "building.jpg", "grass.jpg", "sky.jpg", "dolan_ultra.jpg", "pewpew.jpg", "UI/healthbar_empty.jpg", "UI/healthbar_green.jpg", "UI/healthbar_orange.jpg", "UI/healthbar_red.jpg" };

	for (unsigned int i = 0; i < MAXTEXTURES; i++)
	{
		printf("[Scene] Loading texture %s\n", texturesToLoad[i].c_str());
		textures[i].LoadTexture("../Assets/Textures/" + texturesToLoad[i], true);
		textures[i].SetFilterType(FILTER_MIN_LINEAR_MIPMAP_LINEAR, FILTER_MAG_NEAREST);
	}

}

void Scene::Update(float dt)
{
	for (std::vector<UIElement*>::iterator it = uiElements.begin(); it < uiElements.end(); ++it)
	{
		(*it)->Update(dt);
	}

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

	DrawUI(uiElements);
	DrawEntities(entities);
	DrawEntities(transparentEntities);

	Renderer::GetInstance().EndFrame();
}

void Scene::DrawUI(std::vector<UIElement*> &uielem)
{
	glm::mat4 V = glm::mat4(1.0);
	glm::mat4 P = glm::ortho(0.f, (float)800, (float)0, 600.f, 0.1f, 1000.f);

	//glDisable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	for (std::vector<UIElement*>::iterator it = uiElements.begin(); it < uiElements.end(); ++it)
	{
		GLuint program = Renderer::GetInstance().GetShaderProgramID((*it)->GetShaderType());
		glUseProgram(program);

		GLuint WorldMatrixID = glGetUniformLocation(program, "WorldTransform");
		GLuint ViewMatrixID = glGetUniformLocation(program, "ViewTransform");
		GLuint ProjMatrixID = glGetUniformLocation(program, "ProjTransform");
		GLuint samplerID = glGetUniformLocation(program, "sampler"); // for texture2d

		//glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &W[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);
		glUniform1i(samplerID, 0); // for texture2d

		textures[(*it)->GetTextureID()].Bind(); // for texture2d

		(*it)->Draw();
	}
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
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

		(*it)->Draw();
	}

	Renderer::GetInstance().EndFrame();
}