#include "Cube.h"

Cube::Cube(Entity *parent, glm::vec3 size) : Entity(parent)
{
	name = "CUBE";
	this->size = size;

	shaderType = SHADER_SOLID_COLOR;
	//shaderType = SHADER_GOURAUD;
	objPath = "../Assets/Models/cube.obj";

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	Initialize();
}

Cube::~Cube()
{
	printf("[Cleanup] Cube deleted\n");
}

void Cube::Update(float dt)
{
	//rotationAngle += 100 * dt;
}