#pragma once

#include "Entity.h"
#include "EnemyFactory.h"

class Enemy : public Entity
{
public:
	Enemy(Entity *parent, EnemyFactory::Direction direction, float horizontalAxis, float timeElapsed);
	~Enemy();
	void Update(float dt);
	void OnCollision(Entity* other);

protected:

private:
	EnemyFactory::Direction direction;
	float timeElapsed;
	float horizontalAxis;

	static const float ENEMY_SPEED_X;
	static const float ENEMY_AMPLITUDE_Y;
	static const float ENEMY_SPEED_Z;
	static const float ENEMY_WAVY_WAVE_SPEED;
	static const float ENEMY_SPINNY_SPIN_SPEED;
	static const float ENEMY_LIFETIME;
};