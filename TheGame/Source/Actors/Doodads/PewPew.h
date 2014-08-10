#pragma once

#include "Entity.h"

class Arwing;

class PewPew : public Entity
{
public:
	PewPew(Entity *parent, glm::vec3 size, Arwing *arwing);
	void Update(float dt);

protected:

private:
	float pewpewSpeed;
	Arwing* arwing;
};