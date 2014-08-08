#pragma once

#include "Entity.h"
#include "EnemyFactory.h"

class Enemy : public Entity
{
public:
	Enemy(Entity *parent, EnemyFactory::Direction direction, float horizontalAxis, float timeElapsed);

	void Update(float dt);

protected:

private:
	EnemyFactory::Direction direction;
	float timeElapsed;
	float horizontalAxis;
};