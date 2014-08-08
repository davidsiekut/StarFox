#include "Arwing.h"

Arwing::Arwing(Entity *parent) : Entity(parent)
{
	name = "ARWING";
	size = glm::vec3(0.02, 0.02, 0.02);

	shaderType = SHADER_SOLID_COLOR;
	//shaderType = SHADER_GOURAUD;
	objPath = "../Assets/Models/arwing.obj";

	COLLIDE_X = 5.f;
	COLLIDE_Y = 5.f;
	COLLIDE_Z = 5.f;

	Initialize();
}

void Arwing::Update(float dt)
{
	position.z += 10 * dt;
	//printf("%f, %f, %f\n", GetPositionWorld().x, GetPositionWorld().y, GetPositionWorld().z);
}