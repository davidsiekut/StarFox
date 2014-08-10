#pragma once

#include "Entity.h"

class PewPew : public Entity
{
public:
	PewPew(Entity *parent, glm::vec3 size);
	void Update(float dt);

protected:

private:
	float pewpewSpeed;
};