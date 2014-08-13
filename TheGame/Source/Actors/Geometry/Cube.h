#pragma once

#include "Entity.h"

class Cube : public Entity
{
public:
	Cube(Entity *parent, glm::vec3 size);
	Cube(Entity *parent, glm::vec3 size, glm::vec2 textureCoordinates);
	~Cube();
	void Update(float dt);

protected:

private:
};