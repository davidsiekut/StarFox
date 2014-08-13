#include "Cube.h"

Cube::Cube(Entity *parent, glm::vec3 size) : Entity(parent)
{
	name = "CUBE";
	this->size = size;
	objPath = "../Assets/Models/cube.obj";

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	Initialize(size);
}

Cube::Cube(Entity *parent, glm::vec3 size, glm::vec2 textureCoordinates) : Entity(parent)
{
	name = "CUBE";
	this->size = size;
	objPath = "../Assets/Models/cube.obj";

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	this->textureCoordinates = textureCoordinates;

	Initialize(size);
}

Cube::~Cube()
{
	//printf("[Cleanup] Cube deleted\n");
}

void Cube::Update(float dt)
{

}