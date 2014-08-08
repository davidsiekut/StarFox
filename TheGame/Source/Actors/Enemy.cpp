#include "Enemy.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>

// TEMPORARY
#include <time.h>

Enemy::Enemy(Entity *parent, EnemyFactory::Direction direction, float horizontalAxis, float timeElapsed) : Entity(parent)
{
	name = "ENEMY";
	size = glm::vec3(5, 5, 20);

	shaderType = SHADER_SOLID_COLOR;
	//shaderType = SHADER_GOURAUD;

	objPath = "../Assets/Models/cube.obj";

	Initialize();

	this->direction = direction;
	this->horizontalAxis = horizontalAxis;
	this->timeElapsed = timeElapsed;

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;
}

Enemy::~Enemy()
{
	printf("[Cleanup] Enemy deleted\n");
}

void Enemy::Update(float dt)
{
	if (direction == EnemyFactory::Direction::LEFT)
	{
		position.x -= dt * 15.f;
	}
	else
	{
		position.x += dt * 15.f;
	}

	timeElapsed += dt * 5.f;
	position.y = horizontalAxis + glm::sin(timeElapsed) * 7.f;

	position.z += dt * 15.f;

	if (timeElapsed > 30.f)
	{
		markedForDeletion = true;
	}
}