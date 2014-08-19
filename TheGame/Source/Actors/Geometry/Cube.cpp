#include "Cube.h"

Cube::Cube(Entity *parent, glm::vec3 size) : Entity(parent)
{
	this->name = "CUBE";
	this->size = size;
	this->objPath = "../Assets/Models/cube.obj";
	this->collider = glm::vec3(size.x, size.y, size.z);

	Initialize(size);
}

Cube::Cube(Entity *parent, glm::vec3 size, glm::vec2 textureCoordinates) : Entity(parent)
{
	this->name = "CUBE";
	this->size = size;
	this->objPath = "../Assets/Models/cube.obj";
	this->textureCoordinates = textureCoordinates;
	this->collider = glm::vec3(size.x, size.y, size.z);

	Initialize(size);
}

Cube::~Cube()
{
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteVertexArrays(1, &vertexBufferID);
}

void Cube::Update(float dt)
{

}