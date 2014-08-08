#include "Cube.h"

Cube::Cube(Entity *parent, glm::vec3 size) : Entity(parent)
{
	name = "CUBE";
	this->size = size;
	shaderType = SHADER_SOLID_COLOR;
	//shaderType = SHADER_GOURAUD;
	objPath = "../Assets/Models/cube.obj";

	Initialize();
}

void Cube::Update(float dt)
{
	rotationAngle += 100 * dt;
}