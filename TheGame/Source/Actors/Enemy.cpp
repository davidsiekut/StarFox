#include "Enemy.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>

// TEMPORARY
#include <time.h>

Enemy::Enemy(Entity *parent) : Entity(parent)
{
	name = "ENEMY";
	scaling = glm::vec3(5, 5, 2);

	shaderType = SHADER_SOLID_COLOR;
	//shaderType = SHADER_GOURAUD;

	objPath = "../Assets/Models/cube.obj";

	Initialize();
}

void Enemy::Update(float dt)
{
}