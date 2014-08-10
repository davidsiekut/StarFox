#include "PewPew.h"
#include "Arwing.h"

PewPew::PewPew(Entity *parent, glm::vec3 size, Arwing *arwing) : Entity(parent), arwing(arwing)
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

	pewpewSpeed = 25.f;
}

void PewPew::Update(float dt)
{
	position.z += 5.0f * dt * pewpewSpeed;

	if (position.z > arwing->GetPosition().z + 100.0f)
	{
		markedForDeletion = true;
	}
}