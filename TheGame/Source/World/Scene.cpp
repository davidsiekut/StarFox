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
#include "ParticleSystem.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <time.h>

const unsigned int Scene::TERRAIN_PRELOAD = 5;
const unsigned int Scene::TERRAIN_LOADAHEAD = 5;

#define MAXTEXTURES 6
Texture textures[MAXTEXTURES];

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

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

	AddEntity(new SkyBox(NULL, a));
}

void Scene::LoadTextures()
{
	std::string texturesToLoad[] = { "default.jpg", "dolan.jpg", "building.jpg", "grass.jpg", "sky.jpg", "dolan_ultra.jpg" };

	for (unsigned int i = 0; i < MAXTEXTURES; i++)
	{
		printf("[Scene] Loading texture %s\n", texturesToLoad[i].c_str());
		textures[i].LoadTexture("../Assets/Textures/" + texturesToLoad[i], true);
		textures[i].SetFilterType(FILTER_MIN_LINEAR_MIPMAP_LINEAR, FILTER_MAG_NEAREST);
	}

}

GLuint Scene::LoadDDS(const char* path)
{
	unsigned char header[124];
	FILE* filePointer;

	filePointer = fopen(path, "rb");
	if (filePointer == NULL)
	{
		return 0;
	}

	char fileCode[4];
	fread(fileCode, 1, 4, filePointer);
	if (strncmp(fileCode, "DDS ", 4) != 0) 
	{
		fclose(filePointer);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, filePointer);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, filePointer);
	/* close the file pointer */
	fclose(filePointer);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}
	free(buffer);

	return textureID;
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
			// update entity
			(*it)->Update(dt);
			
			// physics check (only check things that will probably collide)
			if ((*it)->GetName().compare("PLAYER") == 0 ||
				(*it)->GetName().compare("PEWPEW") == 0 ||
				(*it)->GetName().compare("ENEMY") == 0 ||
				(*it)->GetName().compare("BUILDING") == 0
				)
			{
				for (std::vector<Entity*>::iterator itt = entities.begin(); itt < entities.end();++itt)
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

	// update terrain
	if ((a->GetPosition().z / Chunk::CHUNK_DEPTH) + TERRAIN_LOADAHEAD > lastChunk)
	{
		AddChunk(glm::vec3(0.f, 0.f, lastChunk * Chunk::CHUNK_DEPTH));
	}

	if (score < 1000)
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

		glUniform3f(lAttenuationID, 0.0f, 0.0f, 0.02f);
		glUniform3f(lColorID, 1.0f, 1.0f, 1.0f);
		glUniform4f(lPositionID, 0.f, -1.f, 0.f, 0.f);
		glUniform1f(samplerID, 0); // for texture2d

		(*it)->Draw();
	}

	Renderer::GetInstance().EndFrame();
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

	if (lastChunk > 3 && !bossSpawned)
		BuildingFactory::GetInstance().GenerateBuilding(pos);
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