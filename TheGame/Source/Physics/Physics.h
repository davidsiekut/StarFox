#pragma once

#include "Entity.h"

class Physics
{
public:
	static bool CheckAABBCollision(Entity *b1, Entity *b2);
	static bool CheckOBBCollision(Entity *b1, Entity *b2);
};