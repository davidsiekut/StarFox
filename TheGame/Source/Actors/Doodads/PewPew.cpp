#include "PewPew.h"

PewPew::PewPew(Entity *parent, glm::vec3 size) : Entity(parent)
{
	name = "PEWPEW";
	this->size = size;

	shaderType = SHADER_SOLID_COLOR;
	//shaderType = SHADER_GOURAUD;

	// TODO Make this load the pewpew.obj (it's giving an error for some reason)
	objPath = "../Assets/Models/cube.obj";

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	Initialize();

	pewpewSpeed = 25.0f;
}

void PewPew::Update(float dt)
{
	position.z += 1.0f * dt * pewpewSpeed;
}