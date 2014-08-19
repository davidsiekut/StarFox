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

#include "Enemy.h"
#include "PewPew.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

#define ENEMY_SCORE_PLUS 100

const float Enemy::ENEMY_SPEED_X = 30.f;
const float Enemy::ENEMY_AMPLITUDE_Y = 10.f;
const float Enemy::ENEMY_SPEED_Z = 55.f;
const float Enemy::ENEMY_LIFETIME = 35.f;
const float Enemy::ENEMY_WAVY_WAVE_SPEED = 5.f;
const float Enemy::ENEMY_SPINNY_SPIN_SPEED = 250.f;

unsigned int Enemy::arrayID = 0;
unsigned int Enemy::bufferID = 0;
unsigned int Enemy::bufferSize = 0;

Enemy::Enemy(Entity *parent) : Entity(parent)
{

}

Enemy::Enemy(Entity *parent, EnemyFactory::Direction direction, float horizontalAxis, float timeElapsed) : Entity(parent)
{
	this->name = "ENEMY";
	this->size = glm::vec3(3.f, 3.f, 3.f);
	this->SetRotation(glm::vec3(0.f, 1.f, 0.f), 90.f);
	this->objPath = "../Assets/Models/sphere.obj";
	this->textureID = 1;
	this->shaderType = SHADER_PHONG_TEXTURED;
	this->collider = glm::vec3(size.x, size.y, size.z);
	this->shield = 2;

	this->direction = direction;
	this->attackCooldown = ENEMY_ATTACK_COOLDOWN;
	this->horizontalAxis = horizontalAxis;
	this->timeElapsed = timeElapsed;

	poofSystem = nullptr;

	if (arrayID == 0)
	{
		Entity::BufferID gpuBufferIds = Initialize(size);
		arrayID = gpuBufferIds.arrayID;
		bufferID = gpuBufferIds.bufferID;
		bufferSize = gpuBufferIds.bufferSize;
	}

	vertexArrayID = arrayID;
	vertexBufferID = bufferID;
	vertexBufferSize = bufferSize;

	Entity::CreateShadow();
}

Enemy::~Enemy()
{
	if (poofSystem != nullptr)
	{
		poofSystem->SetMarkedForDeletion();
	}
}

void Enemy::Update(float dt)
{
	if (timeElapsed > ENEMY_LIFETIME)
	{
		this->SetMarkedForDeletion();
	}

	if (GetShieldAmount() <= 0)
	{
		if (poofSystem == nullptr)
		{
			Scene::GetInstance().score += ENEMY_SCORE_PLUS;

			poofSystem = new ParticleSystem(this, 1.5f, -1.f, 0.f);
			Scene::GetInstance().AddEntity(poofSystem);
		}
		this->SetScaling(glm::vec3(0.f));
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
	if (other->GetName() == "PEWPEW" && (((PewPew*)other)->GetOwner() == "PLAYER"))
	{
		TakeDamage(((PewPew*)other)->GetDamage());
	}
}