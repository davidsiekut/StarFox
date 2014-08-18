#include "Cube.h"

Cube::Cube(Entity *parent, glm::vec3 size) : Entity(parent)
{
	name = "CUBE";
	this->size = size;
	objPath = "../Assets/Models/cube.obj";

	collider.x = size.x;
	collider.y = size.y;
	collider.z = size.z;

	Initialize(size);
}

Cube::Cube(Entity *parent, glm::vec3 size, glm::vec2 textureCoordinates) : Entity(parent)
{
	name = "CUBE";
	this->size = size;
	objPath = "../Assets/Models/cube.obj";

	collider.x = size.x;
	collider.y = size.y;
	collider.z = size.z;

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