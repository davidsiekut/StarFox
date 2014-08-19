//
// COMP 371 Term Project
//
// Created by
// Boutas, Vasiliki   (6220304)
// Di Girolamo, John  (6202918)
// Ozgaon, Dror Asher (6296742)
// Siekut, David      (6329810)
// Tran, Quang        (6339816)
// Wan, Kwok - Chak   (6291643)
//
// Contributions to this file:
//
//

#include "Scene.h"
#include "BuildingFactory.h"
#include "Chunk.h"
#include "Cube.h"
#include "Enemy.h"
#include "EnemyFactory.h"
#include "GameplayCamera.h"
#include "InputManager.h"
#include "PewPew.h"
#include "Physics.h"
#include "Renderer.h"
#include "SkyBox.h"
#include "Texture.h"
#include "ThirdPersonCamera.h"
#include "UltraBoss.h"
#include "WindowManager.h"

#include "UIElement.h"
#include "Digit.h"
#include "HealthBar.h"
#include "Image.h"
#include "WinnerImage.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <time.h>

#define TERRAIN_PRELOAD 5
#define TERRAIN_LOADAHEAD 5
#define SCORE_SPAWN_BOSS 1

#define MAXTEXTURES 28
Texture textures[MAXTEXTURES];

Scene::Scene()
{

}

void Scene::Initialize()
{
	printf("[Scene] Initializing...\n");

	srand((int)time(NULL));

	loadTextures();

	// player setup
	a = new Arwing(NULL);
	a->SetPosition(glm::vec3(0.f, 10.f, 0.f));
	AddEntity(a);

	// setup some other stuff
	enemyFactory = new EnemyFactory(a);
	camera = new GameplayCamera(25.f, a); 
	//camera = new ThirdPersonCamera(25.f, a);
	//camera = new GameplayCamera(25.f, a);

	// load initial level geometry
	for (unsigned int i = 0; i < TERRAIN_PRELOAD; i++)
	{
		AddChunk(glm::vec3(0.f, 0.f, i * Chunk::CHUNK_DEPTH));
	}

	SkyBox* skybox = new SkyBox(NULL, a);
	skybox->SetPosition(glm::vec3(0, 200, a->GetPosition().z + 500));
	AddEntity(skybox);

	buildUI();
}

void Scene::Update(float dt)
{
	// update camera
	camera->Update(dt);

	updateUIElements(uiElements, dt);
	updateEntities(entities, dt);
	updateEntities(transparentEntities, dt);
	updateEnemyAttack();

	// update terrain
	if ((a->GetPosition().z / Chunk::CHUNK_DEPTH) + TERRAIN_LOADAHEAD > lastChunk)
	{
		AddChunk(glm::vec3(0.f, 0.f, lastChunk * Chunk::CHUNK_DEPTH));
	}

	// has boss score been reached?
	if (score < SCORE_SPAWN_BOSS)
	{
		// spawn new enemies?
		enemyFactory->SpawnCheck(dt);
	}
	else if (!bossSpawned)
	{
		// if boss has already spawned, skip all this
		bossSpawned = true;
		boss = enemyFactory->SpawnUltraBoss();

		Image* ui_bossIcon = new Image(glm::vec3(740, 540, 0), glm::vec3(85, 85, 1));
		ui_bossIcon->SetTextureID(15);
		uiElements.push_back(ui_bossIcon);

		Image* ui_bossHealthBarFrame = new Image(glm::vec3(577, 570, 0), glm::vec3(250, 25, 1));
		uiElements.push_back(ui_bossHealthBarFrame);

		HealthBar* ui_bossHealthBar = new HealthBar(glm::vec3(577, 570.5, 0.1), glm::vec3(240, 14.6, 1), "BOSS");
		ui_bossHealthBar->SetTextureID(8);
		uiElements.push_back(ui_bossHealthBar);
	}
}

void Scene::Draw()
{
	Renderer::GetInstance().BeginFrame();

	drawUI(uiElements);
	drawEntities(entities);
	drawEntities(transparentEntities);

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
		this->camera = new ThirdPersonCamera(25.f, a);
		glfwSetInputMode(WindowManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		InputManager::SetDisabled(true);
	}
}

void Scene::GameWon()
{
	if (!gameWon)
	{
		gameWon = true;
		WinnerImage* winner = new WinnerImage(glm::vec3(400, 300, 0), glm::vec3(512, 512, 1));
		uiElements.push_back(winner);
	}
}

void Scene::loadTextures()
{
	std::string texturesToLoad[] = {
		"default.jpg", "dolan.jpg", "building.jpg", "grass.jpg", "sky.jpg", "dolan_ultra.jpg", "pewpew.jpg", 
		"UI/healthbar_empty.jpg", "UI/healthbar_green.jpg", "UI/healthbar_orange.jpg", "UI/healthbar_red.jpg", 
		"UI/player1.jpg", "UI/player2.jpg", "UI/player3.jpg", "UI/player4.jpg",
		"UI/boss.jpg", "UI/score_frame.jpg",
		"UI/zero.jpg", "UI/one.jpg", "UI/two.jpg", "UI/three.jpg", "UI/four.jpg", 
		"UI/five.jpg", "UI/six.jpg", "UI/seven.jpg", "UI/eight.jpg", "UI/nine.jpg", 
		"winner.jpg" };

	for (unsigned int i = 0; i < MAXTEXTURES; i++)
	{
		printf("[Scene] Loading texture %s\n", texturesToLoad[i].c_str());
		textures[i].LoadTexture("../Assets/Textures/" + texturesToLoad[i], true);
		textures[i].SetFilterType(FILTER_MIN_LINEAR_MIPMAP_LINEAR, FILTER_MAG_NEAREST);
	}
}

