#include "Enemy.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>

const float Enemy::ENEMY_SPEED_X = 20.f;
const float Enemy::ENEMY_AMPLITUDE_Y = 10.f;
const float Enemy::ENEMY_SPEED_Z = 80.f;
const float Enemy::ENEMY_LIFETIME = 30.f;
const float Enemy::ENEMY_WAVY_WAVE_SPEED = 5.f;
const float Enemy::ENEMY_SPINNY_SPIN_SPEED = 250.f;

Enemy::Enemy(Entity *parent, EnemyFactory::Direction direction, float horizontalAxis, float timeElapsed) : Entity(parent)
{
	name = "ENEMY";
	size = glm::vec3(5.f, 5.f, 5.f);
	objPath = "../Assets/Models/cube.obj";

	this->direction = direction;
	this->horizontalAxis = horizontalAxis;
	this->timeElapsed = timeElapsed;

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	Initialize();
}

Enemy::~Enemy()
{
	//printf("[Cleanup] Enemy deleted\n");
}

void Enemy::Update(float dt)
{
	rotationAngle += dt * ENEMY_SPINNY_SPIN_SPEED;

	if (direction == EnemyFactory::Direction::LEFT)
	{
		position.x -= dt * ENEMY_SPEED_X;
	}
	else
	{
		position.x += dt * ENEMY_SPEED_X;
	}

	timeElapsed += dt * ENEMY_WAVY_WAVE_SPEED;

	position.y = horizontalAxis + glm::sin(timeElapsed) * ENEMY_AMPLITUDE_Y;
	position.z += dt * ENEMY_SPEED_Z;

	if (timeElapsed > ENEMY_LIFETIME)
	{
		markedForDeletion = true;
	}
}