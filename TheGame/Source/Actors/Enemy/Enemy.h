#pragma once

#include "Entity.h"
#include "EnemyFactory.h"

#define ENEMY_ATTACK_COOLDOWN 0.5f

class Enemy : public Entity
{
public:
	Enemy(Entity *parent);
	Enemy(Entity *parent, EnemyFactory::Direction direction, float horizontalAxis, float timeElapsed);
	~Enemy();
	void Update(float dt);
	void OnCollision(Entity* other);

	float GetAttackCooldown() { return attackCooldown; }
	virtual void ResetAttackCooldown() { attackCooldown = ENEMY_ATTACK_COOLDOWN; }

protected:
	float attackCooldown;

private:
	EnemyFactory::Direction direction;

	float timeElapsed;
	float horizontalAxis;
	ParticleSystem* poofSystem;

	static unsigned int arrayID;
	static unsigned int bufferID;
	static unsigned int bufferSize;

	static const float ENEMY_SPEED_X;
	static const float ENEMY_AMPLITUDE_Y;
	static const float ENEMY_SPEED_Z;
	static const float ENEMY_WAVY_WAVE_SPEED;
	static const float ENEMY_SPINNY_SPIN_SPEED;
	static const float ENEMY_LIFETIME;
};