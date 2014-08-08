#pragma once

#include "Entity.h"
#include "EnemyFactory.h"

class Enemy : public Entity
{
public:
	Enemy(Entity *parent, EnemyFactory::Direction direction);

	void Update(float dt);

protected:

private:
	EnemyFactory::Direction direction;
};