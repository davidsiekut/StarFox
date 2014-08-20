#pragma once

#include "Enemy.h"
#include "ParticleSystem.h"

#define BOSS_ATTACK_COOLDOWN 0.2f

class UltraBoss : public Enemy
{
public:
	UltraBoss(Entity *parent);
	~UltraBoss();
	void Update(float dt);
	void OnCollision(Entity* other);
	virtual void ResetAttackCooldown() { attackCooldown = BOSS_ATTACK_COOLDOWN; }
};