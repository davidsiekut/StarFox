#include "PewPew.h"
#include "Scene.h"
#include "TextureHelper.h"

const float PewPew::PEWPEW_LIFETIME = 0.5f;
const float PewPew::PEWPEW_SPEED_PLAYER = 290.f;
const float PewPew::PEWPEW_SPEED_ENEMY = 25.f;
const float PewPew::BLOOM_SCALE = 2.75f;

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
		objPath = "../Assets/Models/pewpew.obj";
		damage = 50.f;
		this->textureID = 6;
	}
	else
	{
		this->size = glm::vec3(0.5f, 0.5f, 0.5f);
		objPath = "../Assets/Models/sphere.obj";
		damage = 10.f;
		this->textureID = 1;
	}

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	shaderType = SHADER_BLOOM;

	Entity::Initialize(size);

	std::vector<Vertex> buffer = Entity::LoadVertices();

	// Get the maximum x and y to create a billboard.
	float size_x, size_y;
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

	if (owner == "ENEMY")
	{
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
		markedForDeletion = true;
	}
}

void PewPew::OnCollision(Entity* other)
{
	if (other->GetName() != owner && other->GetName() != "PEWPEW")
	{
		markedForDeletion = true;
	}
}
