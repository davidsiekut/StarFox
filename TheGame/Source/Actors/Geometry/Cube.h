#pragma once

#include "Entity.h"

class Cube : public Entity
{
public:
	Cube(Entity *parent, glm::vec3 size);
	virtual ~Cube();
	void Update(float dt);

protected:

private:
};