#pragma once
#include "Entity.h"
class Cube :
	public Entity
{
public:
	Cube(Entity* parent = NULL);
	~Cube();

	virtual void Draw();

private:
	unsigned int vertexBufferID;
	unsigned int vertexArrayID;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};
};

