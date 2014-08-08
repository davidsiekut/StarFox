#pragma once

#include "Cube.h"
#include "Entity.h"

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);
	void Update(float dt);

protected:

private:
};