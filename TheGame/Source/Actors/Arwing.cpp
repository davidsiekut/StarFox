#include "Arwing.h"
#include <glm/gtc/matrix_transform.hpp>

// TEMPORARY
#include <time.h>

Arwing::Arwing(Entity *parent) : Entity(parent)
{
	name = "ARWING";
	scaling = glm::vec3(0.02, 0.02, 0.02);

	//shaderType = SHADER_SOLID_COLOR;
	shaderType = SHADER_GOURAUD;

	objPath = "../Assets/Models/arwing.obj";

	Initialize();
}

void Arwing::Update(float dt)
{
	position.z += 60 * dt;
}