void Scene::buildUI()
{
	// Player healthbar
	Image* ui_healthBarFrame = new Image(glm::vec3(223, 30, 0), glm::vec3(250, 25, 1));
	uiElements.push_back(ui_healthBarFrame);

	HealthBar* ui_healthBar = new HealthBar(glm::vec3(223, 30.5, 0.1), glm::vec3(240, 14.6, 1), "PLAYER");
	ui_healthBar->SetTextureID(8);
	uiElements.push_back(ui_healthBar);

	// Player icon
	int i = (rand() % 4) + 11;

	Image* ui_playerIcon = new Image(glm::vec3(60, 60, 0), glm::vec3(85, 85, 1));
	ui_playerIcon->SetTextureID(i);
	uiElements.push_back(ui_playerIcon);

	//Scoreboard
	Image* ui_scoreBoard = new Image(glm::vec3(85, 555, 0), glm::vec3(136, 50, 1));
	ui_scoreBoard->SetTextureID(16);
	uiElements.push_back(ui_scoreBoard);

	Digit* ui_zero1 = new Digit(glm::vec3(36, 555, 0.1), glm::vec3(19, 28, 1), 5);
	ui_zero1->SetTextureID(17);
	uiElements.push_back(ui_zero1);

	Digit* ui_zero2 = new Digit(glm::vec3(55, 555, 0.1), glm::vec3(19, 28, 1), 4);
	ui_zero2->SetTextureID(17);
	uiElements.push_back(ui_zero2);

	Digit* ui_zero3 = new Digit(glm::vec3(74, 555, 0.1), glm::vec3(19, 28, 1), 3);
	ui_zero3->SetTextureID(17);
	uiElements.push_back(ui_zero3);

	Digit* ui_zero4 = new Digit(glm::vec3(93, 555, 0.1), glm::vec3(19, 28, 1), 2);
	ui_zero4->SetTextureID(17);
	uiElements.push_back(ui_zero4);

	Digit* ui_zero5 = new Digit(glm::vec3(112, 555, 0.1), glm::vec3(19, 28, 1), 1);
	ui_zero5->SetTextureID(17);
	uiElements.push_back(ui_zero5);

	Digit* ui_zero6 = new Digit(glm::vec3(131, 555, 0.1), glm::vec3(19, 28, 1), 0);
	ui_zero6->SetTextureID(17);
	uiElements.push_back(ui_zero6);
}

void Scene::updateEnemyAttack()
{
	// if enemies can attack, do so and add pewpews to entity list
	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); ++it)
	{
		if ((*it)->GetName().compare("ENEMY") == 0)
		{
			if (((Enemy*)(*it))->GetAttackCooldown() <= 0)
			{
				float meetingTime = ((*it)->GetPosition().z - a->GetPosition().z) / (a->speedZ - PewPew::PEWPEW_SPEED_ENEMY);
				glm::vec3 target = a->GetPosition();
				target.z += meetingTime * a->speedZ;

				glm::vec3 direction = glm::normalize(target - (*it)->GetPosition());

				PewPew* pewpew = new PewPew("ENEMY", direction);
				pewpew->SetPosition(glm::vec3((*it)->GetPosition().x, (*it)->GetPosition().y, (*it)->GetPosition().z));
				queued.push_back(pewpew);
				((Enemy*)(*it))->ResetAttackCooldown();
			}
		}
		else if ((*it)->GetName().compare("BOSS") == 0)
		{
			if (((Enemy*)(*it))->GetAttackCooldown() <= 0)
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
				((Enemy*)(*it))->ResetAttackCooldown();
			}
		}
	}
	for (std::vector<Entity*>::iterator it = queued.begin(); it < queued.end(); ++it)
	{
		entities.push_back(*it);
	}
	queued.clear();
}

void Scene::updateEntities(std::vector<Entity*> &entities, float dt)
{
	for (std::vector<Entity*>::iterator it = entities.begin(); it < entities.end();)
	{
		// deletion check
		if ((*it)->IsMarkedForDeletion())
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

void Scene::updateUIElements(std::vector<UIElement*> &uielements, float dt)
{
	for (std::vector<UIElement*>::iterator it = uiElements.begin(); it < uiElements.end(); ++it)
	{
		(*it)->Update(dt);
	}
}

void Scene::drawEntities(std::vector<Entity*> &entities)
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


		if ((arwing != NULL && !(*arwing).IsFlashing()) || arwing == NULL /*||
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

void Scene::drawUI(std::vector<UIElement*> &uielem)
{
	glm::mat4 V = glm::mat4(1.0);
	glm::mat4 P = glm::ortho(0.f, (float)800, (float)0, 600.f, 0.1f, 1000.f);

	glDepthFunc(GL_ALWAYS);
	//glDisable(GL_DEPTH_TEST);
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
	glDepthFunc(GL_LESS);
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