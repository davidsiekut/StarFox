#include "Enemy.h"
#include "PewPew.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

const float Enemy::ENEMY_SPEED_X = 20.f;
const float Enemy::ENEMY_AMPLITUDE_Y = 10.f;
const float Enemy::ENEMY_SPEED_Z = 60.f;
const float Enemy::ENEMY_LIFETIME = 40.f;
const float Enemy::ENEMY_WAVY_WAVE_SPEED = 5.f;
const float Enemy::ENEMY_SPINNY_SPIN_SPEED = 250.f;

Enemy::Enemy(Entity *parent, EnemyFactory::Direction direction, float horizontalAxis, float timeElapsed) : Entity(parent)
{
	name = "ENEMY";
	size = glm::vec3(5.f, 5.f, 5.f);
	objPath = "../Assets/Models/sphere.obj";
	shaderType = SHADER_PHONG_TEXTURED;
	textureID = 1;

	shield = 150;

	this->direction = direction;
	this->horizontalAxis = horizontalAxis;
	this->timeElapsed = timeElapsed;

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	Initialize(size);
}

Enemy::~Enemy()
{
	//printf("[Cleanup] Enemy deleted\n");
}

void Enemy::Update(float dt)
{
	if (GetShieldAmount() <= 0)
	{
		Scene::GetInstance().AddStaticParticleSystem(this->GetPosition(), 1.f, 1.f, 0.f);

		markedForDeletion = true;
		Scene::GetInstance().score += 111;
	}

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

	if (attackCooldown <= 0)
	{
		//attackCooldown = 1.f;
		//Scene::GetInstance().Fire();
	}
	else
	{
		attackCooldown -= dt;
	}
}

void Enemy::OnCollision(Entity* other)
{
	if (other->GetName() == "PEWPEW" && (((PewPew*)other)->owner == "PLAYER"))
	{
		Scene::GetInstance().AddStaticParticleSystem(this->GetPosition(), 0.2f, 0.2f, 0.f);
		TakeDamage(((PewPew*)other)->damage);
	}
}