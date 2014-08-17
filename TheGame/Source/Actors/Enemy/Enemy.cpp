#include "Enemy.h"
#include "PewPew.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

const float Enemy::ENEMY_SPEED_X = 30.f;
const float Enemy::ENEMY_AMPLITUDE_Y = 10.f;
const float Enemy::ENEMY_SPEED_Z = 55.f;
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

	poofSystem = nullptr;

	Initialize(size);
}

Enemy::~Enemy()
{
	//printf("[Cleanup] Enemy deleted\n");
	if (poofSystem != nullptr)
	{
		poofSystem->markedForDeletion = true;
	}
}

void Enemy::Update(float dt)
{
	if (timeElapsed > ENEMY_LIFETIME)
	{
		markedForDeletion = true;
	}

	if (GetShieldAmount() <= 0)
	{
		if (poofSystem == nullptr)
		{
			Scene::GetInstance().score += 111;

			poofSystem = new ParticleSystem(this, 1.5f, -1.f, 0.f);
			Scene::GetInstance().AddEntity(poofSystem);
		}
		this->SetScaling(glm::vec3(0.f));
		//markedForDeletion = true;
		return;
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

	if (attackCooldown > 0)
	{
		attackCooldown -= dt;
	}
}

void Enemy::OnCollision(Entity* other)
{
	if (other->GetName() == "PEWPEW" && (((PewPew*)other)->owner == "PLAYER"))
	{
		TakeDamage(((PewPew*)other)->damage);
	}
}