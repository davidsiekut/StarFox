#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(Entity *parent);

	void Update(float dt);

protected:

private:
};