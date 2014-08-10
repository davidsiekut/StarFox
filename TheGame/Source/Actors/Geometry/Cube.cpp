#include "Cube.h"

Cube::Cube(Entity *parent, glm::vec3 size) : Entity(parent)
{
	name = "CUBE";
	this->size = size;
	objPath = "../Assets/Models/cube.obj";
	shaderType = SHADER_TEXTURED;

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	Initialize();
}

Cube::~Cube()
{
	//printf("[Cleanup] Cube deleted\n");
}

void Cube::Update(float dt)
{

}