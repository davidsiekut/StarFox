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
// Dror Asher Ozgaon
// Quang Tran
//

#include "PewPew.h"
#include "Scene.h"
#include "TextureHelper.h"

const float PewPew::PEWPEW_LIFETIME = 0.5f;
const float PewPew::PEWPEW_SPEED_PLAYER = 290.f;
const float PewPew::PEWPEW_SPEED_ENEMY = 25.f;
const float PewPew::BLOOM_SCALE = 2.75f;

unsigned int PewPew::sphereArrayID = 0;
unsigned int PewPew::sphereBufferID = 0;
unsigned int PewPew::sphereBufferSize = 0;

unsigned int PewPew::laserArrayID = 0;
unsigned int PewPew::laserBufferID = 0;
unsigned int PewPew::laserBufferSize = 0;

float PewPew::size_x = 0;
float PewPew::size_y = 0;

PewPew::PewPew(std::string owner) : Entity(NULL), owner(owner)
{
	if (owner == "PLAYER")
	{
		direction = glm::vec3(0.f, 0.f, 1.f);
	}
	else
	{
		direction = glm::vec3(0.f, 0.f, -1.f);
	}

	Init();
}

PewPew::PewPew(std::string owner, glm::vec3 direction) : Entity(NULL), owner(owner), direction(direction)
{
	Init();
}

void PewPew::Init()
{
	name = "PEWPEW";

	if (owner == "PLAYER")
	{
		this->size = glm::vec3(2.f, 2.f, 10.f);
		this->objPath = "../Assets/Models/pewpew.obj";
		this->textureID = 6;
		this->damage = 1.0f;
	}
	else
	{
		this->SetRotation(glm::vec3(0.f, 1.f, 0.f), 90.f);
		this->size = glm::vec3(0.5f, 0.5f, 0.5f);
		this->objPath = "../Assets/Models/sphere.obj";
		this->textureID = 1;
		this->damage = 10.f;
	}

	this->shaderType = SHADER_BLOOM;
	this->collider = glm::vec3(size.x, size.y, size.z);

	if (sphereArrayID == 0 && owner == "ENEMY")
	{
		Entity::BufferID gpuBufferIds = Initialize(size);
		sphereArrayID = gpuBufferIds.arrayID;
		sphereBufferID = gpuBufferIds.bufferID;
		sphereBufferSize = gpuBufferIds.bufferSize;
	}
	else if (laserArrayID == 0 && owner == "PLAYER")
	{
		Entity::BufferID gpuBufferIds = Initialize(size);
		laserArrayID = gpuBufferIds.arrayID;
		laserBufferID = gpuBufferIds.bufferID;
		laserBufferSize = gpuBufferIds.bufferSize;
	}
		
	if (owner == "ENEMY")
	{
		vertexArrayID = sphereArrayID;
		vertexBufferID = sphereBufferID;
		vertexBufferSize = sphereBufferSize;
	}
	else
	{
		vertexArrayID = laserArrayID;
		vertexBufferID = laserBufferID;
		vertexBufferSize = laserBufferSize;
	}
	

	if (owner == "ENEMY")
	{
		if (size_x == 0)
		{
			std::vector<Vertex> buffer = Entity::LoadVertices();

			// Get the maximum x and y to create a billboard.
			for (std::vector<Vertex>::iterator it = buffer.begin(); it < buffer.end(); it++)
			{
				if (it == buffer.begin())
				{
					size_x = (*it).position.x * size.x;
					size_y = (*it).position.y * size.y;
				}
				else
				{
					if ((*it).position.x * size.x > size_x)
					{
						size_x = (*it).position.x * size.x;
					}
					if ((*it).position.y * size.y > size_y)
					{
						size_y = (*it).position.y * size.y;
					}
				}
			}
		}

		bloom = new Bloom(*this, size_x * BLOOM_SCALE, size_y * BLOOM_SCALE);
		Scene::GetInstance().AddEntity(bloom);
	}
}

PewPew::~PewPew()
{
	if (bloom != nullptr)
	{
		Scene::GetInstance().RemoveEntity(bloom);
	}
}

void PewPew::Update(float dt)
{
	if (owner == "PLAYER")
		position += direction * dt * PEWPEW_SPEED_PLAYER;
	else
		position += direction * dt * PEWPEW_SPEED_ENEMY;

	timeElapsed += dt;

	// Player pewpews have a lifetime. Enemy pewpews will be deleted when they go behind the player.
	if ((timeElapsed > PEWPEW_LIFETIME && owner == "PLAYER") || Scene::GetInstance().IsGameOver())
	{
		SetMarkedForDeletion();
	}
}

void PewPew::OnCollision(Entity* other)
{
	if (other->GetName() != owner && other->GetName() != "PEWPEW")
	{
		SetMarkedForDeletion();
	}
}
