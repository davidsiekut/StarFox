#include "Arwing.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>

// TEMPORARY
#include <time.h>

Arwing::Arwing(Entity *parent) : Entity(parent)
{
	name = "ARWING";
	scaling = glm::vec3(0.02, 0.02, 0.02);

	shaderType = SHADER_SOLID_COLOR;
	//shaderType = SHADER_GOURAUD;

	objPath = "../Assets/Models/arwing.obj";

	Initialize();
}

void Arwing::Update(float dt)
{
	rotationAngle -= 20 * dt;
}